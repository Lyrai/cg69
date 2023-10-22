#include <cmath>
#include "transform.h"

Matrix<4> Transform::objectToWorldMatrix() const {
    return objectToWorld;
}

Matrix<4> Transform::worldToObjectMatrix() const {
    return worldToObject;
}

Transform::Transform(const sf::Vector3f &position)
    : position({0, 0, 0}), objectToWorld(Matrix<4>::identity()), worldToObject(Matrix<4>::identity())
{
    Transform::moveBy(position);
}

void Transform::moveBy(const sf::Vector3f &v) {
    transform(Matrix<3>::identity(), v);

    Matrix<4> m = Matrix<4>::identity();
    m(3, 0) = v.x;
    m(3, 1) = v.y;
    m(3, 2) = v.z;

    objectToWorld = objectToWorld * m;

    m(3, 0) *= -1;
    m(3, 1) *= -1;
    m(3, 2) *= -1;

    worldToObject = m * worldToObject;
}

std::vector<sf::Vector3f> Transform::transformed(const Matrix<3> &m, const sf::Vector3f &v) const {
    Matrix<4> m4 ({
        m(0, 0), m(0, 1), m(0, 2), 0,
        m(1, 0), m(1, 1), m(1, 2), 0,
        m(2, 0), m(2, 1), m(2, 2), 0,
        v.x, v.y, v.z, 1
    });

    return transformed(m4);
}

void Transform::rotateAround(Line *line, float cosa, float sina) {

}

void Transform::scaleAround(const sf::Vector3f &p, float kx, float ky, float kz) {

}

void Transform::transform(const Matrix<3> &m, const sf::Vector3f &v) {
    Matrix<4> m4 ({
        m(0, 0), m(0, 1), m(0, 2), 0,
        m(1, 0), m(1, 1), m(1, 2), 0,
        m(2, 0), m(2, 1), m(2, 2), 0,
        v.x, v.y, v.z, 1
    });

    transform(m4);
}

void Transform::transform(const Matrix<4> &m) {
    sf::Vector4f tmp(position.x, position.y, position.z, 1);
    auto v4 = tmp * m;
    position = sf::Vector3f(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);
}

void Transform::rotateAroundX(float angle) {
    sf::Vector4f tmp(position.x, position.y, position.z, 1);
    float sina = sin(angle * M_PI / 180);
    float cosa = cos(angle * M_PI / 180);

    Matrix<4> m = Matrix<4>::identity();
    m(1, 1) = cosa;
    m(1, 2) = sina;
    m(2, 1) = -sina;
    m(2, 2) = cosa;

    auto v4 = tmp * m;
    position = sf::Vector3f(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);

    objectToWorld = objectToWorld *  m;

    m(1, 2) = -sina;
    m(2, 1) = sina;

    worldToObject = m * worldToObject;
}

void Transform::rotateAroundY(float angle) {
    sf::Vector4f tmp(position.x, position.y, position.z, 1);
    float sina = sin(angle * M_PI / 180);
    float cosa = cos(angle * M_PI / 180);

    Matrix<4> m = Matrix<4>::identity();
    m(0, 0) = cosa;
    m(0, 2) = sina;
    m(2, 0) = -sina;
    m(2, 2) = cosa;

    auto v4 = tmp * m;
    position = sf::Vector3f(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);
    objectToWorld = objectToWorld *  m;

    m(0, 2) = -sina;
    m(2, 0) = sina;

    worldToObject = m * worldToObject;
}
