#include <cmath>
#include <set>
#include <utility>
#include "object.h"
#include "indexpolygon.h"
#include "algorithm.h"

sf::Vector3f Object::center() const {
    sf::Vector3f result;
    for(const auto& vertex: vertices()) {
        result += vertex;
    }

    return { result.x / vertices().size(), result.y / vertices().size(), result.z / vertices().size() };
}

Object::Object(const sf::Vector3f& position, const std::vector<sf::Vector3f> &vertices, const Edges &edges)
    : Transform(position), _vertices(vertices), _edges(edges)
{ }

Object::Object(const sf::Vector3f &position, std::vector<sf::Vector3f> &&vertices, Edges&& indices)
    : Transform(position), _vertices(vertices), _edges(indices)
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

    for(auto vertex : vertices()) {
        auto tmp = sf::Vector4f(vertex.x, vertex.y, vertex.z, 1);
        auto v4 = tmp * m;
        result.emplace_back(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);
    }

    return result;
}

Object::Object(const sf::Vector3f &position, const std::vector<sf::Vector3f> &vertices, Polygons& polygons)
        : Transform(position), _vertices(vertices)
{
    for(auto& polygon: polygons) {
        polygon.setColor({(sf::Uint8)(rand() % 256), (sf::Uint8)(rand() % 256), (sf::Uint8)(rand() % 256)});
        auto triangles = triangulate(polygon);
        for (auto &triangle: triangles) {
            triangle.calculateNormal(this);
            _polygons.push_back(triangle);
        }
    }
}

Object::Object(const sf::Vector3f &position, const std::vector<sf::Vector3f> &vertices, Polygons& polygons, const sf::Vector3f& polygonFaceDirection)
    : Transform(position), _vertices(vertices)
{
    for(auto& polygon: polygons) {
        polygon.setColor({(sf::Uint8)(rand() % 256), (sf::Uint8)(rand() % 256), (sf::Uint8)(rand() % 256)});
        auto triangles = triangulate(polygon);
        for (auto &triangle: triangles) {
            triangle.calculateNormal(this, polygonFaceDirection);
            _polygons.push_back(triangle);
        }
    }
}

std::vector<sf::Vector3f> Object::transformed(const std::vector<sf::Vector3f>& vertices, const Matrix<4> &m) {
    std::vector<sf::Vector3f> result;
    result.reserve(vertices.size());

    for(int i = 0; i < vertices.size(); ++i) {
        auto tmp = sf::Vector4f(vertices[i].x, vertices[i].y, vertices[i].z, 1);
        auto v4 = tmp * m;
        result.emplace_back(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);
    }

    return result;
}

void Object::transform(std::vector<sf::Vector3f> &vertices, const Matrix<4> &m) {
    for(int i = 0; i < vertices.size(); ++i) {
        auto tmp = sf::Vector4f(vertices[i].x, vertices[i].y, vertices[i].z, 1);
        auto v4 = tmp * m;
        vertices[i] = sf::Vector3f(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);
    }
}
std::vector<sf::Vector3f> Object::rotatedAroundY(const std::vector<sf::Vector3f> &vertices, float angle){
    std::vector<sf::Vector3f> result;
    result.reserve(vertices.size());

    float sina = sin(angle * M_PI / 180);
    float cosa = cos(angle * M_PI / 180);
    Matrix<4> m = Matrix<4>::identity();
    m(0, 0) = cosa;
    m(0, 2) = sina;
    m(2, 0) = -sina;
    m(2, 2) = cosa;

    for(auto vertex : vertices) {
        auto tmp = sf::Vector4f(vertex.x, vertex.y, vertex.z, 1);
        auto v4 = tmp * m;
        result.emplace_back(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);
    }

    return result;
}
std::vector<sf::Vector3f> Object::rotatedAroundX(const std::vector<sf::Vector3f>& vertices, float angle){
    std::vector<sf::Vector3f> result;
    result.reserve(vertices.size());

    float sina = sin(angle * M_PI / 180);
    float cosa = cos(angle * M_PI / 180);
    Matrix<4> m = Matrix<4>::identity();
    m(1, 1) = cosa;
    m(1, 2) = sina;
    m(2, 1) = -sina;
    m(2, 2) = cosa;

    for(int i = 0; i < vertices.size(); ++i) {
        auto tmp = sf::Vector4f(vertices[i].x, vertices[i].y, vertices[i].z, 1);
        auto v4 = tmp * m;
        result.emplace_back(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);
    }

    return result;
}

std::vector<sf::Vector3f> Object::rotatedAroundZ(const std::vector<sf::Vector3f>& vertices, float angle)
{
    std::vector<sf::Vector3f> result;
    result.reserve(vertices.size());

    float sina = sin(angle * M_PI / 180);
    float cosa = cos(angle * M_PI / 180);
    Matrix<4> m = Matrix<4>::identity();
    m(0, 0) = cosa;
    m(0, 1) = sina;
    m(1, 0) = -sina;
    m(1, 1) = cosa;

    for(int i = 0; i < vertices.size(); ++i) {
        auto tmp = sf::Vector4f(vertices[i].x, vertices[i].y, vertices[i].z, 1);
        auto v4 = tmp * m;
        result.emplace_back(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);
    }

    return result;
}

Object &Object::operator=(Object &&other) {
    resetMatrices();
    _vertices = std::move(other._vertices);
    _edges = std::move(other._edges);
    _polygons = std::move(other._polygons);
    moveBy(other.position);
    return *this;
}

std::vector<sf::Vector3f> Object::movedBy(const std::vector<sf::Vector3f>& vertices, sf::Vector3f delta) {
    std::vector<sf::Vector3f> result;
    result.reserve(vertices.size());

    for (const auto &point: vertices) {
        result.push_back(point + delta);
    }

    return result;
}
