#ifndef CG6_TRANSFORM_H
#define CG6_TRANSFORM_H

#include <SFML/Graphics.hpp>
#include "line.h"
#include "vertex.h"

enum class Axis {
    X,
    Y,
    Z
};

class Transform {
public:
    explicit Transform(const sf::Vector3f &position);
    virtual void moveBy(const sf::Vector3f &v);
    virtual void rotateAround(Line *line, float angle);
    virtual void rotateAroundX(float angle);
    virtual void rotateAroundY(float angle);
    virtual void rotateAroundZ(float angle);
    virtual void rotateAroundLine(float angle, Axis axis);
    virtual void scaleAround(const sf::Vector3f &p, float kx, float ky, float kz);
    virtual void mirrorAroundAxis(Axis axis);
    virtual void transform(const Matrix<3> &m, const sf::Vector3f &v);
    virtual void transform(const Matrix<4> &m);

    Matrix<4> objectToWorldMatrix() const;
    Matrix<4> worldToObjectMatrix() const;
    sf::Vector3f toWorldCoordinates(const sf::Vector3f& vec) const;
    sf::Vector3f toObjectCoordinates(const sf::Vector3f& vec) const;
    sf::Vector3f getPosition() const { return position; }

protected:
    sf::Vector3f position;
    void resetMatrices();

private:
    mutable Matrix<4> objectToWorld;
    mutable Matrix<4> worldToObject;
};



#endif //CG6_TRANSFORM_H
