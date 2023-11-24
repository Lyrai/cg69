#include "algorithm.h"
#include "gui.h"

void draw_line(Pixbuf& pixbuf, sf::Vector2i begin, sf::Vector2i end, const sf::Color& border_color) {
    auto d = end - begin;
    auto steep = abs(d.y) > abs(d.x);

    if(steep) {
        std::swap(begin.x, begin.y);
        std::swap(end.x, end.y);
    }

    if(begin.x > end.x) {
        std::swap(begin, end);
    }

    d = end - begin;

    auto y_step = d.y / abs(d.y);
    auto y = begin.y;

    d.y = abs(d.y);
    int di = 2 * d.y - d.x;

    for(int x = begin.x; x <= end.x; ++x) {
        pixbuf.at(steep ? y : x, steep ? x : y) = border_color;

        di += 2 * d.y - ((di >= 0) * 2 * d.x);
        y += (di >= 0) * y_step;
    }
}

Object constructRotationFigure(const std::vector<sf::Vector3f> &points,Camera& cam,int steps,Axis axis)
{
    float count = 360.f / steps;
    std::vector<Vertex> result;
    auto new_points = points;
    result.reserve(points.size() * steps);
    switch (axis) {
        case Axis::X:
        {
            new_points = Object::rotatedAroundZ(points, 90);
            break;
        }
        case Axis::Z:
        {
            new_points = Object::rotatedAroundX(points, 90);
            break;
        }
    }
    int pointCount = new_points.size();
    switch (axis) {
        case Axis::X:
        {
            for (int i = 0; i < steps; i++)
            {
                auto temp = Object::rotatedAroundX(new_points, count*i);
                for( auto point :temp)
                    result.push_back(point);
            }
            break;
        }
        case Axis::Y:
        {
            for (int i = 0; i < steps; i++)
            {
                auto temp = Object::rotatedAroundY(new_points, count*i);
                for( auto point :temp)
                    result.push_back(point);
            }
            break;
        }
        case Axis::Z:
        {
            for (int i = 0; i < steps; i++)
            {
                auto temp = Object::rotatedAroundZ(new_points, count*i);
                for( auto point :temp)
                    result.push_back(point);
            }
            break;
        }
    }

    Polygons polygons;
    for (int i = 0; i < steps; ++i) {
        for (int j = 0; j < pointCount - 1; ++j) {
            polygons.emplace_back(std::vector<int> {pointCount * i + j, pointCount * i + j + 1, (pointCount * (i + 1) + j + 1) % (pointCount * (steps)),
                                                    (pointCount * (i + 1) + j) % (pointCount * (steps))});
        }
    }
    switch (axis) {
        case Axis::X:
        {
            result.emplace_back(new_points[0].x, 0, 0);
            result.emplace_back(new_points[pointCount - 1].x, 0, 0);
            break;
        }
        case Axis::Y:
        {
            result.emplace_back(0, new_points[0].y, 0);
            result.emplace_back(0, new_points[pointCount - 1].y, 0);
            break;
        }
        case Axis::Z:
        {
            result.emplace_back(0, 0, new_points[0].z);
            result.emplace_back(0, 0, new_points[pointCount - 1].z);
            break;
        }


    }

    for (int i = 0; i < steps; ++i) {
        polygons.emplace_back(std::vector<int> {(int)(result.size() - 2), i * pointCount, ((i + 1) * pointCount) % (steps * pointCount)});
        polygons.emplace_back(std::vector<int> {(int)(result.size() - 1), i * pointCount + pointCount - 1, ((i + 1) * pointCount + pointCount - 1) % (steps * pointCount)});
    }

    return Object({0, 0, 0}, result, polygons);
}

sf::Vector3f planeIntersection(const sf::Vector3f &begin, const sf::Vector3f &end, float z) {
    auto first = begin;
    auto second = end;

    auto normal = sf::Vector3f(0, 0, 1);
    auto ca = sf::Vector3f(1, 1, z) - first;
    auto vcn = dot(ca, normal);
    auto cv = second - first;
    auto vcm = dot(cv, normal);
    auto k = vcn / vcm;

    auto x = sf::Vector3f(cv.x * k, cv.y * k, cv.z * k);
    x += first;

    return x;
}


std::vector<IndexPolygon> triangulate(const IndexPolygon &polygon) {
    std::vector<IndexPolygon> result;

    if(polygon.indices().size() == 3) {
        return { polygon };
    }

    auto& vertices = polygon.indices();
    for(int i = 2; i < vertices.size(); ++i) {
        IndexPolygon triangle({ vertices[0], vertices[i - 1], vertices[i] }, polygon.color());
        result.push_back(triangle);
    }

    return result;
}