#include <iostream>
#include <cmath>
#include "camera.h"
#include "algorithm.h"

Camera::Camera(const sf::Vector3f &position, std::vector<Object*>* objects, const sf::Vector2u& screenSize)
    : Transform(position), objects(objects), localPosition(sf::Vector3f(0, 0, -1)), topRight(5, 5, 0), screenSize(screenSize)
{ }

void Camera::render() const {
    pixbuf->fill(sf::Color::White);

    for(const auto object: *objects) {
        auto transformed = object->transformed(projectionTransformMatrix);
        auto viewSpace = Object({}, transformed, {}).transformed(object->objectToWorldMatrix() * worldToObjectMatrix());
        //auto viewSpace = object->transformed(object->objectToWorldMatrix() * worldToObjectMatrix());
        //auto transformed = projectionTransform(viewSpace);
        auto projected = project(viewSpace);
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
        auto phi = -45 * M_PI / 180;
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

        projectionTransformMatrix = yRotate * xRotate;
    } else {
        projectionTransformMatrix = Matrix<4>(perspective);
    }
}

std::vector<sf::Vector3f> Camera::projectionTransform(const std::vector<sf::Vector3f> &obj) const {
    return Object({0,0,0}, obj, {}).transformed(projectionTransformMatrix);
}

std::vector<sf::Vector2i> Camera::mapToScreen(const std::vector<sf::Vector2f> &projected) const {
    std::vector<sf::Vector2i> result;
    auto kx = screenSize.x / topRight.x;
    auto ky = screenSize.y / topRight.y;
    for(const auto& vec: projected) {
        result.emplace_back(vec.x * kx, vec.y * ky);
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
