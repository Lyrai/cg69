#include "line.h"

Line::Line(const sf::Vector3f& begin, const sf::Vector3f& end): Polygon({begin, end})
{ }

bool Line::contains(const sf::Vector3f &point)
{
    auto begin = points[0];
    auto end = points[1];
    if(point == begin || point == end) {
        return true;
    }
    if((point.x - begin.x) * (end.y - begin.y) - (point.y - begin.y) * (end.x - begin.x) < 1e-1) {
        auto x1 = begin.x > end.x ? end.x : begin.x;
        auto x2 = begin.x < end.x ? end.x : begin.x;

        auto y1 = begin.y > end.y ? end.y : begin.y;
        auto y2 = begin.y < end.y ? end.y : begin.y;

        return point.x >= x1 && point.x <= x2 && point.y >= y1 && point.y <= y2;
    }

    return false;
}
