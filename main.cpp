#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "camera.h"
#include "figures.h"
#include "gui.h"


sf::Color background_color;

int main() {
    background_color = sf::Color::White;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
    sf::Texture texture;
    auto window_size = window.getSize();
    tgui::Gui gui(window);
    Mode mode;

    texture.create(window_size.x, window_size.y);

    Pixbuf pixbuf(sf::Vector2u(window_size.x, window_size.y));
    std::vector<Object *> objects;

    Object cube = createIcosahedron();
//  Object cube = createCube();
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
    //texture.update(pixbuf.raw());
    sf::Vector3f vm(0, 0, 0);
    Line l({0, 0, 0}, {2, 2, 2});
    setupGui(gui, cube, cam, mode);

    auto canvas = gui.get<tgui::CanvasSFML>("canvas");
    auto x1Input = gui.get<tgui::EditBox>("x1");
    auto y1Input = gui.get<tgui::EditBox>("y1");
    auto z1Input = gui.get<tgui::EditBox>("z1");
    auto x2Input = gui.get<tgui::EditBox>("x2");
    auto y2Input = gui.get<tgui::EditBox>("y2");
    auto z2Input = gui.get<tgui::EditBox>("z2");

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
                    cam.resize(canvasSizeU);
                    texture.create(canvasSizeU.x, canvasSizeU.y);
                    break;
                }
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::Key::A:
                            switch(mode) {
                                case Mode::Axis: {
                                    cube.rotateAroundY(-1);
                                    break;
                                }
                                case Mode::Center: {
                                    cube.rotateAroundLine(-1, Axis::Y);
                                    break;
                                }
                                case Mode::Line: {
                                    Line l(sf::Vector3f(x1Input->getText().toFloat(), y1Input->getText().toFloat(),
                                                        z1Input->getText().toFloat()),
                                           sf::Vector3f(x2Input->getText().toFloat(), y2Input->getText().toFloat(),
                                                        z2Input->getText().toFloat()));
                                    cube.rotateAround(&l, -1);
                                    break;
                                }
                                case Mode::Scale: {
                                    cube.scaleAround(cube.center(), 0.98, 0.98, 0.98);
                                    break;
                                }
                                case Mode::Mirror: {
                                    cube.mirrorAroundAxis(Axis::Y);
                                    break;
                                }
                                case Mode::Move: {
                                    cube.moveBy(sf::Vector3f(-0.1, 0, 0));
                                    break;
                                }
                            }
                            break;
                        case sf::Keyboard::Key::D:
                            switch(mode) {
                                case Mode::Axis: {
                                    cube.rotateAroundY(1);
                                    break;
                                }
                                case Mode::Center: {
                                    cube.rotateAroundLine(1, Axis::Y);
                                    break;
                                }
                                case Mode::Line: {
                                    Line l(sf::Vector3f(x1Input->getText().toFloat(), y1Input->getText().toFloat(),
                                                        z1Input->getText().toFloat()),
                                           sf::Vector3f(x2Input->getText().toFloat(), y2Input->getText().toFloat(),
                                                        z2Input->getText().toFloat()));
                                    cube.rotateAround(&l, 1);
                                    break;
                                }
                                case Mode::Scale: {
                                    cube.scaleAround(cube.center(), 1.02, 1.02, 1.02);
                                    break;
                                }
                                case Mode::Move: {
                                    cube.moveBy(sf::Vector3f(0.1, 0, 0));
                                    break;
                                }
                            }
                            break;
                        case sf::Keyboard::Key::W:
                            switch(mode) {
                                case Mode::Axis: {
                                    cube.rotateAroundX(-1);
                                    break;
                                }
                                case Mode::Center: {
                                    cube.rotateAroundLine(-1, Axis::X);
                                    break;
                                }
                                case Mode::Move: {
                                    cube.moveBy(sf::Vector3f(0, -0.1, 0));
                                    break;
                                }
                            }
                            break;
                        case sf::Keyboard::Key::S:
                            switch(mode) {
                                case Mode::Axis: {
                                    cube.rotateAroundX(1);
                                    break;
                                }
                                case Mode::Center: {
                                    cube.rotateAroundLine(1, Axis::X);
                                    break;
                                }
                                case Mode::Move: {
                                    cube.moveBy(sf::Vector3f(0, 0.1, 0));
                                    break;
                                }
                                case Mode::Mirror: {
                                    cube.mirrorAroundAxis(Axis::X);
                                    break;
                                }
                            }
                            break;
                        case sf::Keyboard::Key::E:
                            switch(mode) {
                                case Mode::Axis: {
                                    cube.rotateAroundZ(1);
                                    break;
                                }
                                case Mode::Center: {
                                    cube.rotateAroundLine(1, Axis::Z);
                                    break;
                                }
                                case Mode::Move: {
                                    cube.moveBy(sf::Vector3f(0, 0, -0.1));
                                    break;
                                }
                            }
                            break;
                        case sf::Keyboard::Key::Q:
                            switch(mode) {
                                case Mode::Axis: {
                                    cube.rotateAroundZ(-1);
                                    break;
                                }
                                case Mode::Center: {
                                    cube.rotateAroundLine(-1, Axis::Z);
                                    break;
                                }
                                case Mode::Move: {
                                    cube.moveBy(sf::Vector3f(0, 0, 0.1));
                                    break;
                                }
                                case Mode::Mirror: {
                                    cube.mirrorAroundAxis(Axis::Z);
                                    break;
                                }
                            }
                            break;
                        case sf::Keyboard::Key::Space:
                            switch (mode) {
                                case Mode::Axis:{
                                    cube.rotateAroundX(x1Input->getText().toFloat(0));
                                    cube.rotateAroundY(y1Input->getText().toFloat(0));
                                    cube.rotateAroundZ(z1Input->getText().toFloat(0));
                                    break;
                                }
                                case Mode::Center:{
                                    cube.rotateAroundLine(x1Input->getText().toFloat(0),Axis::X);
                                    cube.rotateAroundLine(y1Input->getText().toFloat(0),Axis::Y);
                                    cube.rotateAroundLine(z1Input->getText().toFloat(0),Axis::Z);
                                    break;
                                }
                            }
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
