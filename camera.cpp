#include <iostream>
#include <cmath>
#include "camera.h"
#include "algorithm.h"

Camera::Camera(const sf::Vector3f &position, std::vector<Object*>* objects, const sf::Vector2u& screenSize)
    : Transform(position), objects(objects), localPosition(sf::Vector3f(0, 0, -8)), topRight(2.5, 2.5, 0), bottomLeft(-2.5, -2.5, 0), screenSize(screenSize)
{ }

void Camera::render() const {
    pixbuf->fill(sf::Color::White);

    for(auto object: *objects) {
        auto viewSpace = object->transformed(object->objectToWorldMatrix() * worldToObjectMatrix());
        std::vector<sf::Vector3f> transformed;
        Object* clipped = nullptr;
        if(projection == Projection::Perspective) {
            clipped = clip(viewSpace, object);
            transformed = projectionTransform(clipped->vertices());
        } else {
            transformed = projectionTransform(viewSpace);
        }

        auto projected = project(transformed);
        auto mapped = mapToScreen(projected);
        draw(mapped, projection == Projection::Perspective ? clipped : object);
        delete clipped;

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
        m(2, 3) = -1 / localPosition.z;
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
    auto clippingPlane = localPosition.z + 3;
    for(const auto& edge: obj->edges()) {
        auto first = vertices[edge.first];
        auto second = vertices[edge.second];
        if(first.z >= clippingPlane && second.z >= clippingPlane) {
            edges.push_back(edge);
            continue;
        }

        if(first.z < clippingPlane && second.z < clippingPlane) {
            continue;
        }

        if(second.z < first.z) {
            std::swap(first, second);
        }

        auto normal = sf::Vector3f(0, 0, 1);
        auto ca = sf::Vector3f(1, 1, clippingPlane) - first;
        auto vcn = dot(ca, normal);
        auto cv = second - first;
        auto vcm = dot(cv, normal);
        auto k = vcn / vcm;
        if(k < 0) {
            continue;
        } else if(k > 1) {
            edges.push_back(edge);
            continue;
        }
        auto x = sf::Vector3f(cv.x * k, cv.y * k, cv.z * k);
        x += first;
        auto newEdge = edge;
        if(vertices[edge.first].z < clippingPlane) {
            newEdge.first = vertices.size();
        } else if(vertices[edge.second].z < clippingPlane) {
            newEdge.second = vertices.size();
        }
        vertices.push_back(x);

        edges.push_back(newEdge);
    }

    return new Object({0, 0, 0}, vertices, edges);
}
