#include <iostream>
#include <cmath>
#include "camera.h"
#include "algorithm.h"

Camera::Camera(const sf::Vector3f &position, std::vector<Object*>* objects, const sf::Vector2u& screenSize)
    : Transform(position), objects(objects), localPosition(sf::Vector3f(0, 0, -2)), topRight(2.5, 2.5, 0), bottomLeft(-2.5, -2.5, 0), screenSize(screenSize)
{ }

void Camera::render() const {
    pixbuf->fill(sf::Color::White);

    for(auto object: *objects) {
        //auto transformed = object->transformed(projectionTransformMatrix);
        //auto viewSpace = Object({}, transformed, {}).transformed(object->objectToWorldMatrix() * worldToObjectMatrix());
        //auto projected = project(viewSpace);
        auto viewSpace = object->transformed(object->objectToWorldMatrix() * worldToObjectMatrix());
        auto clipped = clip(viewSpace, object);
        auto transformed = projectionTransform(clipped->vertices());
        auto projected = project(transformed);
        auto mapped = mapToScreen(projected);
        draw(mapped, object);
    }
}

void Camera::draw(const std::vector<sf::Vector2i> &vertices, Object *obj) const {
    for(const auto edge: obj->edges()) {
        draw_line(*pixbuf, vertices[edge.first], vertices[edge.second], sf::Color::Black);
    }
}

void Camera::setProjection(Projection proj) {
    projection = proj;
    if(proj == Projection::Parallel) {
        /*auto phi = -45 * M_PI / 180;
        auto theta = 35.26 * M_PI / 180;
        Matrix<4> yRotate {{
            (float)cos(phi), 0, (float)-sin(phi),0,
            0,                  1, 0,                  0,
            (float)sin(phi), 0, (float)cos(phi), 0,
            0,                  0, 0,                  1
        }};
        Matrix<4> xRotate {{
            1, 0,                    0,                    0,
            0, (float)cos(theta), (float)sin(theta), 0,
            0, (float)-sin(theta),(float)cos(theta), 0,
            0, 0,                    0,                    1
        }};

        projectionTransformMatrix = yRotate * xRotate;*/
        //projectionTransformMatrix = parallel;
        projectionTransformMatrix = Matrix<4>::identity();
    } else {
        Matrix<4> m = Matrix<4>::identity();
        sf::Vector4f tmp(localPosition.x, localPosition.y, localPosition.z, 1);
        auto v4 = tmp * objectToWorldMatrix();
        m(2, 3) = -1 / (v4.z / v4.w);
        projectionTransformMatrix = m;
    }
}

std::vector<sf::Vector3f> Camera::projectionTransform(const std::vector<sf::Vector3f> &obj) const {
    return Object({0,0,0}, obj, {}).transformed(projectionTransformMatrix);
}

std::vector<sf::Vector2i> Camera::mapToScreen(const std::vector<sf::Vector2f> &projected) const {
    std::vector<sf::Vector2i> result;
    auto viewPlane = topRight - bottomLeft;
    auto kx = screenSize.x / viewPlane.x;
    auto ky = screenSize.y / viewPlane.y;
    for(const auto& vec: projected) {
        result.emplace_back((vec.x + viewPlane.x / 2) * kx, (vec.y + viewPlane.y / 2) * ky);
    }

    return result;
}

std::vector<sf::Vector3f> Camera::movedBy(const sf::Vector3f &v) const {
    return std::vector<sf::Vector3f>();
}

std::vector<sf::Vector3f> Camera::rotatedAround(Line *line, float cosa, float sina) const {
    return std::vector<sf::Vector3f>();
}

std::vector<sf::Vector3f> Camera::scaledAround(const sf::Vector3f &p, float kx, float ky, float kz) const {
    return std::vector<sf::Vector3f>();
}

std::vector<sf::Vector3f> Camera::transformed(const Matrix<4> &m) const {
    return std::vector<sf::Vector3f>();
}

std::vector<sf::Vector2f> Camera::project(const std::vector<sf::Vector3f> &vertices) const {
    auto projected = Object({0,0,0}, vertices, {}).transformed(projectionMatrix);
    std::vector<sf::Vector2f> result;
    result.reserve(projected.size());
    for(const auto vec: projected) {
        result.emplace_back(vec.x, vec.y);
    }

    return result;
}

std::vector<sf::Vector3f> Camera::rotatedAroundX(float angle) {
    return std::vector<sf::Vector3f>();
}

std::vector<sf::Vector3f> Camera::rotatedAroundY(float angle) {
    return std::vector<sf::Vector3f>();
}

Object *Camera::clip(const std::vector<sf::Vector3f> &transformedVertices, Object *obj) const {
    std::vector<std::pair<int, int>> edges;
    std::vector<sf::Vector3f> vertices = transformedVertices;
    bool clipped = false;
    for(const auto& edge: obj->edges()) {
        if(vertices[edge.first].z >= 0 && vertices[edge.second].z >= 0) {
            edges.push_back(edge);
            continue;
        }

        if(vertices[edge.first].z < 0 && vertices[edge.second].z < 0) {
            continue;
        }

        clipped = true;

        auto ca = vertices[edge.first] - sf::Vector3f(0, 0, 0);
        auto vcn = dot(ca, sf::Vector3f(0, 0, 1));
        auto cv = vertices[edge.second] - vertices[edge.first];
        auto vcm = dot(cv, sf::Vector3f(0, 0, 1));
        auto k = vcn / vcm;
        auto x = sf::Vector3f(cv.x * k, cv.y * k, cv.z * k);
        vertices.push_back(x);
        auto newEdge = edge;
        if(vertices[edge.first].z < 0) {
            newEdge.first = vertices.size();
        } else if(vertices[edge.second].z < 0) {
            newEdge.second = vertices.size();
        }

        edges.push_back(newEdge);
    }

    return new Object({0, 0, 0}, vertices, edges);
}
