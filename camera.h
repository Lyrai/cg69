#ifndef CG6_CAMERA_H
#define CG6_CAMERA_H

#include "object.h"
#include "indexpolygon.h"
#include "pixbuf.h"
#include "lightsource.h"

enum class Projection {
    Parallel,
    Perspective
};

class Camera: public Transform {
public:
    explicit Camera(const sf::Vector3f& position, std::vector<Object*>* objects, std::vector<LightSource*>* lightSources, const sf::Vector2u& screenSize);
    void setProjection(Projection proj);
    void setPixbuf(Pixbuf* buf) { pixbuf = buf; }

    void resize(const sf::Vector2u& newSize);

    void rotateAroundLine(float angle, Axis axis) override;
    std::vector<sf::Vector3f> screenToMap(const std::vector<sf::Vector2i> &projected) const;
    void render() const;
    sf::Vector3f viewDirection() const;
    bool getZTest() const { return zTest; }
    void setZTest(bool enabled) { zTest = enabled; }

private:
    std::vector<Vertex> projectionTransform(const std::vector<Vertex>& obj) const;
    std::vector<sf::Vector2f> project(const std::vector<Vertex>& vertices) const;
    std::vector<sf::Vector2i> mapToScreen(const std::vector<sf::Vector2f>& projected) const;
    void clip(const std::vector<Vertex>& vertices, Object* obj, Object& result) const;
    std::vector<Vertex> clipPolygon(const std::vector<Vertex> &vertices) const;
    void draw(const std::vector<sf::Vector2i>& vertices, Object* obj) const;
    void drawPolygon(const std::vector<sf::Vector2i>& vertices) const;
    void renderPolygon(const IndexPolygon& polygon, Object* obj, const std::vector<LightSource>& lightSources) const;
    void rasterize(std::vector<Vertex>& vertices, const IndexPolygon& polygon, const std::vector<LightSource>& lightSources, std::vector<sf::Vector3f>& normals) const;
    void renderLightSource(const LightSource& source) const;

private:
    Projection projection;
    Matrix<4> projectionTransformMatrix;
    mutable Pixbuf* pixbuf;
    float* zbuffer;
    std::vector<Object*>* objects;
    sf::Vector2f projectionPlaneSize;
    sf::Vector3f localPosition;
    sf::Vector2u screenSize;
    mutable sf::Texture texture;
    float viewAngle;
    bool zTest;
    std::vector<LightSource*>* lightSources;

    static constexpr float projectionMatrix[16] {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 0, 0,
            0, 0, 0, 1
    };

    static constexpr float perspective[16] {0, 0, 0, 0,
                                     0, 0, 0, 0,
                                     0, 0, 0, 0,
                                     0, 0, 0, 0};

    static constexpr float parallel[16] {0.7071, -0.408194688, 0.577375434,  0,
                                         0,       0.81654,     0.57728,      0,
                                        -0.7071, -0.408194688, 0.577375434, 0,
                                        0,      0,           0,           1};
};


#endif //CG6_CAMERA_H
