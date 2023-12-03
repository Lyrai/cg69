#include "indexvertex.h"

IndexVertex::IndexVertex(int index, const sf::Vector2f& texCoords)
    : _index(index), _coords(texCoords), _hasTexture(true)
{ }

IndexVertex::IndexVertex(int index)
    : _index(index), _hasTexture(false)
{ }
