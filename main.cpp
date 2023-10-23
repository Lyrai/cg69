#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include "camera.h"
#include "figures.h"

sf::Color background_color;


int main() {
    background_color = sf::Color::White;
    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");

    sf::Texture texture;
    auto window_size = window.getSize();
    texture.create(window_size.x, window_size.y);

    Pixbuf pixbuf(window_size);
    std::vector<Object *> objects;

    Object cube = createIcosahedron();
    objects.push_back(&cube);
    cube.rotateAroundY(45);
    cube.moveBy({1, 0, 2});
    Object gizmos({0, 0, 0}, {{0, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 0, 1}}, {{0, 1}, {0, 2}, {0, 3}});
    //objects.push_back(&gizmos);
    Camera cam({0, 0, 5}, &objects, window_size);
    cam.setPixbuf(&pixbuf);
    cam.setProjection(Projection::Parallel);

    //texture.update(pixbuf.raw());
    sf::Vector3f vm(3, 3, 3);
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::Key::A:
                            cam.rotateAroundY(-1);
                            break;
                        case sf::Keyboard::Key::D:
                            cam.rotateAroundY(1);
                            break;
                        case sf::Keyboard::Key::W:
                            cam.rotateAroundX(-1);
                            break;
                        case sf::Keyboard::Key::S:
                            cam.rotateAroundX(1);
                            break;
                        case sf::Keyboard::Key::E:
                            cam.scaleAround(vm, 0.1, 0.1, 0.1);
                            break;
                        case sf::Keyboard::Key::Q:
                            cam.scaleAround(vm, 0.9, 0.9, 0.9);
                            break;
                        case sf::Keyboard::Key::Up:
                            cam.moveBy(sf::Vector3f(0, 0.1, 0));
                            break;
                        case sf::Keyboard::Key::Down:
                            cam.moveBy(sf::Vector3f(0, -0.1, 0));
                            break;
                        case sf::Keyboard::Key::Left:
                            cam.moveBy(sf::Vector3f(0.1, 0, 0));
                            break;
                        case sf::Keyboard::Key::Right:
                            cam.moveBy(sf::Vector3f(-0.1, 0, 0));
                            break;
                        case sf::Keyboard::Key::K:
                            cam.moveBy(sf::Vector3f(0, 0, -0.1));
                            break;
                        case sf::Keyboard::Key::L:
                            cam.moveBy(sf::Vector3f(0, 0, 0.1));
                            break;
                    }
                    break;
            }
        }

        window.clear();
        cam.render();
        texture.update(pixbuf.raw());
        window.draw(sf::Sprite(texture));
        window.display();
        //cube.moveBy({0, 0, -0.001});

    }
    return 0;
}
