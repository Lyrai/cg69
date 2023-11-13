#include "algorithm.h"

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

Object constructRotationFigure(const std::vector<sf::Vector3f> &points,Camera& cam)
{
    int steps = 10;
    float count = 360.f / steps;
    std::vector<sf::Vector3f> result;
    result.reserve(points.size() * steps);
    int pointCount = points.size();
    for (int i = 0; i < steps; i++)
    {
        auto temp = Object::rotatedAroundY(points, count*i);
        for( auto point :temp)
            result.push_back(point);
    }

    Polygons polygons;
    for (int i = 0; i < steps; ++i) {
        for (int j = 0; j < pointCount - 1; ++j) {
            polygons.emplace_back(std::vector<int> {pointCount * i + j, pointCount * i + j + 1, (pointCount * (i + 1) + j + 1) % (pointCount * (steps)),
                                                    (pointCount * (i + 1) + j) % (pointCount * (steps))});
        }
    }

    result.emplace_back(0, points[0].y, 0);
    result.emplace_back(0, points[pointCount - 1].y, 0);
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