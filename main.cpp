#include <iostream>
#include <SFML/Graphics.hpp>
#include "pixbuf.h"

sf::Color background_color;

int main() {
    background_color = sf::Color::White;
    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");

    sf::Texture texture;
    auto window_size = window.getSize();
    texture.create(window_size.x, window_size.y);

    Pixbuf pixbuf(window_size);
    pixbuf.fill(background_color);

    texture.update(pixbuf.raw());

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            }
        }

        window.clear();
        window.draw(sf::Sprite(texture));
        window.display();
    }
    return 0;
}
