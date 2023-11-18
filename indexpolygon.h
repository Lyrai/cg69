#ifndef CG6_INDEXPOLYGON_H
#define CG6_INDEXPOLYGON_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "object.h"

class IndexPolygon {
public:
    IndexPolygon(const std::vector<int>& indices);

    const std::vector<int>& indices() const { return _indices; }
    const sf::Vector3f& normal() const { return _normal; }
    const sf::Vector3f& center() const { return _center; }
    void calculateNormal(const Object* object);

private:
    std::vector<int> _indices;
    sf::Vector3f _normal;
    sf::Vector3f  _center;
    friend class Object;
};


#endif //CG6_INDEXPOLYGON_H
