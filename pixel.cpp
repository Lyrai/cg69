#include "pixel.h"

Pixel &Pixel::operator=(const std::array<sf::Uint8, 3>& arr) {
    buffer[0] = arr[0];
    buffer[1] = arr[1];
    buffer[2] = arr[2];

    return *this;
}

Pixel &Pixel::operator=(const sf::Color &color) {
    buffer[0] = color.r;
    buffer[1] = color.g;
    buffer[2] = color.b;
    buffer[3] = color.a;

    return *this;
}

sf::Color Pixel::color() {
    return {r(), g(), b()};
}
