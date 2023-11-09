#include <cmath>
#include "transform.h"

Matrix<4> Transform::objectToWorldMatrix() const {
    return objectToWorld;
}

Matrix<4> Transform::worldToObjectMatrix() const {
    return worldToObject;
}

Transform::Transform(const sf::Vector3f &position)
        : position({0, 0, 0}), objectToWorld(Matrix<4>::identity()), worldToObject(Matrix<4>::identity()) {
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
    Matrix<4> m4({
                         m(0, 0), m(0, 1), m(0, 2), 0,
                         m(1, 0), m(1, 1), m(1, 2), 0,
                         m(2, 0), m(2, 1), m(2, 2), 0,
                         v.x, v.y, v.z, 1
                 });

    return transformed(m4);
}

void Transform::scaleAround(const sf::Vector3f &p, float kx, float ky, float kz) {
    sf::Vector4f tmp(position.x, position.y, position.z, 1);
    sf::Vector3f v(kx, ky, kz);
    Matrix<4> m_toZero = Matrix<4>::identity();
    Matrix<4> m_scale = Matrix<4>::identity();
    Matrix<4> m_backPos = Matrix<4>::identity();
    m_toZero(3, 0) = p.x;
    m_toZero(3, 1) = p.y;
    m_toZero(3, 2) = p.z;

    m_scale(0, 0) = v.x;
    m_scale(1, 1) = v.y;
    m_scale(2, 2) = v.z;

    m_backPos(3, 0) = -p.x;
    m_backPos(3, 1) = -p.y;
    m_backPos(3, 2) = -p.z;

    auto mh = m_toZero * m_scale * m_backPos;
    auto v4 = tmp * mh;

    position = sf::Vector3f(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);

    objectToWorld = objectToWorld * mh;

    m_toZero(3, 0) = -p.x;
    m_toZero(3, 1) = -p.y;
    m_toZero(3, 2) = -p.z;

    m_scale(0, 0) = 1 / v.x;
    m_scale(1, 1) = 1 / v.y;
    m_scale(2, 2) = 1 / v.z;

    m_backPos(3, 0) = p.x;
    m_backPos(3, 1) = p.y;
    m_backPos(3, 2) = p.z;

    auto mht = m_backPos * m_scale * m_toZero;

    worldToObject = mht * worldToObject;

}

void Transform::transform(const Matrix<3> &m, const sf::Vector3f &v) {
    Matrix<4> m4({
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
    position = sf::Vector3f(v4.x / v4.w,
                            v4.y / v4.w,
                            v4.z / v4.w);
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

    objectToWorld = objectToWorld * m;

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
    objectToWorld = objectToWorld * m;

    m(0, 2) = -sina;
    m(2, 0) = sina;

    worldToObject = m * worldToObject;
}


void Transform::rotateAroundZ(float angle) {
    sf::Vector4f tmp(position.x, position.y, position.z, 1);
    float sina = sin(angle * M_PI / 180);
    float cosa = cos(angle * M_PI / 180);

    Matrix<4> m = Matrix<4>::identity();
    m(0, 0) = cosa;
    m(0, 1) = sina;
    m(1, 0) = -sina;
    m(1, 1) = cosa;

    auto v4 = tmp * m;
    position = sf::Vector3f(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);
    objectToWorld = objectToWorld * m;

    m(0, 1) = -sina;
    m(1, 0) = sina;

    worldToObject = m * worldToObject;
}

void Transform::rotateAroundLine(float angle, Axis axis) {
    auto tmp = position;
    moveBy(-tmp);
    switch (axis) {
        case Axis::X:
            rotateAroundX(angle);
            break;
        case Axis::Y:
            rotateAroundY(angle);
            break;
        case Axis::Z:
            rotateAroundZ(angle);
            break;
    }
    moveBy(tmp);
}

void Transform::mirrorAroundAxis(Axis axis) {
//    auto tmp = position;
//    moveBy(-tmp);
    switch (axis) {
        case Axis::X:
            scaleAround({0, position.y, position.z}, -1, 1, 1);
            break;
        case Axis::Y:
            scaleAround({position.x, 0, position.z}, 1, -1, 1);
            break;
        case Axis::Z:
            scaleAround({position.x, position.y, 0}, 1, 1, -1);
            break;
    }
//    moveBy(tmp);
}


void Transform::rotateAround(Line *line, float angle) {

    auto tmp = line->getPoints()[0];
    moveBy(-tmp);
    auto LineVec = line->getPoints()[1] - line->getPoints()[0];

    LineVec = normalize(LineVec);

    if (LineVec.y == 0 && LineVec.z == 0) {
        rotateAroundX(angle);
        moveBy(tmp);
        return;
    }

    auto d = sqrt(LineVec.y * LineVec.y + LineVec.z * LineVec.z);

    rotateAroundX(asin(LineVec.y / d) * 180 / M_PI);
    rotateAroundY(asin(LineVec.x) * 180 / M_PI);
    rotateAroundZ(angle);
    rotateAroundY(-asin(LineVec.x) * 180 / M_PI);
    rotateAroundX(-asin(LineVec.y / d) * 180 / M_PI);

    moveBy(tmp);
}

std::vector<sf::Vector3f> Transform::transformed(const std::vector<sf::Vector3f>& vertices, const Matrix<4> &m) {
    std::vector<sf::Vector3f> result;
    result.reserve(vertices.size());

    for(int i = 0; i < vertices.size(); ++i) {
        auto tmp = sf::Vector4f(vertices[i].x, vertices[i].y, vertices[i].z, 1);
        auto v4 = tmp * m;
        result.emplace_back(v4.x / v4.w, v4.y / v4.w, v4.z / v4.w);
    }

    return result;
}