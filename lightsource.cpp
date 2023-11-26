#include "lightsource.h"

LightSource::LightSource(const sf::Vector3f &position)
    : Transform(position)
{ }

LightSource LightSource::transformed(const Matrix<4> &m) const {
    auto tmp = sf::Vector4f(position.x, position.y, position.z, 1);
    auto r = tmp * m;
    return LightSource({r.x / r.w, r.y / r.w, r.z / r.w});
}
