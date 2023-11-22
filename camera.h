#ifndef CG6_CAMERA_H
#define CG6_CAMERA_H

#include "object.h"
#include "indexpolygon.h"
#include "pixbuf.h"

enum class Projection {
    Parallel,
    Perspective
};

class Camera: public Transform {
public:
    explicit Camera(const sf::Vector3f& position, std::vector<Object*>* objects, const sf::Vector2u& screenSize);
    void setProjection(Projection proj);
    void setPixbuf(Pixbuf* buf) { pixbuf = buf; }

    void resize(const sf::Vector2u& newSize);

    std::vector<sf::Vector3f> movedBy(const sf::Vector3f &v) const override;
    std::vector<sf::Vector3f> rotatedAround(Line *line, float cosa, float sina) const override;
    std::vector<sf::Vector3f> scaledAround(const sf::Vector3f &p, float kx, float ky, float kz) const override;
    std::vector<sf::Vector3f> rotatedAroundX(float angle) override;
    std::vector<sf::Vector3f> rotatedAroundY(float angle) override;
    std::vector<sf::Vector3f> transformed(const Matrix<4> &m) const override;
    void rotateAroundLine(float angle, Axis axis) override;
    std::vector<sf::Vector3f> screenToMap(const std::vector<sf::Vector2i> &projected) const;
    void render() const;
    sf::Vector3f viewDirection() const;
    bool getZTest() const { return zTest; }
    void setZTest(bool enabled) { zTest = enabled; }

private:
    std::vector<sf::Vector3f> projectionTransform(const std::vector<sf::Vector3f>& obj) const;
    std::vector<sf::Vector2f> project(const std::vector<sf::Vector3f>& vertices) const;
    std::vector<sf::Vector2i> mapToScreen(const std::vector<sf::Vector2f>& projected) const;
    void clip(const std::vector<sf::Vector3f>& vertices, Object* obj, Object& result) const;
    std::vector<sf::Vector3f> clipPolygon(const std::vector<sf::Vector3f> &vertices) const;
    void draw(const std::vector<sf::Vector2i>& vertices, Object* obj) const;
    void drawPolygon(const std::vector<sf::Vector2i>& vertices) const;
    void renderPolygon(const IndexPolygon& polygon, Object* obj) const;
    void rasterize(const std::vector<sf::Vector3f>& vertices, const IndexPolygon& polygon) const;

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
