#include "matrix.h"

sf::Vector4f::Vector4f(float x, float y, float z, float w): x(x), y(y), z(z), w(w)
{ }

sf::Vector4f sf::Vector4f::operator+(const sf::Vector4f &other) {
    return sf::Vector4f(x + other.x, y + other.y, z + other.z, w + other.w);
}

sf::Vector4f sf::Vector4f::operator-(const sf::Vector4f &other) {
    return sf::Vector4f(x + other.x, y + other.y, z + other.z, w + other.w);
}
