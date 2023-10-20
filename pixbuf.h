#ifndef CG2_PIXBUF_H
#define CG2_PIXBUF_H

#include <SFML/Graphics.hpp>
#include "pixel.h"

class Pixbuf {
private:
    sf::Uint8* buffer;
    unsigned int linear_size;
    bool own_memory;

public:
    sf::Vector2u size;

public:
    Pixel operator[](const sf::Vector2i& pos);

    Pixel at(int x, int y);

    explicit Pixbuf(const sf::Vector2u& size);
    explicit Pixbuf(const sf::Vector2u& size, sf::Uint8* buf);

    void fill(const sf::Color& color);
    void resize(const sf::Vector2u& new_size);
    inline sf::Uint8* raw() { return buffer; }
    ~Pixbuf();
};


#endif //CG2_PIXBUF_H
