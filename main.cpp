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

    Object cube = createDodecahedron();
//    Object cube = createCube();
    objects.push_back(&cube);
    Object gizmos({0, 0, 0}, {{0, 0, 0},
                              {1, 0, 0},
                              {0, 1, 0},
                              {0, 0, 1}}, {{0, 1},
                                           {0, 2},
                                           {0, 3}});
    //objects.push_back(&gizmos);
    Camera cam({0, 0, 0}, &objects, window_size);
    cam.setPixbuf(&pixbuf);
    cam.setProjection(Projection::Perspective);

    //texture.update(pixbuf.raw());
    sf::Vector3f vm(0, 0, 0);
    Line l({0, 0, 0}, {2, 2, 2});

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
                            cube.rotateAroundX(-1);
                            break;
                        case sf::Keyboard::Key::S:
                            cube.rotateAroundX(1);
                            break;
                        case sf::Keyboard::Key::Z:
                            cube.rotateAroundZ(-1);
                            break;
                        case sf::Keyboard::Key::X:
                            cube.rotateAroundZ(1);
                            break;
                        case sf::Keyboard::Key::C:
                            cube.rotateAround(&l, 1);
                            break;
                        case sf::Keyboard::Key::B:
                            cube.rotateAroundLine(1, Axis::X);
                            break;
                        case sf::Keyboard::Key::N:
                            cube.rotateAroundLine(1, Axis::Y);
                            break;
                        case sf::Keyboard::Key::M:
                            cube.rotateAroundLine(1, Axis::Z);
                            break;
                        case sf::Keyboard::Key::G:
                            cube.mirrorAroundAxis(Axis::X);
                            break;
                        case sf::Keyboard::Key::H:
                            cube.mirrorAroundAxis(Axis::Y);
                            break;
                        case sf::Keyboard::Key::J:
                            cube.mirrorAroundAxis(Axis::Z);
                            break;
                        case sf::Keyboard::Key::E:
                            cube.scaleAround(vm, 0.9, 0.9, 0.9);
                            break;
                        case sf::Keyboard::Key::Q:
                            cube.scaleAround(vm, 1.1, 1.1, 1.1);
                            break;
                        case sf::Keyboard::Key::Up:
                            cube.moveBy(sf::Vector3f(0, 0.1, 0));
                            break;
                        case sf::Keyboard::Key::Down:
                            cube.moveBy(sf::Vector3f(0, -0.1, 0));
                            break;
                        case sf::Keyboard::Key::Left:
                            cube.moveBy(sf::Vector3f(0.1, 0, 0));
                            break;
                        case sf::Keyboard::Key::Right:
                            cube.moveBy(sf::Vector3f(-0.1, 0, 0));
                            break;
                        case sf::Keyboard::Key::K:
                            cube.moveBy(sf::Vector3f(0, 0, -0.1));
                            break;
                        case sf::Keyboard::Key::L:
                            cube.moveBy(sf::Vector3f(0, 0, 0.1));
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
    }
    return 0;
}
