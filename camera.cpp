#include <iostream>
#include <cmath>
#include "camera.h"
#include "algorithm.h"

Camera::Camera(const sf::Vector3f &position, std::vector<Object*>* objects, const sf::Vector2u& screenSize)
    : Transform(position), objects(objects), localPosition(sf::Vector3f(0, 0, -3)), projectionPlaneSize(5, 5), screenSize(screenSize), viewAngle(90), zbuffer(new float[screenSize.x * screenSize.y])
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
    for(int i = 0; i < screenSize.y * screenSize.x; ++i) {
        zbuffer[i] = std::numeric_limits<float>::infinity();
        //zbuffer[i] = 0;
    }
    for(auto object: *objects) {
        for(auto& polygon: object->polygons()) {
            renderPolygon(polygon, object);
        }
        /*auto viewSpace = object->transformed(object->objectToWorldMatrix() * worldToObjectMatrix());
        Object clipped({}, {}, {});
        clip(viewSpace, object, clipped);

        auto transformed = projectionTransform(clipped.vertices());
        auto projected = project(transformed);
        auto mapped = mapToScreen(projected);
        draw(mapped, &clipped);*/
    }
}

void Camera::draw(const std::vector<sf::Vector2i> &vertices, Object *obj) const {
    for(const auto edge: obj->edges()) {
        draw_line(*pixbuf, vertices[edge.first], vertices[edge.second], sf::Color::Black);
    }
}

