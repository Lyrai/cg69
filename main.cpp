#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "camera.h"

sf::Color background_color;

int main() {
    background_color = sf::Color::White;
    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");

    sf::Texture texture;
    auto window_size = window.getSize();
    texture.create(window_size.x, window_size.y);

    Pixbuf pixbuf(window_size);
    std::vector<Object*> objects;
    std::vector<sf::Vector3f> cubeVertices {
            {0, 0, 0},
            {1, 0, 0},
            {1, 1, 0},
            {0, 1, 0},
            {0, 0, 1},
            {1, 0, 1},
            {1, 1, 1},
            {0, 1, 1},
    };
    std::vector<std::pair<int, int>> cubeEdges {
            {0, 1},
            {1, 2},
            {2, 3},
            {3, 0},
            {0, 4},
            {1, 5},
            {2, 6},
            {3, 7},
            {4, 5},
            {5, 6},
            {6, 7},
            {7, 4}
    };
    Object cube({1, 1, 1}, cubeVertices, cubeEdges);
    objects.push_back(&cube);
    Object gizmos({0, 0, 0}, {{0, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 0, 1}}, {{0, 1}, {0, 2}, {0, 3}});
    objects.push_back(&gizmos);
    Camera cam({-2.5, -2.5, 0}, &objects, window_size);
    cam.setPixbuf(&pixbuf);
    cam.setProjection(Projection::Parallel);

    //texture.update(pixbuf.raw());

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
        cam.render();
        texture.update(pixbuf.raw());
        window.draw(sf::Sprite(texture));
        window.display();
        /*for(auto object: objects) {
            object->moveBy({0.001, 0.001, 0});
        }*/

    }
    return 0;
}
