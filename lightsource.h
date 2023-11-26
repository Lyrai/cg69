#ifndef CG6_LIGHTSOURCE_H
#define CG6_LIGHTSOURCE_H

#include "transform.h"

class LightSource: public Transform {
public:
    LightSource(const sf::Vector3f& position);

    LightSource transformed(const Matrix<4>& m) const;
};


#endif //CG6_LIGHTSOURCE_H
