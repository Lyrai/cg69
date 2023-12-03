#ifndef CG6_OBJECT_H
#define CG6_OBJECT_H

#include "polygon.h"
#include "transform.h"

class IndexPolygon;

using Edges = std::vector<std::pair<int, int>>;
using Polygons = std::vector<IndexPolygon>;

class Object: public Transform {
public:
    explicit Object(const sf::Vector3f& position, const std::vector<Vertex>& vertices, const Edges& indices);
    explicit Object(const sf::Vector3f& position, std::vector<Vertex>&& vertices, Edges&& indices);
    explicit Object(const sf::Vector3f& position, const std::vector<Vertex>& vertices, Polygons& polygons);
    explicit Object(const sf::Vector3f& position, const std::vector<Vertex>& vertices, Polygons& polygons, const sf::Vector3f& polygonFaceDirection);
    Object(const Object& other) = default;

    std::vector<Vertex> movedBy(const sf::Vector3f& v) const;
    std::vector<Vertex> rotatedAround(Line* line, float cosa, float sina) const;
    std::vector<Vertex> rotatedAroundX(float angle);
    std::vector<Vertex> rotatedAroundY(float angle);
    std::vector<Vertex> scaledAround(const sf::Vector3f& p, float kx, float ky, float kz) const;
    std::vector<Vertex> transformed(const Matrix<4>& m) const;
    static std::vector<Vertex> transformed(const std::vector<Vertex>& vertices, const Matrix<4> &m);
    static std::vector<sf::Vector3f> transformed(const std::vector<sf::Vector3f>& vertices, const Matrix<4> &m);
    static std::vector<sf::Vector3f> rotatedAroundY(const std::vector<sf::Vector3f>& vertices, float angle);
    static std::vector<sf::Vector3f> rotatedAroundX(const std::vector<sf::Vector3f>& vertices, float angle);
    static std::vector<sf::Vector3f> rotatedAroundZ(const std::vector<sf::Vector3f>& vertices, float angle);
    static std::vector<Vertex> movedBy(const std::vector<Vertex>& vertices, sf::Vector3f delta);


    static void transform(std::vector<Vertex>& vertices, const Matrix<4> &m);

    sf::Vector3f center() const;

    const std::vector<std::pair<int, int>>& edges() const { return _edges; }
    const std::vector<Vertex>& vertices() const { return _vertices; }
    std::vector<Vertex>& vertices() { return _vertices; }
    const Polygons& polygons() const { return _polygons; }

    Object& operator=(Object&& other);

    void setTexture(const sf::Image& texture);

private:
    std::vector<Vertex> _vertices;
    Edges _edges;
    Polygons _polygons;
};


#endif //CG6_OBJECT_H
