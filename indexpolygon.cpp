#include "indexpolygon.h"

IndexPolygon::IndexPolygon(const std::vector<int> &indices, const sf::Color& color)
    : _indices(indices), _color(color)
{ }

void IndexPolygon::calculateNormal(const Object *object) {
    auto& vertices = object->vertices();

    sf::Vector3f polygonCenter = center(object);
    auto directionToPolygon = normalize(polygonCenter - object->center());

    auto first = vertices[indices()[0]].toVec3() - vertices[indices()[1]].toVec3();
    auto second = vertices[indices()[2]].toVec3() - vertices[indices()[1]].toVec3();

    _normal = {first.y * second.z - first.z * second.y, first.z * second.x - first.x * second.z, first.x * second.y - first.y * second.x};
    _normal = normalize(_normal);
    auto angleCos = dot(directionToPolygon, _normal);
    if(angleCos < 0) {
        _normal = -_normal;
    }
}

void IndexPolygon::calculateNormal(const Object *object, const sf::Vector3f &directionToPolygon) {
    auto& vertices = object->vertices();

    center(object);

    auto first = vertices[indices()[0]].toVec3() - vertices[indices()[1]].toVec3();
    auto second = vertices[indices()[2]].toVec3() - vertices[indices()[1]].toVec3();

    _normal = {first.y * second.z - first.z * second.y, first.z * second.x - first.x * second.z, first.x * second.y - first.y * second.x};
    _normal = normalize(_normal);
    auto angleCos = dot(directionToPolygon, _normal);
    if(angleCos < 0) {
        _normal = -_normal;
    }
}

const sf::Vector3f &IndexPolygon::center(const Object* object) {
    auto& vertices = object->vertices();

    sf::Vector3f polygonCenter;
    for(auto idx: indices()) {
        polygonCenter += vertices[idx].toVec3();
    }

    polygonCenter = {polygonCenter.x / indices().size(), polygonCenter.y / indices().size(), polygonCenter.z / indices().size()};
    _center = polygonCenter;
    return _center;
}
