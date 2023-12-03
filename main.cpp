#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <chrono>
#include "camera.h"
#include "figures.h"
#include "gui.h"
#include "expressionparser.h"
#include "algorithm.h"



sf::Color background_color;
using namespace std::chrono;

int main() {
    background_color = sf::Color::White;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
    sf::Texture texture;
    auto window_size = window.getSize();
    tgui::Gui gui(window);
    Mode mode;
    Axis axis;
    sf::Texture texture_1;
    sf::Texture texture_2;
    texture_1.loadFromFile("images.png");
    texture_2.loadFromFile("rolik.png");
    texture.create(window_size.x, window_size.y);

    Pixbuf pixbuf(sf::Vector2u(window_size.x, window_size.y));
    std::vector<Object *> objects;
    std::vector<LightSource*> lightSources;
    //auto icosahedron = parseFigure("figures/icosahedron_poly.json");
    //icosahedron.moveBy({1, 1, 7});
    auto roomPlanes = constructRoomPlanes();
    for (auto & roomPlane : roomPlanes) {
        objects.push_back(&roomPlane);
    }
    //objects.push_back(&icosahedron);

    Object cube = parseFigure("figures/cube_poly.json");
    sf::Image tex1Image = texture_2.copyToImage();
    cube.setTexture(tex1Image);
    //Polygons polygons {std::vector<int> {0, 1, 2, 3}};
    //Object cube = Object({0, 0, 0}, {Vertex(0, 0, 0, sf::Color::Green), Vertex(1, 0, 0, sf::Color::White), Vertex(1, 1, 0, sf::Color::Blue), Vertex(0, 1, 0, sf::Color::Red)}, polygons);
    objects.push_back(&cube);
    LightSource light({0, -2, 0});
    lightSources.push_back(&light);
    LightSource light1({0, 2, 0});
    lightSources.push_back(&light1);
    Object gizmos({0, 0, 0}, {{0, 0, 0},
                              {1, 0, 0},
                              {0, 1, 0},
                              {0, 0, 1}}, Edges({{0, 1},
                                                 {0, 2},
                                                 {0, 3}}));
    //objects.push_back(&gizmos);
    std::vector<sf::Vector2i> pointsFig;
    Camera cam({0, 0, -3}, &objects, &lightSources, sf::Vector2u(window_size.x, window_size.y));
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
    auto rotatingFigures = gui.get<tgui::Button>("rotFig");
    auto drawFigures = gui.get<tgui::Button>("draw");


    auto x0 = -7.f;
    auto x1 = 7.f;
    auto y0 = -7.f;
    auto y1 = 7.f;
    auto steps = 100;

    auto graphX0 = gui.get<tgui::EditBox>("graphX0");
    auto graphX1 = gui.get<tgui::EditBox>("graphX1");
    auto graphY0 = gui.get<tgui::EditBox>("graphY0");
    auto graphY1 = gui.get<tgui::EditBox>("graphY1");
    auto graphSteps = gui.get<tgui::EditBox>("graphSteps");
    auto rotSteps = gui.get<tgui::EditBox>("stepsRot");
    auto rotAxis = gui.get<tgui::EditBox>("axisRot");



    graphX0->setText(tgui::String(x0));
    graphX1->setText(tgui::String(x1));
    graphY0->setText(tgui::String(y0));
    graphY1->setText(tgui::String(y1));
    graphSteps->setText(tgui::String(steps));

    auto drawGraph = [&]() {
        if (formulaInput->getText().empty()) {
            return;
        }
        ExpressionParser parser;
        Expression expr = parser.parse(formulaInput->getText().toStdString());
        if (!expr.isValid()) {
            return;
        }
        auto ystep = (y1 - y0) / steps;
        auto xstep = (x1 - x0) / steps;

        std::vector<Vertex> points;
        for (int i = 0; i < steps; ++i) {
            auto y = y0 + ystep * i;
            for (int j = 0; j < steps; ++j) {
                auto x = x0 + xstep * j;
                points.emplace_back(x, -(float) expr.evaluate(x, y), y);
            }
        }

        Polygons polygons;
        for (int i = 0; i < steps - 1; ++i) {
            for (int j = 0; j < steps - 1; ++j) {
                polygons.emplace_back(std::vector<IndexVertex> {steps * j + i, steps * j + i + 1, steps * (j + 1) + i + 1,
                                                       steps * (j + 1) + i});
            }
        }

        cube = Object({0, 0, 0}, points, polygons, {0, -1, 0});
    };

    graphX0->onTextChange([&]() { x0 = graphX0->getText().toFloat(); drawGraph(); });
    graphX1->onTextChange([&]() { x1 = graphX1->getText().toFloat(); drawGraph(); });
    graphY0->onTextChange([&]() { y0 = graphY0->getText().toFloat(); drawGraph(); });
    graphY1->onTextChange([&]() { y1 = graphY1->getText().toFloat(); drawGraph(); });
    graphSteps->onTextChange([&]() { steps = graphSteps->getText().toInt(); drawGraph(); });

    auto loadButton = gui.get<tgui::Button>("load");
    auto saveButton = gui.get<tgui::Button>("save");

    loadButton->onClick([&]() {
        auto fileDialog = tgui::FileDialog::create();
        gui.add(fileDialog);
        fileDialog->setPath("figures/");
        fileDialog->onFileSelect([=, &cube]() {
            cube = parseFigure(fileDialog->getSelectedPaths()[0].asNativeString());
        });
    });

    saveButton->onClick([&]() {
        auto fileDialog = tgui::FileDialog::create();
        gui.add(fileDialog);
        fileDialog->setFileMustExist(false);
        fileDialog->setPath("figures/");
        fileDialog->onFileSelect([=, &cube]() {
            saveFigure(cube, fileDialog->getSelectedPaths()[0].asNativeString());
        });
    });

    //5 * (cos(x^2 + y^2 + 1) / (x^2 + y^2 + 1) + 0.1)
    //cos(x^2 + y^2) / (x^2 + y^2 + 1)


    formulaInput->onTextChange(drawGraph);// 0 1 2 3 4 5 6 7

    drawFigures->onClick([=,&cube,&pointsFig,&cam,&axis](){
        if(!pointsFig.empty()) {
            std::vector<sf::Vector3f> vertices;
            auto worldSpace = cam.screenToMap(pointsFig);
            for (const auto &vertex: worldSpace) {
                vertices.push_back(vertex);
            }
            cube = constructRotationFigure(vertices, cam,rotSteps->getText().toInt(),axis);
        }
    });

    rotSteps->onTextChange([=,&cube,&pointsFig,&cam,&axis](){
        if(!pointsFig.empty()) {
            std::vector<sf::Vector3f> vertices;
            auto worldSpace = cam.screenToMap(pointsFig);
            //vertices.emplace_back(0, worldSpace[0].y, 0);
            for (const auto &vertex: worldSpace) {
                vertices.push_back(vertex);
            }
        }
    });

    rotAxis->onTextChange([=,&axis](){
            switch (rotAxis->getText()[0]) {
                case 'x':
                {
                    axis = Axis::X;
                    break;
                }
                case 'y':
                {
                    axis = Axis::Y;
                    break;
                }case 'z':
                {
                    axis = Axis::Z;
                    break;
                }

            }

    });


    auto prev = high_resolution_clock::now();
    auto fps = gui.get<tgui::Label>("fps");
    canvas->onClick([&]() {
        if (mode == Mode::FiguresRotate) {
            pointsFig.push_back(sf::Mouse::getPosition());
            Edges edges;
            for (int i = 0; i < pointsFig.size() - 1; ++i) {
                edges.emplace_back(i, i + 1);
            }

            std::vector<sf::Vector3f> points = cam.screenToMap(pointsFig);
            std::vector<Vertex> vertices;
            vertices.resize(points.size());
            std::transform(points.begin(), points.end(), vertices.begin(), [](auto v) { return Vertex(v); });
            cube = Object({0, 0, 0}, vertices, edges);
        } else {
            pointsFig.clear();
        }
    });

    auto clearButton = gui.get<tgui::Button>("clear");
    clearButton->onClick([&](){
        canvas->clear(sf::Color::White);
        cube = createCube();
        pointsFig.clear();
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
                            switch (mode) {
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
                            switch (mode) {
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
                            switch (mode) {
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
                            switch (mode) {
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
                            switch (mode) {
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
                            switch (mode) {
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
                                case Mode::Axis: {
                                    cube.rotateAroundX(x1Input->getText().toFloat(0));
                                    cube.rotateAroundY(y1Input->getText().toFloat(0));
                                    cube.rotateAroundZ(z1Input->getText().toFloat(0));
                                    break;
                                }
                                case Mode::Center: {
                                    cube.rotateAroundLine(x1Input->getText().toFloat(0), Axis::X);
                                    cube.rotateAroundLine(y1Input->getText().toFloat(0), Axis::Y);
                                    cube.rotateAroundLine(z1Input->getText().toFloat(0), Axis::Z);
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
        auto diff = duration_cast<milliseconds>(high_resolution_clock::now() - prev).count();
        prev = high_resolution_clock::now();
        fps->setText(tgui::String(1000 / diff));
        gui.draw();
        window.display();
    }

    return 0;
}
