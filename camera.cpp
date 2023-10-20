#include "camera.h"
#include "algorithm.h"

Camera::Camera(const sf::Vector3f &position, std::vector<Object*>* objects): Transform(position), objects(objects)
{ }

void Camera::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    sf::Texture tex;
    pixbuf->fill(sf::Color::White);
    for(const auto object: *objects) {
        auto viewSpace = object->transformed(object->objectToWorldMatrix() * worldToObjectMatrix());
        auto transformed = projectionTransform(viewSpace);
        auto projected = project(transformed);
        auto mapped = mapToScreen(projected);
        draw(mapped, object);
    }
    tex.update(pixbuf->raw());
    target.draw(sf::Sprite(tex));
}

void Camera::draw(const std::vector<sf::Vector2i> &vertices, Object *obj) const {
    for(const auto edge: obj->edges()) {
        draw_line(*pixbuf, vertices[edge.first], vertices[edge.second], sf::Color::Black);
    }
}

void Camera::setProjection(Projection proj) {
    projection = proj;
    if(proj == Projection::Parallel) {
        projectionTransformMatrix = Matrix<4>(parallel);
    } else {
        projectionTransformMatrix = Matrix<4>(perspective);
    }
}
