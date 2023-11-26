#include <iostream>
#include <cmath>
#include <thread>
#include <cassert>
#include "camera.h"
#include "algorithm.h"

Camera::Camera(const sf::Vector3f &position, std::vector<Object*>* objects, std::vector<LightSource*>* lightSources, const sf::Vector2u& screenSize)
    : Transform(position), objects(objects), localPosition(sf::Vector3f(0, 0, -8)), screenSize(screenSize), viewAngle(90), zbuffer(new float[screenSize.x * screenSize.y]), zTest(true), lightSources(lightSources)
{
    auto obj = Object({}, {{0, 0, -localPosition.z}}, Edges());
    auto vertices = obj.rotatedAroundY(viewAngle / 2);
    auto projectionPlane = planeIntersection(localPosition, vertices[0].toVec3(), 0);
    projectionPlaneSize.x = std::abs(projectionPlane.x * 2);
    auto screenRatio = (float)screenSize.y / screenSize.x;
    projectionPlaneSize.y = projectionPlaneSize.x * screenRatio;
}

void Camera::render() const {
    pixbuf->fill(sf::Color::White);
    for(int i = 0; i < screenSize.y * screenSize.x; ++i) {
        zbuffer[i] = std::numeric_limits<float>::infinity();
    }

    std::vector<LightSource> lightSourcesViewSpace;
    lightSourcesViewSpace.reserve(lightSources->size());

    for(auto source: *lightSources) {
        lightSourcesViewSpace.push_back(source->transformed(worldToObjectMatrix()));
    }

    for(auto object: *objects) {
        for(auto& polygon: object->polygons()) {
            renderPolygon(polygon, object, lightSourcesViewSpace);
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
    projection = proj;
    if(proj == Projection::Parallel) {
        projectionTransformMatrix = Matrix<4>::identity();
    } else {
        Matrix<4> m = Matrix<4>::identity();
        m(2, 3) = -1 / localPosition.z;
        projectionTransformMatrix = m;
    }
}

std::vector<Vertex> Camera::projectionTransform(const std::vector<Vertex> &obj) const {
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

std::vector<sf::Vector2f> Camera::project(const std::vector<Vertex> &vertices) const {
    auto projected = Object::transformed(vertices, projectionMatrix);
    std::vector<sf::Vector2f> result;
    result.reserve(projected.size());
    for(const auto vec: projected) {
        result.emplace_back(vec.x, vec.y);
    }

    return result;
}

void Camera::clip(const std::vector<Vertex> &transformedVertices, Object *obj, Object& result) const {
    std::vector<std::pair<int, int>> edges;
    edges.reserve(obj->edges().size());
    std::vector<Vertex> vertices = transformedVertices;
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

        auto x = planeIntersection(first.toVec3(), second.toVec3(), clippingPlane);

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

void Camera::renderPolygon(const IndexPolygon &polygon, Object *obj, const std::vector<LightSource>& lightSources) const {
    std::vector<sf::Vector3f> points { polygon.normal() + polygon.center(), polygon.center() };
    auto worldNormal = Object::transformed(points, obj->objectToWorldMatrix() * worldToObjectMatrix());
    auto normal = worldNormal[0] - worldNormal[1];
    auto viewVector = projection == Projection::Perspective ? normalize(localPosition - worldNormal[1]) : normalize(localPosition);
    if(dot(normal, viewVector) < 0) {
        return;
    }

    std::vector<Vertex> vertices;
    for(auto idx: polygon.indices()) {
        vertices.push_back(obj->vertices()[idx]);
    }

    auto viewSpace = Object::transformed(vertices, obj->objectToWorldMatrix() * worldToObjectMatrix());
    //auto clipped = clipPolygon(viewSpace);

    /*if(clipped.empty()) {
        return;
    }*/

    auto transformed = projectionTransform(viewSpace);

    if(!zTest) {
        auto projected = project(transformed);
        auto mapped = mapToScreen(projected);
        drawPolygon(mapped);
        return;
    }

    std::vector<sf::Vector3f> normals;
    normals.reserve(vertices.size());
    for(int i = 0; i < vertices.size(); ++i) {
        auto& vertex = vertices[i];
        std::vector<Vertex> points {vertex.normal() + vertex.toVec3(), vertex.toVec3()};
        auto v = Object::transformed(points, obj->objectToWorldMatrix() * worldToObjectMatrix());

        normals.push_back(normalize(v[0].toVec3() - v[1].toVec3()));
    }
    rasterize(viewSpace, polygon, lightSources, normals);

    /*std::vector<int> indices;
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
    }*/
}

std::vector<Vertex> Camera::clipPolygon(const std::vector<Vertex> &vertices) const {
    std::vector<Vertex> result;
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

        auto x = planeIntersection(first.toVec3(), second.toVec3(), clippingPlane);

        if(first.z < clippingPlane) {
            Vertex n(x);
            n.setColor(first.color());
            result.push_back(n);
        } else {
            result.push_back(first);
            Vertex n(x);
            n.setColor(second.color());
            result.push_back(n);
        }
    }

    return result;
}

void Camera::drawPolygon(const std::vector<sf::Vector2i> &vertices) const {
    for(int i = 0; i < vertices.size(); ++i) {
        draw_line(*pixbuf, vertices[i], vertices[(i + 1) % vertices.size()], sf::Color::Black);
    }
}

void Camera::rasterize(std::vector<Vertex> &v, const IndexPolygon& polygon, const std::vector<LightSource>& lightSources, std::vector<sf::Vector3f>& normals) const {
    auto transformed = projectionTransform(v);
    if (transformed[0].y > transformed[1].y) {
        std::swap(transformed[0], transformed[1]);
        std::swap(v[0], v[1]);
        std::swap(normals[0], normals[1]);
    }
    if (transformed[0].y > transformed[2].y) {
        std::swap(transformed[0], transformed[2]);
        std::swap(v[0], v[2]);
        std::swap(normals[0], normals[2]);
    }
    if (transformed[1].y > transformed[2].y) {
        std::swap(transformed[1], transformed[2]);
        std::swap(v[1], v[2]);
        std::swap(normals[1], normals[2]);
    }

    float zinv[3] {
            transformed[0].z,
            transformed[1].z,
            transformed[2].z
    };

    sf::Color colors[3] {
        transformed[0].color(),
        transformed[1].color(),
        transformed[2].color()
    };

    float lightness[3] {
        0,
        0,
        0
    };

    for(int i = 0; i < 3; ++i) {
        for (auto &source: lightSources) {
            auto direction = normalize(source.getPosition() - v[i].toVec3());
            lightness[i] += std::max(0.0f, dot(normals[i], direction));
        }
    }

    for (float& lightnes : lightness) {
        lightnes = std::clamp(lightnes, 0.f, 1.f);
    }

    auto screenSpace = mapToScreen(project(transformed));
    int height = screenSpace[2].y - screenSpace[0].y;
    if(height == 0) {
        return;
    }

    /*auto rasterizeHalf = [&](int i) {
        int segmentHeight = screenSpace[1 + i].y - screenSpace[i].y + 1;
        //if(segmentHeight < 1e-5) {
        //    return;
        //}

        auto start = std::clamp(screenSpace[i].y, 0, (int)screenSize.y - 1);
        auto end = std::clamp(screenSpace[1 + i].y, 0, (int)screenSize.y - 1);
        for(int y = start; y <= end; ++y) {
            float alpha = (float) (y - screenSpace[0].y) / height;
            float beta = (float) (y - screenSpace[i].y) / segmentHeight;

            float zinvA = zinv[0] + (zinv[2] - zinv[0]) * alpha;
            float zinvB = zinv[i] + (zinv[1 + i] - zinv[i]) * beta;

            sf::Color colorA (colors[0].r + (colors[2].r - colors[0].r) * alpha, colors[0].g + (colors[2].g - colors[0].g) * alpha, colors[0].b + (colors[2].b - colors[0].b) * alpha);
            sf::Color colorB (colors[i].r + (colors[1 + i].r - colors[i].r) * beta, colors[i].g + (colors[1 + i].g - colors[i].g) * beta, colors[i].b + (colors[1 + i].b - colors[i].b) * beta);

            int A = screenSpace[0].x + (screenSpace[2].x - screenSpace[0].x) * alpha;
            int B = screenSpace[i].x + (screenSpace[1 + i].x - screenSpace[i].x) * beta;

            if (A > B) {
                std::swap(A, B);
                std::swap(zinvA, zinvB);
                std::swap(colorA, colorB);
            }

            auto startX = std::clamp(A, 0, (int)screenSize.x - 1);
            auto endX = std::clamp(B, 0, (int)screenSize.x - 1);
            for (int x = startX; x <= endX; ++x) {
                float gamma = (float) (x - A) / (B - A);
                float z = zinvA + (zinvB - zinvA) * gamma;
                sf::Color color (colorA.r + (colorB.r - colorA.r) * gamma, colorA.g + (colorB.g - colorA.g) * gamma, colorA.b + (colorB.b - colorA.b) * gamma);

                auto bufferIdx = y * screenSize.x + x;
                if(zbuffer[bufferIdx] > z) {
                    zbuffer[bufferIdx] = z;
                    pixbuf->at(x, y) = color;
                }
            }
        }
    };*/

    for(int i = 0; i < 2; ++i) {
        int segmentHeight = screenSpace[1 + i].y - screenSpace[i].y;
        if(segmentHeight == 0) {
            continue;
        }

        auto start = std::clamp(screenSpace[i].y, 0, (int)screenSize.y - 1);
        auto end = std::clamp(screenSpace[1 + i].y, 0, (int)screenSize.y - 1);
        for(int y = start; y <= end; ++y) {
            float alpha = (float) (y - screenSpace[0].y) / height;
            float beta = (float) (y - screenSpace[i].y) / segmentHeight;

            float zinvA = zinv[0] + (zinv[2] - zinv[0]) * alpha;
            float zinvB = zinv[i] + (zinv[1 + i] - zinv[i]) * beta;

            sf::Color colorA (colors[0].r + (colors[2].r - colors[0].r) * alpha, colors[0].g + (colors[2].g - colors[0].g) * alpha, colors[0].b + (colors[2].b - colors[0].b) * alpha);
            sf::Color colorB (colors[i].r + (colors[1 + i].r - colors[i].r) * beta, colors[i].g + (colors[1 + i].g - colors[i].g) * beta, colors[i].b + (colors[1 + i].b - colors[i].b) * beta);

            float lightnessA = lightness[0] + (lightness[2] - lightness[0]) * alpha;
            float lightnessB = lightness[i] + (lightness[1 + i] - lightness[i]) * beta;

            int A = screenSpace[0].x + (screenSpace[2].x - screenSpace[0].x) * alpha;
            int B = screenSpace[i].x + (screenSpace[1 + i].x - screenSpace[i].x) * beta;

            if (A > B) {
                std::swap(A, B);
                std::swap(zinvA, zinvB);
                std::swap(colorA, colorB);
                std::swap(lightnessA, lightnessB);
            }

            auto startX = std::clamp(A, 0, (int)screenSize.x - 1);
            auto endX = std::clamp(B, 0, (int)screenSize.x - 1);
            for (int x = startX; x <= endX; ++x) {
                float gamma = (float) (x - A) / (B - A);
                float z = zinvA + (zinvB - zinvA) * gamma;
                if(z < 0) {
                    continue;
                }
                sf::Color color (colorA.r + (colorB.r - colorA.r) * gamma, colorA.g + (colorB.g - colorA.g) * gamma, colorA.b + (colorB.b - colorA.b) * gamma);
                float lightness = lightnessA + (lightnessB - lightnessA) * gamma;

                auto bufferIdx = y * screenSize.x + x;
                if(zbuffer[bufferIdx] > z) {
                    zbuffer[bufferIdx] = z;
                    pixbuf->at(x, y) = sf::Color(color.r * lightness, color.g * lightness, color.b * lightness);
                    //pixbuf->at(x, y) = color;
                }
            }
        }
    }

    //std::thread upper(rasterizeHalf, 0);
    //std::thread lower(rasterizeHalf, 1);
    //rasterizeHalf(0);

    //upper.join();
    //lower.join();
}

sf::Vector3f Camera::viewDirection() const {
    std::vector<sf::Vector3f> points {localPosition, {0, 0, 0}};
    points = Object::transformed(points, objectToWorldMatrix());

    return normalize(points[1] - points[0]);
}

void Camera::rotateAroundLine(float angle, Axis axis) {
    auto tmp = localPosition;
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
