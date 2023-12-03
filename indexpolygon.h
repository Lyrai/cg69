#ifndef CG6_INDEXPOLYGON_H
#define CG6_INDEXPOLYGON_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "object.h"
#include "indexvertex.h"

class IndexPolygon {
public:
    IndexPolygon(const std::vector<IndexVertex>& indices, const sf::Color& = sf::Color(220, 220, 220));

    const std::vector<IndexVertex>& indices() const { return _indices; }
    const sf::Vector3f& normal() const { return _normal; }
    const sf::Vector3f& center() const { return _center; }
    const sf::Vector3f& center(const Object* object);
    void calculateNormal(const Object* object);
    void calculateNormal(const Object* object, const sf::Vector3f& directionToPolygon);
    const sf::Color& color() const { return _color; }
    void setTexture(const sf::Image& texture);
    void setColor(const sf::Color& color) { _color = color; }
    const sf::Uint8* texture() const { return _texture; }
    const sf::Vector2u& textureSize() const { return _textureSize; }
    bool hasTexture() const { return _hasTexture; }

private:
    std::vector<IndexVertex> _indices;
    sf::Vector3f _normal;
    sf::Vector3f _center;
    sf::Color _color;
    const sf::Uint8* _texture;
    sf::Vector2u _textureSize;
    bool _hasTexture;

    friend class Object;
};


#endif //CG6_INDEXPOLYGON_H
