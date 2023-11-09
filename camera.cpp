#include <iostream>
#include <cmath>
#include "camera.h"
#include "algorithm.h"

Camera::Camera(const sf::Vector3f &position, std::vector<Object*>* objects, const sf::Vector2u& screenSize)
    : Transform(position), objects(objects), localPosition(sf::Vector3f(0, 0, -3)), projectionPlaneSize(5, 5), screenSize(screenSize), viewAngle(120)
{
    auto obj = Object({}, {{0, 0, -localPosition.z}}, Edges());
    auto vertices = obj.rotatedAroundY(viewAngle / 2);
    auto projectionPlane = planeIntersection(localPosition, vertices[0], 0);
    projectionPlaneSize.x = std::abs(projectionPlane.x * 2);
    auto screenRatio = (float)screenSize.y / screenSize.x;
    projectionPlaneSize.y = projectionPlaneSize.x * screenRatio;
}

void Camera::render() const {
    pixbuf->fill(sf::Color::White);

    for(auto object: *objects) {
        auto viewSpace = object->transformed(object->objectToWorldMatrix() * worldToObjectMatrix());
        Object clipped({}, {}, {});
        clip(viewSpace, object, clipped);

        auto transformed = projectionTransform(clipped.vertices());
        auto projected = project(transformed);
        auto mapped = mapToScreen(projected);
        draw(mapped, &clipped);
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
    return Object::transformed(obj, projectionTransformMatrix);
}

std::vector<sf::Vector2i> Camera::mapToScreen(const std::vector<sf::Vector2f> &projected) const {
    std::vector<sf::Vector2i> result;
    auto kx = screenSize.x / projectionPlaneSize.x;
    auto ky = screenSize.y / projectionPlaneSize.y;
    for(const auto& vec: projected) {
        result.emplace_back((vec.x + projectionPlaneSize.x / 2) * kx, (vec.y + projectionPlaneSize.y / 2) * ky);
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
    auto projected = Object::transformed(vertices, projectionMatrix);
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

void Camera::clip(const std::vector<sf::Vector3f> &transformedVertices, Object *obj, Object& result) const {
    std::vector<std::pair<int, int>> edges;
    edges.reserve(obj->edges().size());
    std::vector<sf::Vector3f> vertices = transformedVertices;
    auto clippingPlane = 0.f;
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

        auto x = planeIntersection(first, second, clippingPlane);

        auto newEdge = edge;
        if(first.z < clippingPlane) {
            newEdge.first = vertices.size();
        } else {
            newEdge.second = vertices.size();
        }
        vertices.push_back(x);

        edges.push_back(newEdge);
    }

    result = std::move(Object({0, 0, 0}, std::move(vertices), std::move(edges)));
}

sf::Vector3f Camera::planeIntersection(const sf::Vector3f &begin, const sf::Vector3f &end, float z) const {
    auto first = begin;
    auto second = end;

    auto normal = sf::Vector3f(0, 0, 1);
    auto ca = sf::Vector3f(1, 1, z) - first;
    auto vcn = dot(ca, normal);
    auto cv = second - first;
    auto vcm = dot(cv, normal);
    auto k = vcn / vcm;

    auto x = sf::Vector3f(cv.x * k, cv.y * k, cv.z * k);
    x += first;

    return x;
}

void Camera::resize(const sf::Vector2u &newSize) {
    screenSize = newSize;

    auto screenRatio = (float)screenSize.y / screenSize.x;
    projectionPlaneSize.y = projectionPlaneSize.x * screenRatio;
}
