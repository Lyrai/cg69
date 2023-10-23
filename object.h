#ifndef CG6_OBJECT_H
#define CG6_OBJECT_H

#include "polygon.h"
#include "transform.h"

class Object: public Transform {
public:
    explicit Object(const sf::Vector3f& position, const std::vector<sf::Vector3f>& vertices, const std::vector<std::pair<int, int>>& indices);

    std::vector<sf::Vector3f> movedBy(const sf::Vector3f& v) const override;
    std::vector<sf::Vector3f> rotatedAround(Line* line, float cosa, float sina) const override;
    std::vector<sf::Vector3f> rotatedAroundX(float angle) override;
    std::vector<sf::Vector3f> rotatedAroundY(float angle) override;
    std::vector<sf::Vector3f> scaledAround(const sf::Vector3f& p, float kx, float ky, float kz) const override;
    std::vector<sf::Vector3f> transformed(const Matrix<4>& m) const override;

    sf::Vector3f center() const;

    const std::vector<std::pair<int, int>>& edges() const { return _edges; }
    const std::vector<sf::Vector3f>& vertices() const { return _vertices; }

private:
    std::vector<sf::Vector3f> _vertices;
    std::vector<std::pair<int, int>> _edges;
};


#endif //CG6_OBJECT_H
