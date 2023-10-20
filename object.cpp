#include "object.h"

sf::Vector3f Object::center() const {
    sf::Vector3f result;
    for(const auto& vertex: vertices) {
        result += vertex;
    }

    return { result.x / vertices.size(), result.y / vertices.size(), result.z / vertices.size() };
}

Object::Object(const sf::Vector3f& position, const std::vector<sf::Vector3f> &vertices, const std::vector<std::pair<int, int>> &edges)
    : Transform(position), vertices(vertices), _edges(edges)
{ }

std::vector<sf::Vector3f> Object::movedBy(const sf::Vector3f &v) const {
    static constexpr float values[9] {1, 0, 0,
                                      0, 1, 0,
                                      0, 0, 1};
    static constexpr Matrix<3> matrix(values);

    std::vector<sf::Vector3f> result;
    result.reserve(vertices.size());

    for (const auto &point: vertices) {
        result.push_back(matrix * point + v);
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
    result.reserve(vertices.size());

    for(int i = 0; i < vertices.size(); ++i) {
        auto v4 = sf::Vector4f(vertices[i].x, vertices[i].y, vertices[i].z, 1);
        auto tmp = m * v4;
        result[i] = sf::Vector3f(tmp.x, tmp.y, tmp.z);
    }

    return result;
}