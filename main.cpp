#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "camera.h"
#include "figures.h"


sf::Color background_color;

enum class Mode{
    Axis,
    Line,
    Center,
    Scale,
    Move,
    Mirror
};

int main() {
    background_color = sf::Color::White;
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
    tgui::Gui gui(window);
    Mode mode;
    auto canvas = tgui::CanvasSFML::create({"100%", "100%"});
    gui.add(canvas);
    auto layoutMain = tgui::HorizontalLayout::create({"100%", "5%"});
    auto layoutProjection = tgui::HorizontalLayout::create({"100%", "5%"});
    auto layoutTransformation = tgui::HorizontalLayout::create({"100%", "5%"});
    auto layoutFigures = tgui::HorizontalLayout::create({"100%", "5%"});
    auto layoutRotation = tgui::HorizontalLayout::create({"100%", "5%"});
    auto layoutInput = tgui::HorizontalLayout::create({"40%", "3%"});
    layoutInput->setPosition("0%","7%");
    layoutProjection->setVisible(false);
    layoutTransformation->setVisible(false);
    layoutFigures->setVisible(false);
    layoutRotation->setVisible(false);
    layoutInput->setVisible(false);

    gui.add(layoutMain);
    gui.add(layoutProjection);
    gui.add(layoutTransformation);
    gui.add(layoutFigures);
    gui.add(layoutRotation);
    gui.add(layoutInput);

    //Main
    auto projectionButton = tgui::Button::create("Projections");
    projectionButton->setTextSize(20);
    auto transformationButton = tgui::Button::create("Transformation");
    transformationButton->setTextSize(20);
    auto figuresButton = tgui::Button::create("Figures");
    figuresButton->setTextSize(20);

    layoutMain->add(projectionButton);
    layoutMain->add(transformationButton);
    layoutMain->add(figuresButton);

    layoutMain->insertSpace(0, 0.025);
    layoutMain->insertSpace(2, 0.025);
    layoutMain->insertSpace(4, 0.025);
    layoutMain->insertSpace(6, 0.025);


    //Projection
    auto parallelButton = tgui::Button::create("Parallel");
    parallelButton->setTextSize(20);

    auto perspectiveButton = tgui::Button::create("Perspective");
    perspectiveButton->setTextSize(20);
    auto backButtonProj = tgui::Button::create("Back");
    backButtonProj->setTextSize(20);
    layoutProjection->add(parallelButton);
    layoutProjection->add(perspectiveButton);
    layoutProjection->add(backButtonProj);
    layoutProjection->insertSpace(0, 0.025);
    layoutProjection->insertSpace(2, 0.025);
    layoutProjection->insertSpace(4, 0.025);
    layoutProjection->insertSpace(6, 0.025);

    //Transformation
    auto rotateButton = tgui::Button::create("Rotate");
    rotateButton->setTextSize(20);
    auto mirrorButton = tgui::Button::create("Mirror");
    mirrorButton->setTextSize(20);
    auto scaleButton = tgui::Button::create("Scale");
    scaleButton->setTextSize(20);
    auto moveButton = tgui::Button::create("Move");
    moveButton->setTextSize(20);
    auto backButtonTrans = tgui::Button::create("Back");
    backButtonTrans->setTextSize(20);
    layoutTransformation->add(rotateButton);
    layoutTransformation->add(mirrorButton);
    layoutTransformation->add(scaleButton);
    layoutTransformation->add(moveButton);
    layoutTransformation->add(backButtonTrans);

    layoutTransformation->insertSpace(0, 0.025);
    layoutTransformation->insertSpace(2, 0.025);
    layoutTransformation->insertSpace(4, 0.025);
    layoutTransformation->insertSpace(6, 0.025);
    layoutTransformation->insertSpace(8, 0.025);
    layoutTransformation->insertSpace(10, 0.025);

    //Figures
    auto cubeButton = tgui::Button::create("Cube");
    cubeButton->setTextSize(20);
    auto tetrahedronButton = tgui::Button::create("Tetrahedron");
    tetrahedronButton->setTextSize(20);
    auto octahedronButton = tgui::Button::create("Octahedron");
    octahedronButton->setTextSize(20);
    auto icosahedronButton = tgui::Button::create("Icosahedron");
    icosahedronButton->setTextSize(20);
    auto dodecahedronButton = tgui::Button::create("Dodecahedron");
    dodecahedronButton->setTextSize(18);
    auto shelestStarButton = tgui::Button::create("Star");
    shelestStarButton->setTextSize(20);
    auto backButtonFig = tgui::Button::create("Back");
    backButtonFig->setTextSize(20);

    layoutFigures->add(cubeButton);
    layoutFigures->add(tetrahedronButton);
    layoutFigures->add(octahedronButton);
    layoutFigures->add(icosahedronButton);
    layoutFigures->add(dodecahedronButton);
    layoutFigures->add(shelestStarButton);
    layoutFigures->add(backButtonFig);



    layoutFigures->insertSpace(0, 0.025);
    layoutFigures->insertSpace(2, 0.025);
    layoutFigures->insertSpace(4, 0.025);
    layoutFigures->insertSpace(6, 0.025);
    layoutFigures->insertSpace(8, 0.025);
    layoutFigures->insertSpace(10, 0.025);
    layoutFigures->insertSpace(12, 0.025);
    layoutFigures->insertSpace(14, 0.025);
    layoutFigures->insertSpace(16, 0.025);
    layoutFigures->insertSpace(18, 0.025);

    //Rotation
    auto rotateAxisButton = tgui::Button::create("Axis");
    rotateAxisButton->setTextSize(20);
    auto rotateCenterButton = tgui::Button::create("Center");
    rotateCenterButton->setTextSize(20);
    auto rotateLineButton = tgui::Button::create("Line");
    rotateLineButton->setTextSize(20);
    auto backButtonRot = tgui::Button::create("Back");
    backButtonRot->setTextSize(20);



    layoutRotation->add(rotateAxisButton);
    layoutRotation->add(rotateCenterButton);
    layoutRotation->add(rotateLineButton);
    layoutRotation->add(backButtonRot);


    layoutRotation->insertSpace(0, 0.025);
    layoutRotation->insertSpace(2, 0.025);
    layoutRotation->insertSpace(4, 0.025);
    layoutRotation->insertSpace(6, 0.025);
    layoutRotation->insertSpace(8, 0.025);


    auto rotateArea1 = tgui::EditBox::create();
    auto rotateArea2 = tgui::EditBox::create();
    auto rotateArea3 = tgui::EditBox::create();
    auto rotateArea4 = tgui::EditBox::create();
    auto rotateArea5 = tgui::EditBox::create();
    auto rotateArea6 = tgui::EditBox::create();
    auto text1 = tgui::Label::create("x1");
    auto text2 = tgui::Label::create("y1");
    auto text3 = tgui::Label::create("z1");
    auto text4 = tgui::Label::create("x2");
    auto text5 = tgui::Label::create("y2");
    auto text6 = tgui::Label::create("z2");
    text1->setTextSize(30);
    text2->setTextSize(30);
    text3->setTextSize(30);
    text4->setTextSize(30);
    text5->setTextSize(30);
    text6->setTextSize(30);
    text1->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
    text1->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    text2->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
    text2->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    text3->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
    text3->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    text4->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
    text4->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    text5->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
    text5->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    text6->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
    text6->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);





    layoutInput->add(text1);
    layoutInput->addSpace(0.05);
    layoutInput->add(rotateArea1);
    layoutInput->addSpace(0.05);
    layoutInput->add(text2);
    layoutInput->addSpace(0.05);
    layoutInput->add(rotateArea2);
    layoutInput->addSpace(0.05);
    layoutInput->add(text3);
    layoutInput->addSpace(0.05);
    layoutInput->add(rotateArea3);
    layoutInput->addSpace(0.05);
    layoutInput->add(text4);
    layoutInput->addSpace(0.05);
    layoutInput->add(rotateArea4);
    layoutInput->addSpace(0.05);
    layoutInput->add(text5);
    layoutInput->addSpace(0.05);
    layoutInput->add(rotateArea5);
    layoutInput->addSpace(0.05);
    layoutInput->add(text6);
    layoutInput->addSpace(0.05);
    layoutInput->add(rotateArea6);
    layoutInput->addSpace(0.05);
    layoutInput->setVisible(false);







    sf::Texture texture;
    //auto window_size = window.getSize();
    auto window_size = canvas->getSize();
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

    parallelButton->onClick([&cam]() { cam.setProjection(Projection::Parallel); });
    perspectiveButton->onClick([&cam]() { cam.setProjection(Projection::Perspective); });
    projectionButton->onClick([=]() {
        layoutMain->setVisible(false);
        layoutProjection->setVisible(true);
    });
    transformationButton->onClick([=]() {
        layoutMain->setVisible(false);
        layoutTransformation->setVisible(true);
    });
    figuresButton->onClick([=]() {
        layoutMain->setVisible(false);
        layoutFigures->setVisible(true);
    });
    backButtonProj->onClick([=]() {
        layoutProjection->setVisible(false);
        layoutMain->setVisible(true);
    });
    backButtonTrans->onClick([=]() {
        layoutTransformation->setVisible(false);
        layoutMain->setVisible(true);
    });
    backButtonFig->onClick([=]() {
        layoutFigures->setVisible(false);
        layoutMain->setVisible(true);
    });
    backButtonRot->onClick([=]() {
        layoutRotation->setVisible(false);
        layoutTransformation->setVisible(true);
        layoutInput->setVisible(false);
    });
    cubeButton->onClick([&cube](){
        cube = createCube();
    });
    rotateButton->onClick([=](){
        layoutInput->setVisible(true);
        layoutTransformation->setVisible(false);
        layoutRotation->setVisible(true);
    });
    tetrahedronButton->onClick([&cube](){
        cube = createTetrahedron();
    });
    octahedronButton->onClick([&cube](){
        cube = createOctahedron();
    });
    icosahedronButton->onClick([&cube](){
        cube = createIcosahedron();
    });
    dodecahedronButton->onClick([&cube](){
        cube = createDodecahedron();
    });
    shelestStarButton->onClick([&cube](){
        cube = createShelestStar();
    });
    rotateAxisButton->onClick([=,&mode](){
        mode = Mode::Axis;
        text4->setVisible(false);
        text5->setVisible(false);
        text6->setVisible(false);
        rotateArea4->setVisible(false);
        rotateArea5->setVisible(false);
        rotateArea6->setVisible(false);
    });
    rotateCenterButton->onClick([=,&mode](){
        mode = Mode::Center;
        text4->setVisible(false);
        text5->setVisible(false);
        text6->setVisible(false);
        rotateArea4->setVisible(false);
        rotateArea5->setVisible(false);
        rotateArea6->setVisible(false);
    });
    rotateLineButton->onClick([=,&mode](){
        text4->setVisible(true);
        text5->setVisible(true);
        text6->setVisible(true);
        rotateArea4->setVisible(true);
        rotateArea5->setVisible(true);
        rotateArea6->setVisible(true);
        mode = Mode::Line;
    });


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
