#include <cmath>
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
            polygons.emplace_back(std::vector<IndexVertex> {pointCount * i + j, pointCount * i + j + 1, (pointCount * (i + 1) + j + 1) % (pointCount * (steps)),
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
        polygons.emplace_back(std::vector<IndexVertex> {result.size() - 2, i * pointCount, ((i + 1) * pointCount) % (steps * pointCount)});
        polygons.emplace_back(std::vector<IndexVertex> {result.size() - 1, i * pointCount + pointCount - 1, ((i + 1) * pointCount + pointCount - 1) % (steps * pointCount)});
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


std::vector<IndexPolygon> triangulate(IndexPolygon &polygon, Object* obj) {
    std::vector<IndexPolygon> result;

    int idx = obj->vertices().size();
    sf::Vector2f center;
    for(const auto& vertex: polygon.indices()) {
        center += vertex.coords();
    }
    auto size = polygon.indices().size();
    IndexVertex newVertex {idx, {center.x / size, center.y / size} };
    
    obj->vertices().emplace_back(polygon.center(obj));

    auto& vertices = polygon.indices();
    for(int i = 0; i < vertices.size(); ++i) {
        auto next = (i + 1) % vertices.size();
        IndexPolygon triangle {
            {
                newVertex,
                {vertices[i].index(), vertices[i].coords()},
                {vertices[next].index(), vertices[next].coords()}
            },
            polygon.color()
        };
        result.push_back(triangle);
    }

    return result;
}

sf::Color getPixel(const sf::Uint8* texture, const sf::Vector2u& textureSize, const sf::Vector2f& coords) {
    sf::Vector2u pixelCoords(textureSize.x * coords.x, textureSize.y * coords.y);
    auto ptr = texture + (pixelCoords.y * textureSize.x + pixelCoords.x) * 4;

    return {ptr[0], ptr[1], ptr[2], ptr[3]};
}

void drawZBuffer(Pixbuf* pixbuf, int x, int y, float* zbuffer, const sf::Vector3f& viewSpace, const sf::Color& color, float ratio) {
    auto bufferIdx = pixbuf->size.x * y + x;
    if(zbuffer[bufferIdx] > viewSpace.z) {
        zbuffer[bufferIdx] = viewSpace.z;
        auto c = pixbuf->at(x, y).color();
        pixbuf->at(x, y) = sf::Color(c.r * (1 - ratio) + color.r * ratio, c.g * (1 - ratio) + color.g * ratio, c.b * (1 - ratio) + color.b * ratio);
    }
}

void drawCircle(Pixbuf* pixbuf, const sf::Vector2i& center, const sf::Vector3f& viewSpace, int r, float* zbuffer) {
    int x = 0;
    int y = r;
    int delta = 1 - 2 * r;
    int error = 0;
    int x1 = center.x;
    int y1 = center.y;
    sf::Color color {200, 200, 50, 1};
    while (y >= x) {
        drawZBuffer(pixbuf, x1 + x, y1 + y, zbuffer, viewSpace, color, 0);
        drawZBuffer(pixbuf, x1 + x, y1 - y, zbuffer, viewSpace, color, 0);
        for(int yy = y1 - y; yy < y1 + y; ++yy) {
            auto sqrMagnitude = x * x + (yy - y1) * (yy - y1);
            drawZBuffer(pixbuf, x1 + x, yy, zbuffer, viewSpace, color, pow(1 - sqrMagnitude / (float)(r * r), 2));
        }
        drawZBuffer(pixbuf, x1 - x, y1 + y, zbuffer, viewSpace, color, 0);
        drawZBuffer(pixbuf, x1 - x, y1 - y, zbuffer, viewSpace, color, 0);
        for(int yy = y1 - y; yy < y1 + y; ++yy) {
            auto sqrMagnitude = -x * -x + (yy - y1) * (yy - y1);
            drawZBuffer(pixbuf, x1 - x, yy, zbuffer, viewSpace, color, pow(1 - sqrMagnitude / (float)(r * r), 2));
        }
        drawZBuffer(pixbuf, x1 + y, y1 + x, zbuffer, viewSpace, color, 0);
        drawZBuffer(pixbuf, x1 + y, y1 - x, zbuffer, viewSpace, color, 0);
        for(int yy = y1 - x; yy < y1 + x; ++yy) {
            auto sqrMagnitude = y * y + (yy - y1) * (yy - y1);
            drawZBuffer(pixbuf, x1 + y, yy, zbuffer, viewSpace, color, pow(1 - sqrMagnitude / (float)(r * r), 2));
        }
        drawZBuffer(pixbuf, x1 - y, y1 + x, zbuffer, viewSpace, color, 0);
        drawZBuffer(pixbuf, x1 - y, y1 - x, zbuffer, viewSpace, color, 0);
        for(int yy = y1 - x; yy < y1 + x; ++yy) {
            auto sqrMagnitude = -y * -y + (yy - y1) * (yy - y1);
            drawZBuffer(pixbuf, x1 - y, yy, zbuffer, viewSpace, color, pow(1 - sqrMagnitude / (float)(r * r), 2));
        }
        error = 2 * (delta + y) - 1;
        if ((delta < 0) && (error <= 0)) {
            delta += 2 * ++x + 1;
            continue;
        }
        if ((delta > 0) && (error > 0)) {
            delta -= 2 * --y + 1;
            continue;
        }
        delta += 2 * (++x - --y);
    }
}

std::vector<IndexPolygon> subdivide(IndexPolygon& polygon, Object* obj) {
    std::vector<IndexPolygon> result;

    int idx = obj->vertices().size();
    sf::Vector2f center;
    for(const auto& vertex: polygon.indices()) {
        center += vertex.coords();
    }
    auto size = polygon.indices().size();
    IndexVertex newVertex {idx, {center.x / size, center.y / size} };

    obj->vertices().emplace_back(polygon.center(obj));

    for(int i = 0; i < polygon.indices().size(); ++i) {
        auto next = (i + 1) % polygon.indices().size();
        auto idx1 = obj->vertices().size();
        auto point = obj->vertices()[next].toVec3() - obj->vertices()[i].toVec3();
        obj->vertices().push_back({point.x / 2, point.y / 2, point.z / 2});
        result.emplace_back(std::vector<IndexVertex> {idx, idx1, polygon.indices()[i]}, polygon.color());
    }

    return result;
}