void Camera::setProjection(Projection proj) {
    if(proj == Projection::Parallel) {
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
        result.emplace_back((vec.x) * kx + screenSize.x / 2, (vec.y) * ky + screenSize.y / 2);
    }

    return result;
}
std::vector<sf::Vector3f> Camera::screenToMap(const std::vector<sf::Vector2i> &projected) const {
    std::vector<sf::Vector3f> result;
    auto kx = screenSize.x / projectionPlaneSize.x;
    auto ky = screenSize.y / projectionPlaneSize.y;
    for(const auto& vec: projected) {
        sf::Vector2i v(vec.x - screenSize.x / 2 - 50, vec.y - screenSize.y / 2 - 64);
        sf::Vector3f temp(v.x / kx, v.y / ky, 0);
        auto res = planeIntersection(localPosition, temp, -position.z);
        sf::Vector4f temp1(res.x, res.y, res.z, 1);
        auto res1 = temp1 * objectToWorldMatrix();
        result.emplace_back(res1.x / res1.w, res1.y / res1.w, res1.z / res1.w);
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

void Camera::resize(const sf::Vector2u &newSize) {
    screenSize = newSize;

    auto screenRatio = (float)screenSize.y / screenSize.x;
    projectionPlaneSize.y = projectionPlaneSize.x * screenRatio;
    delete[] zbuffer;
    zbuffer = new float[screenSize.x * screenSize.y];
}

void Camera::renderPolygon(const IndexPolygon &polygon, Object *obj) const {
    std::vector<sf::Vector3f> points { polygon.normal() + polygon.center(), polygon.center() };
    auto worldNormal = Object::transformed(points, obj->objectToWorldMatrix());
    auto normal = worldNormal[0] - worldNormal[1];
    if(normal.z >= 0) {
        return;
    }

    std::vector<sf::Vector3f> vertices;
    for(auto idx: polygon.indices()) {
        vertices.push_back(obj->vertices()[idx]);
    }

    auto viewSpace = Object::transformed(vertices, obj->objectToWorldMatrix() * worldToObjectMatrix());
    auto clipped = clipPolygon(viewSpace);

    if(clipped.empty()) {
        return;
    }

    if (clipped.size() <= 3) {

        auto transformed = projectionTransform(clipped);
        rasterize(transformed, polygon);
        return;
        //auto projected = project(transformed);
        //auto mapped = mapToScreen(projected);
        //drawPolygon(mapped);
    }
    std::vector<int> indices;
    for (int i = 0; i < clipped.size(); ++i) {
        indices.push_back(i);
    }

    IndexPolygon p(indices, polygon.color());
    auto triangles = triangulate(p);
    for (const auto &triangle: triangles) {
        vertices.clear();
        for (const auto &index: triangle.indices()) {
            vertices.push_back(clipped[index]);
        }

        auto transformed = projectionTransform(vertices);
        rasterize(transformed, triangle);
    }
}

std::vector<sf::Vector3f> Camera::clipPolygon(const std::vector<sf::Vector3f> &vertices) const {
    std::vector<sf::Vector3f> result;
    auto clippingPlane = 0.f;
    for(int i = 0; i < vertices.size(); ++i) {
        auto first = vertices[i];
        auto second = vertices[(i + 1) % vertices.size()];
        if(first.z >= clippingPlane && second.z >= clippingPlane) {
            result.push_back(first);
            continue;
        }

        if(first.z < clippingPlane && second.z < clippingPlane) {
            continue;
        }

        auto x = planeIntersection(first, second, clippingPlane);

        if(first.z < clippingPlane) {
            result.push_back(x);
        } else {
            result.push_back(first);
            result.push_back(x);
        }
    }

    return result;
}

void Camera::drawPolygon(const std::vector<sf::Vector2i> &vertices) const {
    for(int i = 0; i < vertices.size(); ++i) {
        draw_line(*pixbuf, vertices[i], vertices[(i + 1) % vertices.size()], sf::Color::Black);
    }
}

void Camera::rasterize(const std::vector<sf::Vector3f> &vertices, const IndexPolygon& polygon) const {
    auto v = vertices;
    if (v[0].y > v[1].y) {
        std::swap(v[0], v[1]);
    }
    if (v[0].y > v[2].y) {
        std::swap(v[0], v[2]);
    }
    if (v[1].y > v[2].y) {
        std::swap(v[1], v[2]);
    }

    float zinv[3] {
            v[0].z,
            v[1].z,
            v[2].z
    };

    auto screenSpace = mapToScreen(project(v));
    int height = screenSpace[2].y - screenSpace[0].y;
    if(height < 1e-5) {
        return;
    }

    for(int i = 0; i < 2; ++i) {
        int segmentHeight = screenSpace[1 + i].y - screenSpace[i].y;
        if(segmentHeight < 1e-5) {
            continue;
        }

        for(int y = screenSpace[i].y; y <= screenSpace[1 + i].y; ++y) {
            if(y < 0 || y >= screenSize.y) {
                continue;
            }

            float alpha = (float) (y - screenSpace[0].y) / height;
            float beta = (float) (y - screenSpace[i].y) / segmentHeight;

            float zinvA = zinv[0] + (zinv[2] - zinv[0]) * alpha;
            float zinvB = zinv[i] + (zinv[1 + i] - zinv[i]) * beta;

            int A = screenSpace[0].x + (screenSpace[2].x - screenSpace[0].x) * alpha;
            int B = screenSpace[i].x + (screenSpace[1 + i].x - screenSpace[i].x) * beta;

            if (A > B) {
                std::swap(A, B);
                std::swap(zinvA, zinvB);
            }

            for (int x = A; x <= B; ++x) {
                if(x < 0 || x >= screenSize.x) {
                    continue;
                }

                float gamma = (float) (x - A) / (B - A);
                float z = zinvA + (zinvB - zinvA) * gamma;
                auto bufferIdx = y * screenSize.x + x;
                if(zbuffer[bufferIdx] > z) {
                    zbuffer[bufferIdx] = z;
                    pixbuf->at(x, y) = polygon.color();
                }
            }
        }
    }
}

sf::Vector3f Camera::viewDirection() const {
    std::vector<sf::Vector3f> points {localPosition, {0, 0, 0}};
    points = Object::transformed(points, objectToWorldMatrix());

    return normalize(points[1] - points[0]);
}
