#include "vertex.h"
#include "indexpolygon.h"

Vertex::Vertex(float x, float y, float z)
    : x(x), y(y), z(z)
{ }

Vertex::Vertex(std::initializer_list<float> coords)
    : x(data(coords)[0]), y(data(coords)[1]), z(data(coords)[2])
{ }

Vertex Vertex::operator*(const Matrix<3> &m) const {
    sf::Vector4f v(x, y, z, 1);
    Matrix<4> mm {{
        m(0, 0), m(0, 1), m(0, 2), 0,
        m(1, 0), m(1, 1), m(1, 2), 0,
        m(2, 0), m(2, 1), m(2, 2), 0,
        0, 0, 0, 1
    }};
    auto tmp = v * mm;
    Vertex result { tmp.x, tmp.y, tmp.z };
    result.setNormal(_normal);
    result.setColor(_color);

    return result;
}

Vertex Vertex::operator*(const Matrix<4> &m) const {
    sf::Vector4f v(x, y, z, 1);
    auto tmp = v * m;
    Vertex result { tmp.x / tmp.w, tmp.y / tmp.w, tmp.z / tmp.w };
    result.setNormal(_normal);
    result.setColor(_color);

    return result;
}

Vertex Vertex::operator-() const {
    Vertex result { -x, -y, -z };
    result.setNormal(_normal);
    result.setColor(_color);

    return result;
}

Vertex Vertex::operator+(const sf::Vector3f& other) const {
    Vertex result { x + other.x, y + other.y, z + other.y };
    result.setNormal(_normal);
    result.setColor(_color);

    return result;
}

Vertex Vertex::operator-(const sf::Vector3f &other) const {
    Vertex result = *this + -other;
    result.setNormal(_normal);
    result.setColor(_color);

    return result;
}

void Vertex::calculateNormal(const std::vector<int> &containingPolygons, Object* obj) {
    sf::Vector3f sum;
    for(const auto& polygon: containingPolygons) {
        sum += obj->polygons()[polygon].normal();
    }

    _normal = normalize({ sum.x / containingPolygons.size(), sum.y / containingPolygons.size(), sum.z / containingPolygons.size() });
}

Vertex::Vertex()
    : x(0), y(0), z(0)
{ }

Vertex::Vertex(const sf::Vector3f &vec)
    : x(vec.x), y(vec.y), z(vec.z)
{ }

Vertex::Vertex(float x, float y, float z, const sf::Color &color)
    : x(x), y(y), z(z), _color(color)
{ }
