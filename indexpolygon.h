#ifndef CG6_INDEXPOLYGON_H
#define CG6_INDEXPOLYGON_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "object.h"

class IndexPolygon {
public:
    IndexPolygon(const std::vector<int>& indices, const sf::Color& = sf::Color(220, 220, 220));

    const std::vector<int>& indices() const { return _indices; }
    const sf::Vector3f& normal() const { return _normal; }
    const sf::Vector3f& center() const { return _center; }
    void calculateNormal(const Object* object);
    void calculateNormal(const Object* object, const sf::Vector3f& directionToPolygon);
    const sf::Color& color() const { return _color; }
    void setColor(const sf::Color& color) { _color = color; }

private:
    std::vector<int> _indices;
    sf::Vector3f _normal;
    sf::Vector3f _center;
    sf::Color _color;
    friend class Object;
};


#endif //CG6_INDEXPOLYGON_H
