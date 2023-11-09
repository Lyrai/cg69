#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "camera.h"
#include "figures.h"
#include "gui.h"
#include "expressionparser.h"


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

//    Object cube = createIcosahedron();
//    Object cube = parseFigure("figures/octahedron.json");
    Object cube = createCube();
    objects.push_back(&cube);
    Object gizmos({0, 0, 0}, {{0, 0, 0},
                              {1, 0, 0},
                              {0, 1, 0},
                              {0, 0, 1}}, Edges({{0, 1},
                                                 {0, 2},
                                                 {0, 3}}));
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
    auto formulaInput = gui.get<tgui::EditBox>("formula");

    auto x0 = -7.f;
    auto x1 = 7.f;
    auto y0 = -7.f;
    auto y1 = 7.f;
    auto steps = 1000;

    //5 * (cos(x^2 + y^2 + 1) / (x^2 + y^2 + 1) + 0.1)
    //cos(x^2 + y^2) / (x^2 + y^2 + 1)


    formulaInput->onTextChange([=, &cube]() {
        if(formulaInput->getText().size() == 0) {
            return;
        }
        ExpressionParser parser;
        Expression expr = parser.parse(formulaInput->getText().toStdString());
        if(!expr.isValid()) {
            return;
        }
        auto ystep = (y1 - y0) / steps;
        auto xstep = (x1 - x0) / steps;

        std::vector<sf::Vector3f> points;
        for(int i = 0; i < steps; ++i) {
            auto y = y0 + ystep * i;
            for(int j = 0; j < steps; ++j) {
                auto x = x0 + xstep * j;
                points.emplace_back(x, -(float)expr.evaluate(x, y), y);
            }
        }

        Polygons polygons;
        for(int i = 0; i < steps - 1; ++i) {
            for(int j = 0; j < steps - 1; ++j) {
                polygons.emplace_back(std::vector<int> {steps * j + i, steps * j + i + 1, steps * (j + 1) + i + 1, steps * (j + 1) + i});
            }
        }

        cube = Object({0, 0, 0}, points, polygons);
    });

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
