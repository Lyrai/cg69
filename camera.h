#ifndef CG6_CAMERA_H
#define CG6_CAMERA_H

#include "object.h"
#include "pixbuf.h"

enum class Projection {
    Parallel,
    Perspective
};

class Camera: public sf::Drawable, public Transform {
public:
    explicit Camera(const sf::Vector3f& position, std::vector<Object*>* objects);
    void setProjection(Projection proj);
    void setPixbuf(Pixbuf* buf) { pixbuf = buf; }

protected:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

private:
    std::vector<sf::Vector3f> projectionTransform(const std::vector<sf::Vector3f>& obj) const;
    std::vector<sf::Vector2f> project(const std::vector<sf::Vector3f>& vertices) const;
    std::vector<sf::Vector2i> mapToScreen(const std::vector<sf::Vector2f>& projected) const;
    void draw(const std::vector<sf::Vector2i>& vertices, Object* obj) const;

private:
    Projection projection;
    Matrix<4> projectionTransformMatrix;
    Matrix<4> projectionMatrix;
    mutable Pixbuf* pixbuf;
    std::vector<Object*>* objects;
    sf::Vector3f topRight;
    float scale;

    static constexpr float perspective[16] {0, 0, 0, 0,
                                     0, 0, 0, 0,
                                     0, 0, 0, 0,
                                     0, 0, 0, 0};

    static constexpr float parallel[16] {0, 0, 0, 0,
                                  0, 0, 0, 0,
                                  0, 0, 0, 0,
                                  0, 0, 0, 0};
};


#endif //CG6_CAMERA_H
