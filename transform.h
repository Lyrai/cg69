#ifndef CG6_TRANSFORM_H
#define CG6_TRANSFORM_H

#include <SFML/Graphics.hpp>
#include "line.h"

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
    virtual std::vector<sf::Vector3f> movedBy(const sf::Vector3f &v) const = 0;
    virtual std::vector<sf::Vector3f> rotatedAround(Line *line, float cosa, float sina) const = 0;
    virtual std::vector<sf::Vector3f> rotatedAroundX(float angle) = 0;
    virtual std::vector<sf::Vector3f> rotatedAroundY(float angle) = 0;
    virtual std::vector<sf::Vector3f> scaledAround(const sf::Vector3f &p, float kx, float ky, float kz) const = 0;
    virtual std::vector<sf::Vector3f> transformed(const Matrix<3> &m, const sf::Vector3f &v) const;
    virtual std::vector<sf::Vector3f> transformed(const Matrix<4> &m) const = 0;

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
