#include <cmath>
#include "object.h"

sf::Vector3f Object::center() const {
    sf::Vector3f result;
    for(const auto& vertex: vertices()) {
        result += vertex;
    }

    return { result.x / vertices().size(), result.y / vertices().size(), result.z / vertices().size() };
}

Object::Object(const sf::Vector3f& position, const std::vector<sf::Vector3f> &vertices, const std::vector<std::pair<int, int>> &edges)
    : Transform(position), _vertices(vertices), _edges(edges)
{ }

std::vector<sf::Vector3f> Object::movedBy(const sf::Vector3f &v) const {
    std::vector<sf::Vector3f> result;
    result.reserve(vertices().size());

    for (const auto &point: vertices()) {
        result.push_back(point + v);
    }

    return result;
}

std::vector<sf::Vector3f> Object::rotatedAround(Line *line, float cosa, float sina) const {
    return std::vector<sf::Vector3f>();
}

std::vector<sf::Vector3f> Object::scaledAround(const sf::Vector3f &p, float kx, float ky, float kz) const {
    return std::vector<sf::Vector3f>();
}

std::vector<sf::Vector3f> Object::transformed(const Matrix<4> &m) const {
    std::vector<sf::Vector3f> result;
    result.reserve(vertices().size());

    for(int i = 0; i < vertices().size(); ++i) {
        auto tmp = sf::Vector4f(vertices()[i].x, vertices()[i].y, vertices()[i].z, 1);
        auto v4 = tmp * m;
        result.emplace_back(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);
    }

    return result;
}

std::vector<sf::Vector3f> Object::rotatedAroundX(float angle) {
    std::vector<sf::Vector3f> result;
    result.reserve(vertices().size());

    float sina = sin(angle * M_PI / 180);
    float cosa = cos(angle * M_PI / 180);
    Matrix<4> m = Matrix<4>::identity();
    m(1, 1) = cosa;
    m(1, 2) = sina;
    m(2, 1) = -sina;
    m(2, 2) = cosa;

    for(int i = 0; i < vertices().size(); ++i) {
        auto tmp = sf::Vector4f(vertices()[i].x, vertices()[i].y, vertices()[i].z, 1);
        auto v4 = tmp * m;
        result.emplace_back(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);
    }

    return result;
}

std::vector<sf::Vector3f> Object::rotatedAroundY(float angle) {
    std::vector<sf::Vector3f> result;
    result.reserve(vertices().size());

    float sina = sin(angle * M_PI / 180);
    float cosa = cos(angle * M_PI / 180);
    Matrix<4> m = Matrix<4>::identity();
    m(0, 0) = cosa;
    m(0, 2) = sina;
    m(2, 0) = -sina;
    m(2, 2) = cosa;

    for(int i = 0; i < vertices().size(); ++i) {
        auto tmp = sf::Vector4f(vertices()[i].x, vertices()[i].y, vertices()[i].z, 1);
        auto v4 = tmp * m;
        result.emplace_back(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);
    }

    return result;
}
