#include <cstring>
#include "pixbuf.h"

Pixel Pixbuf::operator[](const sf::Vector2i &pos) {
    return Pixel(buffer + (pos.y * 4 * size.x + pos.x * 4));
}

Pixbuf::Pixbuf(const sf::Vector2u &size)
    : size(size), buffer(new sf::Uint8[size.x * size.y * 4]), linear_size(size.x * size.y * 4), own_memory(true)
{ }

Pixbuf::Pixbuf(const sf::Vector2u &size, sf::Uint8 *buf)
    : size(size), buffer(buf), linear_size(size.x * size.y * 4), own_memory(false)
{ }

void Pixbuf::fill(const sf::Color &color) {
    for(int i = 0; i < linear_size; i += 4) {
        buffer[i] = color.r;
        buffer[i + 1] = color.g;
        buffer[i + 2] = color.b;
        buffer[i + 3] = color.a;
    }
}

Pixel Pixbuf::at(int x, int y) {
    return Pixel(buffer + (y * 4 * size.x + x * 4));
}

extern sf::Color background_color;

void Pixbuf::resize(const sf::Vector2u &new_size) {
    Pixbuf tmp(new_size, new sf::Uint8[new_size.x * new_size.y * 4]);
    tmp.fill(background_color);
    auto x_size = std::min(size.x, new_size.x);
    auto y_size = std::min(size.y, new_size.y);

    for(int y = 0; y < y_size; ++y) {
        for(int x = 0; x < x_size; ++x) {
            tmp.at(x, y) = at(x, y).color();
        }
    }

    if(own_memory) {
        delete[] buffer;
        buffer = tmp.buffer;
    } else {
        memcpy(buffer, tmp.buffer, tmp.linear_size);
        delete[] tmp.buffer;
    }

    size = new_size;
    linear_size = tmp.linear_size;
}

Pixbuf::~Pixbuf() {
    if(own_memory) {
        delete[] buffer;
    }
}
