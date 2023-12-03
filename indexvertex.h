#ifndef CG6_INDEXVERTEX_H
#define CG6_INDEXVERTEX_H

#include <SFML/Graphics.hpp>

class IndexVertex {
public:
    IndexVertex(int index);
    IndexVertex(int index, const sf::Vector2f& texCoords);

    int index() const { return _index; }
    const sf::Vector2f& coords() const { return _coords; }

private:
    int _index;
    sf::Vector2f _coords;
    bool _hasTexture;
};


#endif //CG6_INDEXVERTEX_H
