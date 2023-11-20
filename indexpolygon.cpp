#include "indexpolygon.h"

IndexPolygon::IndexPolygon(const std::vector<int> &indices, const sf::Color& color)
    : _indices(indices), _color(color)
{ }

void IndexPolygon::calculateNormal(const Object *object) {
    auto& vertices = object->vertices();

    sf::Vector3f polygonCenter;
    for(auto idx: indices()) {
        polygonCenter += vertices[idx];
    }
    polygonCenter = {polygonCenter.x / indices().size(), polygonCenter.y / indices().size(), polygonCenter.z / indices().size()};
    _center = polygonCenter;
    auto directionToPolygon = normalize(polygonCenter);

    auto first = vertices[indices()[0]] - vertices[indices()[1]];
    auto second = vertices[indices()[2]] - vertices[indices()[1]];

    _normal = {first.y * second.z - first.z * second.y, first.z * second.x - first.x * second.z, first.x * second.y - first.y * second.x};
    _normal = normalize(_normal);
    auto angleCos = dot(directionToPolygon, _normal);
    if(angleCos < 0) {
        _normal = -_normal;
    }
}

void IndexPolygon::calculateNormal(const Object *object, const sf::Vector3f &directionToPolygon) {
    auto& vertices = object->vertices();

    sf::Vector3f polygonCenter;
    for(auto idx: indices()) {
        polygonCenter += vertices[idx];
    }
    polygonCenter = {polygonCenter.x / indices().size(), polygonCenter.y / indices().size(), polygonCenter.z / indices().size()};
    _center = polygonCenter;

    auto first = vertices[indices()[0]] - vertices[indices()[1]];
    auto second = vertices[indices()[2]] - vertices[indices()[1]];

    _normal = {first.y * second.z - first.z * second.y, first.z * second.x - first.x * second.z, first.x * second.y - first.y * second.x};
    _normal = normalize(_normal);
    auto angleCos = dot(directionToPolygon, _normal);
    if(angleCos < 0) {
        _normal = -_normal;
    }
}
