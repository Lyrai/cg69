#ifndef CG2_PIXEL_H
#define CG2_PIXEL_H

#include <SFML/Graphics.hpp>

class Pixel {
    friend class Pixbuf;

private:
    sf::Uint8* buffer;
    explicit Pixel(sf::Uint8* buf): buffer(buf) { }

public:
    inline sf::Uint8 r() { return buffer[0]; }
    inline sf::Uint8 g() { return buffer[1]; }
    inline sf::Uint8 b() { return buffer[2]; }

    sf::Color color();

    Pixel& operator=(const std::array<sf::Uint8, 3>& arr);
    Pixel& operator=(const sf::Color& color);
};


#endif //CG2_PIXEL_H
