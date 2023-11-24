#ifndef CG6_VERTEX_H
#define CG6_VERTEX_H

#include <SFML/Graphics.hpp>
#include "matrix.h"

class Object;

class Vertex {
public:
    float x;
    float y;
    float z;

public:
    Vertex();
    Vertex(float x, float y, float z);
    Vertex(float x, float y, float z, const sf::Color& color);
    Vertex(std::initializer_list<float> coords);
    Vertex(const sf::Vector3f& vec);

    Vertex operator*(const Matrix<3>& m) const;
    Vertex operator*(const Matrix<4>& m) const;
    Vertex operator+(const sf::Vector3f& other) const;
    Vertex operator-(const sf::Vector3f& other) const;
    Vertex operator-() const;

    void calculateNormal(const std::vector<int>& containingPolygons, Object* obj);
    const sf::Color& color() const { return _color; }
    void setColor(const sf::Color& color) { _color = color; }
    const sf::Vector3f& normal() const { return _normal; }
    sf::Vector3f toVec3() const { return { x, y, z }; }

private:
    sf::Vector3f _normal;
    sf::Color _color;

private:
    void setNormal(const sf::Vector3f& normal) { _normal = normal; }
};


#endif //CG6_VERTEX_H
