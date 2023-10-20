#include "algorithm.h"

void draw_line(Pixbuf& pixbuf, sf::Vector2i begin, sf::Vector2i end, const sf::Color& border_color) {
    auto d = end - begin;
    auto steep = abs(d.y) > abs(d.x);

    if(steep) {
        std::swap(begin.x, begin.y);
        std::swap(end.x, end.y);
    }

    if(begin.x > end.x) {
        std::swap(begin, end);
    }

    d = end - begin;

    auto y_step = d.y / abs(d.y);
    auto y = begin.y;

    d.y = abs(d.y);
    int di = 2 * d.y - d.x;

    for(int x = begin.x; x <= end.x; ++x) {
        pixbuf.at(steep ? y : x, steep ? x : y) = border_color;

        di += 2 * d.y - ((di >= 0) * 2 * d.x);
        y += (di >= 0) * y_step;
    }
}