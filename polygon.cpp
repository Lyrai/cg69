#include "polygon.h"
#include "line.h"

Polygon::Polygon()
{ }

Polygon::Polygon(const std::vector<sf::Vector3f> &points): points(points)
{ }

void Polygon::addPoint(const sf::Vector3f &point)
{
    points.push_back(point);
}

void Polygon::applyTransformation(const Matrix<3> &m, const sf::Vector3f& v)
{
    for(int i = 0; i < points.size(); ++i) {
        points[i] = m * points[i] + v;
    }
}

void Polygon::moveBy(const sf::Vector3f &v)
{
    static constexpr float values[9] {1, 0, 0,
                                      0, 1, 0,
                                      0, 0, 1};
    static constexpr auto matrix = Matrix<3>(values);
    applyTransformation(matrix, v);
}

void Polygon::rotateAround(Line* line, float cosa, float sina)
{
    float values[9] {cosa, sina,
                    -sina, cosa};

    int p[2] = {1, 2};
    int a = p[0];
    int b = p[1];
    sf::Vector3f v(-a * cosa + b * sina + a, -a * sina - b * cosa + b, 0);
    applyTransformation(Matrix<3>(values), v);
}

void Polygon::scaleAround(const sf::Vector3f &p, float kx, float ky, float kz)
{
    float values[9] {kx, 0, 0,
                     0, ky, 0,
                     0, 0, kz};

    sf::Vector3f v((1 - kx) * p.x, (1 - ky) * p.y, (1 - kz) * p.z);
    applyTransformation(Matrix<3>(values), v);
}

sf::Vector3f Polygon::center()
{
    sf::Vector3f sum(0, 0, 0);
    auto size = points.size();
    for(int i = 0; i < size; ++i) {
        sum += points[i];
    }

    return {sum.x / size, sum.y / size, sum.z / size};
}

std::vector<sf::Vector3f> Polygon::movedBy(const sf::Vector3f &v) const {
    static constexpr float values[9] {1, 0, 0,
                                      0, 1, 0,
                                      0, 0, 1};
    static constexpr Matrix<3> matrix(values);

    std::vector<sf::Vector3f> result;
    result.reserve(points.size());

    for (const auto &point: points) {
        result.push_back(matrix * point + v);
    }

    return result;
}

std::vector<sf::Vector3f> Polygon::rotatedAround(Line *line, float cosa, float sina) const {
    return std::vector<sf::Vector3f>();
}

std::vector<sf::Vector3f> Polygon::scaledAround(const sf::Vector3f &p, float kx, float ky, float kz) const {
    return std::vector<sf::Vector3f>();
}

const std::vector<sf::Vector3f> &Polygon::getPoints() const {
    return points;
}
