#ifndef CG6_ALGORITHM_H
#define CG6_ALGORITHM_H

#include <SFML/Graphics.hpp>
#include "pixbuf.h"
#include "object.h"
#include "camera.h"
void draw_line(Pixbuf& pixbuf, sf::Vector2i begin, sf::Vector2i end, const sf::Color& border_color);
Object constructRotationFigure(const std::vector<sf::Vector3f>& points, Camera& cam);

#endif //CG6_ALGORITHM_H
