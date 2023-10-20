#ifndef LINE_H
#define LINE_H

#include <SFML/Graphics.hpp>
#include "polygon.h"

enum class RelativePosition {
    Left,
    Right,
    On
};

class Line: public Polygon
{
public:
    Line(const sf::Vector3f& begin, const sf::Vector3f& end);

    bool contains(const sf::Vector3f& point);
};

#endif // LINE_H
