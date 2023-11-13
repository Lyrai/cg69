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

    std::vector<sf::Vector3f> result1;
    for (const auto &item: result) {
        auto vertex = sf::Vector4f(item.x, item.y, item.z, 1);
        auto worldSpace = vertex * cam.objectToWorldMatrix();
        result1.emplace_back(worldSpace.x, worldSpace.y, worldSpace.z);
    }


    return Object({0, 0, 0}, Object::movedBy(result1, -cam.getPosition()), polygons);
}
