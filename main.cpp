#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "camera.h"
#include "figures.h"

sf::Color background_color;


int main() {
    background_color = sf::Color::White;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
    tgui::Gui gui(window);
    auto canvas = tgui::CanvasSFML::create({"90%", "90%"});
    gui.add(canvas);
    canvas->setPosition({"10%", "10%"});
    auto layout = tgui::HorizontalLayout::create({"100%", "10%"});
    layout->insertSpace(1, 0.5);
    gui.add(layout);
    auto parallelButton = tgui::Button::create("Parallel");
    parallelButton->setTextSize(20);
    //parallelButton->setSize({"15%", "5%"});
    //parallelButton->setPosition({"0%", "0%"});

    auto perspectiveButton = tgui::Button::create("Perspective");
    //perspectiveButton->setSize({"15%", "5%"});
    perspectiveButton->setTextSize(20);
    //perspectiveButton->setPosition({"20%", "0%"});


    layout->add(parallelButton);
    layout->add(perspectiveButton);

    sf::Texture texture;
    //auto window_size = window.getSize();
    auto window_size = canvas->getSize();
    texture.create(window_size.x, window_size.y);

    Pixbuf pixbuf(sf::Vector2u(window_size.x, window_size.y));
    std::vector<Object *> objects;

    Object cube = createIcosahedron();
//    Object cube = createCube();
    objects.push_back(&cube);
    Object gizmos({0, 0, 0}, {{0, 0, 0},
                              {1, 0, 0},
                              {0, 1, 0},
                              {0, 0, 1}}, {{0, 1},
                                           {0, 2},
                                           {0, 3}});
    //objects.push_back(&gizmos);
    Camera cam({0, 0, -3}, &objects, sf::Vector2u(window_size.x, window_size.y));
    cam.setPixbuf(&pixbuf);
    cam.setProjection(Projection::Perspective);

    parallelButton->onClick([&cam]() { cam.setProjection(Projection::Parallel); });
    perspectiveButton->onClick([&cam]() { cam.setProjection(Projection::Perspective); });

    //texture.update(pixbuf.raw());
    sf::Vector3f vm(0, 0, 0);
    Line l({0, 0, 0}, {2, 2, 2});

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            gui.handleEvent(event);
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::Resized: {
                    auto canvasSize = canvas->getSize();
                    auto canvasSizeU = sf::Vector2u(canvasSize.x, canvasSize.y);
                    pixbuf.resize(canvasSizeU);
                    texture.create(canvasSizeU.x, canvasSizeU.y);
                    break;
                }
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::Key::A:
                            cube.rotateAroundY(-1);
                            break;
                        case sf::Keyboard::Key::D:
                            cube.rotateAroundY(1);
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
        canvas->clear(sf::Color::White);
        canvas->draw(sf::Sprite(texture));
        canvas->display();
        gui.draw();
        window.display();
    }
    return 0;
}
