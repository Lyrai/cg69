#ifndef RECTITEM_H
#define RECTITEM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstring>

#include "matrix.h"

class Line;

class Polygon
{
public:
    Polygon();
    Polygon(const std::vector<sf::Vector3f>& points);
    void addPoint(const sf::Vector3f& point);

    const std::vector<sf::Vector3f> &getPoints() const;

    void moveBy(const sf::Vector3f& v);
    std::vector<sf::Vector3f> movedBy(const sf::Vector3f& v) const;
    void rotateAround(Line* line, float cosa, float sina);
    std::vector<sf::Vector3f> rotatedAround(Line* line, float cosa, float sina) const;
    void scaleAround(const sf::Vector3f& p, float kx, float ky, float kz);
    std::vector<sf::Vector3f> scaledAround(const sf::Vector3f& p, float kx, float ky, float kz) const;

    sf::Vector3f center();

protected:
    void applyTransformation(const Matrix<3>& m, const sf::Vector3f& v);

protected:
    std::vector<sf::Vector3f> points;
};

#endif // RECTITEM_H
