#ifndef CG6_ALGORITHM_H
#define CG6_ALGORITHM_H

#include <SFML/Graphics.hpp>
#include "pixbuf.h"
#include "object.h"
#include "camera.h"
void draw_line(Pixbuf& pixbuf, sf::Vector2i begin, sf::Vector2i end, const sf::Color& border_color);
Object constructRotationFigure(const std::vector<sf::Vector3f>& points, Camera& cam,int steps,Axis axis);
sf::Vector3f planeIntersection(const sf::Vector3f &begin, const sf::Vector3f &end, float z);
std::vector<IndexPolygon> triangulate(IndexPolygon& polygon, Object* obj);
sf::Color getPixel(const sf::Uint8* texture, const sf::Vector2u& textureSize, const sf::Vector2f& coords);
void drawCircle(Pixbuf* pixbuf, const sf::Vector2i& center, const sf::Vector3f& viewSpace, int r, float* zbuffer);
std::vector<IndexPolygon> subdivide(IndexPolygon& polygon, Object* obj);

#endif //CG6_ALGORITHM_H
