#ifndef CG6_OBJECT_H
#define CG6_OBJECT_H

#include "polygon.h"
#include "transform.h"

class IndexPolygon;

using Edges = std::vector<std::pair<int, int>>;
using Polygons = std::vector<IndexPolygon>;

class Object: public Transform {
public:
    explicit Object(const sf::Vector3f& position, const std::vector<sf::Vector3f>& vertices, const Edges& indices);
    explicit Object(const sf::Vector3f& position, std::vector<sf::Vector3f>&& vertices, Edges&& indices);
    explicit Object(const sf::Vector3f& position, const std::vector<sf::Vector3f>& vertices, Polygons& polygons, bool closedSurface = true);
    Object(const Object& other) = default;

    std::vector<sf::Vector3f> movedBy(const sf::Vector3f& v) const override;
    std::vector<sf::Vector3f> rotatedAround(Line* line, float cosa, float sina) const override;
    std::vector<sf::Vector3f> rotatedAroundX(float angle) override;
    std::vector<sf::Vector3f> rotatedAroundY(float angle) override;
    std::vector<sf::Vector3f> scaledAround(const sf::Vector3f& p, float kx, float ky, float kz) const override;
    std::vector<sf::Vector3f> transformed(const Matrix<4>& m) const override;
    static std::vector<sf::Vector3f> transformed(const std::vector<sf::Vector3f>& vertices, const Matrix<4> &m);
    static std::vector<sf::Vector3f> rotatedAroundY(const std::vector<sf::Vector3f>& vertices, float angle);
    static std::vector<sf::Vector3f> rotatedAroundX(const std::vector<sf::Vector3f>& vertices, float angle);
    static std::vector<sf::Vector3f> rotatedAroundZ(const std::vector<sf::Vector3f>& vertices, float angle);
    static std::vector<sf::Vector3f> movedBy(const std::vector<sf::Vector3f>& vertices, sf::Vector3f delta);

    static void transform(std::vector<sf::Vector3f>& vertices, const Matrix<4> &m);

    sf::Vector3f center() const;

    const std::vector<std::pair<int, int>>& edges() const { return _edges; }
    const std::vector<sf::Vector3f>& vertices() const { return _vertices; }
    const Polygons& polygons() const { return _polygons; }

    Object& operator=(Object&& other);

private:
    std::vector<sf::Vector3f> _vertices;
    Edges _edges;
    Polygons _polygons;
};


#endif //CG6_OBJECT_H
