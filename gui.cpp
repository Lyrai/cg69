#include "gui.h"
#include "figures.h"

enum class Mode;

void setupGui(tgui::Gui& gui, Object& cube, Camera& cam, Mode& mode) {
    auto canvas = tgui::CanvasSFML::create({"100%", "100%"});
    canvas->setWidgetName("canvas");
    gui.add(canvas);
    auto layoutMain = tgui::HorizontalLayout::create({"100%", "5%"});
    auto layoutProjection = tgui::HorizontalLayout::create({"100%", "5%"});
    auto layoutTransformation = tgui::HorizontalLayout::create({"100%", "5%"});
    auto layoutFigures = tgui::HorizontalLayout::create({"100%", "5%"});
    auto layoutRotation = tgui::HorizontalLayout::create({"100%", "5%"});
    auto layoutInput = tgui::HorizontalLayout::create({"40%", "3%"});
    auto layoutGraphInput = tgui::Group::create({"100%", "6%"});
    auto layoutRotationFigure = tgui::HorizontalLayout::create({"100%", "5%"});
    auto layoutLoadSave = tgui::HorizontalLayout::create({"100%", "5%"});
    auto layoutRotFigInput = tgui::HorizontalLayout::create({"20%", "3%"});
    auto renderingLayout = tgui::HorizontalLayout::create({"100%", "5%"});
    auto fps = tgui::Label::create();
    fps->setTextSize(20);
    fps->setPosition("95%", "7%");
    fps->setWidgetName("fps");
    layoutInput->setPosition("0%","7%");
    layoutGraphInput->setPosition("0%","7%");
    layoutRotFigInput->setPosition("0%","7%");
    layoutProjection->setVisible(false);
    layoutTransformation->setVisible(false);
    layoutFigures->setVisible(false);
    layoutRotation->setVisible(false);
    layoutInput->setVisible(false);
    layoutGraphInput->setVisible(false);
    layoutRotationFigure->setVisible(false);
    layoutLoadSave->setVisible(false);
    layoutRotFigInput->setVisible(false);
    renderingLayout->setVisible(false);

    gui.add(layoutMain);
    gui.add(layoutProjection);
    gui.add(layoutTransformation);
    gui.add(layoutFigures);
    gui.add(layoutRotation);
    gui.add(layoutInput);
    gui.add(layoutGraphInput);
    gui.add((layoutRotationFigure));
    gui.add(layoutLoadSave);
    gui.add(fps);
    gui.add(layoutRotFigInput);
    gui.add(renderingLayout);

    //Main
    auto projectionButton = tgui::Button::create("Projections");
    projectionButton->setTextSize(20);
    auto transformationButton = tgui::Button::create("Transformation");
    transformationButton->setTextSize(20);
    auto figuresButton = tgui::Button::create("Figures");
    figuresButton->setTextSize(20);
    auto graphButton = tgui::Button::create("Graph");
    graphButton->setTextSize(20);
    auto rotatingFigButton = tgui::Button::create("Rotation Figures");
    rotatingFigButton->setTextSize(15);
    rotatingFigButton->setWidgetName("rotFig");
    auto loadSaveButton = tgui::Button::create("Load/Save");
    loadSaveButton->setTextSize(20);
    auto renderingButton = tgui::Button::create("Rendering");
    renderingButton->setTextSize(20);

    layoutMain->add(transformationButton);
    layoutMain->add(figuresButton);
    layoutMain->add(graphButton);
    layoutMain->add(loadSaveButton);
    layoutMain->add(renderingButton);

    layoutMain->insertSpace(0, 0.025);
    layoutMain->insertSpace(2, 0.025);
    layoutMain->insertSpace(4, 0.025);
    layoutMain->insertSpace(6, 0.025);
    layoutMain->insertSpace(8, 0.025);
    layoutMain->insertSpace(10, 0.025);

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
    auto tetrahedronButton = tgui::Button::create("Tetrahedron");
    auto octahedronButton = tgui::Button::create("Octahedron");
    auto icosahedronButton = tgui::Button::create("Icosahedron");
    auto dodecahedronButton = tgui::Button::create("Dodecahedron");
    auto shelestStarButton = tgui::Button::create("Star");
    auto backButtonFig = tgui::Button::create("Back");

    cubeButton->setTextSize(20);
    tetrahedronButton->setTextSize(20);
    octahedronButton->setTextSize(20);
    icosahedronButton->setTextSize(20);
    dodecahedronButton->setTextSize(18);
    shelestStarButton->setTextSize(20);
    backButtonFig->setTextSize(20);


    layoutFigures->add(cubeButton);
    layoutFigures->add(tetrahedronButton);
    layoutFigures->add(octahedronButton);
    layoutFigures->add(icosahedronButton);
    layoutFigures->add(dodecahedronButton);
    layoutFigures->add(rotatingFigButton);
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

    //Rotation
    auto rotateAxisButton = tgui::Button::create("Axis");
    auto rotateCenterButton = tgui::Button::create("Center");
    auto rotateLineButton = tgui::Button::create("Line");
    auto backButtonRot = tgui::Button::create("Back");

    rotateAxisButton->setTextSize(20);
    rotateCenterButton->setTextSize(20);
    rotateLineButton->setTextSize(20);
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

    rotateArea1->setWidgetName("x1");
    rotateArea2->setWidgetName("y1");
    rotateArea3->setWidgetName("z1");
    rotateArea4->setWidgetName("x2");
    rotateArea5->setWidgetName("y2");
    rotateArea6->setWidgetName("z2");

    auto text1 = tgui::Label::create("x1");
    auto text2 = tgui::Label::create("y1");
    auto text3 = tgui::Label::create("z1");
    auto text4 = tgui::Label::create("x2");
    auto text5 = tgui::Label::create("y2");
    auto text6 = tgui::Label::create("z2");

    text1->setTextSize(20);
    text2->setTextSize(20);
    text3->setTextSize(20);
    text4->setTextSize(20);
    text5->setTextSize(20);
    text6->setTextSize(20);

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


    auto graphLabelZ = tgui::Label::create("z = ");
    auto graphInputFormula = tgui::EditBox::create();
    auto graphParamsLayout = tgui::HorizontalLayout::create();
    auto graphX0 = tgui::EditBox::create();
    auto graphX1 = tgui::EditBox::create();
    auto graphY0 = tgui::EditBox::create();
    auto graphY1 = tgui::EditBox::create();
    auto graphSteps = tgui::EditBox::create();

    graphX0->setWidgetName("graphX0");
    graphX0->setTextSize(16);
    graphX1->setWidgetName("graphX1");
    graphX1->setTextSize(16);
    graphY0->setWidgetName("graphY0");
    graphY0->setTextSize(16);
    graphY1->setWidgetName("graphY1");
    graphY1->setTextSize(16);
    graphSteps->setWidgetName("graphSteps");
    graphSteps->setTextSize(16);

    auto graphX0Text = tgui::Label::create("x0 = ");
    auto graphX1Text = tgui::Label::create("x1 = ");
    auto graphY0Text = tgui::Label::create("y0 = ");
    auto graphY1Text = tgui::Label::create("y1 = ");
    auto graphStepsText = tgui::Label::create("steps = ");

    graphX0Text->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
    graphX0Text->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    graphX1Text->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
    graphX1Text->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    graphY0Text->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
    graphY0Text->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    graphY1Text->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
    graphY1Text->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    graphStepsText->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
    graphStepsText->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);

    graphX0Text->setTextSize(20);
    graphX0Text->setScrollbarPolicy(tgui::Scrollbar::Policy::Never);
    graphX1Text->setTextSize(20);
    graphX1Text->setScrollbarPolicy(tgui::Scrollbar::Policy::Never);
    graphY0Text->setTextSize(20);
    graphY0Text->setScrollbarPolicy(tgui::Scrollbar::Policy::Never);
    graphY1Text->setTextSize(20);
    graphY1Text->setScrollbarPolicy(tgui::Scrollbar::Policy::Never);
    graphStepsText->setTextSize(20);
    graphStepsText->setScrollbarPolicy(tgui::Scrollbar::Policy::Never);

    graphParamsLayout->add(graphX0Text);
    graphParamsLayout->add(graphX0);
    graphParamsLayout->add(graphX1Text);
    graphParamsLayout->add(graphX1);
    graphParamsLayout->add(graphY0Text);
    graphParamsLayout->add(graphY0);
    graphParamsLayout->add(graphY1Text);
    graphParamsLayout->add(graphY1);
    graphParamsLayout->add(graphStepsText);
    graphParamsLayout->add(graphSteps);


    layoutGraphInput->add(graphParamsLayout);

    graphInputFormula->setWidgetName("formula");
    graphLabelZ->setTextSize(20);
    graphInputFormula->setTextSize(16);
    graphInputFormula->setPosition(graphLabelZ->getSize().x + 8, graphLabelZ->getPosition().y);
    graphInputFormula->setSize({"25%", graphLabelZ->getSize().y});

    graphLabelZ->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);

    graphParamsLayout->setPosition({"-4%", graphLabelZ->getSize().y + 8});
    graphParamsLayout->setSize({"92%", graphLabelZ->getSize().y});

    layoutGraphInput->add(graphLabelZ);
    layoutGraphInput->add(graphInputFormula);

    auto drawButton = tgui::Button::create("Draw");
    auto clearButton = tgui::Button::create("Clear");
    auto back4Button = tgui::Button::create("Back");
    auto addPointsButton = tgui::Button::create("Add points");
    drawButton->setWidgetName("draw");
    clearButton->setWidgetName("clear");
    drawButton->setTextSize(20);
    addPointsButton->setTextSize(20);
    clearButton->setTextSize(20);
    back4Button->setTextSize(20);


    layoutRotationFigure->addSpace(0.05);
    layoutRotationFigure->add(addPointsButton);
    layoutRotationFigure->addSpace(0.05);
    layoutRotationFigure->add(drawButton);
    layoutRotationFigure->addSpace(0.05);
    layoutRotationFigure->add(clearButton);
    layoutRotationFigure->addSpace(0.05);
    layoutRotationFigure->add(back4Button);
    layoutRotationFigure->addSpace(0.05);

    auto stepInputRotation = tgui::EditBox::create();
    auto stepLabel = tgui::Label::create("steps = ");
    auto axisLabel = tgui::Label::create("Axis = ");
    auto axisInput = tgui::EditBox::create();
    stepInputRotation->setWidgetName("stepsRot");
    axisInput->setWidgetName("axisRot");
    stepLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    axisLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    stepLabel->setTextSize(20);
    axisLabel->setTextSize(20);
    layoutRotFigInput->add(stepLabel);
    layoutRotFigInput->add(stepInputRotation);
    layoutRotFigInput->addSpace(0.05);
    layoutRotFigInput->add(axisLabel);
    layoutRotFigInput->add(axisInput);



    auto loadButton = tgui::Button::create("Load");
    auto saveButton = tgui::Button::create("Save");
    auto loadSaveBackButton = tgui::Button::create("Back");

    loadButton->setWidgetName("load");
    saveButton->setWidgetName("save");

    loadButton->setTextSize(20);
    saveButton->setTextSize(20);
    loadSaveBackButton->setTextSize(20);

    layoutLoadSave->addSpace(0.05);
    layoutLoadSave->add(loadButton);
    layoutLoadSave->addSpace(0.05);
    layoutLoadSave->add(saveButton);
    layoutLoadSave->addSpace(0.05);
    layoutLoadSave->add(loadSaveBackButton);
    layoutLoadSave->addSpace(0.05);


    auto zTestButton = tgui::Button::create(cam.getZTest() ? "Z Test Off" : "Z Test On");
    zTestButton->setTextSize(20);
    auto renderingBackButton = tgui::Button::create("Back");
    renderingBackButton->setTextSize(20);

    renderingLayout->addSpace(0.025);
    renderingLayout->add(projectionButton);
    renderingLayout->addSpace(0.025);
    renderingLayout->add(zTestButton);
    renderingLayout->addSpace(0.025);
    renderingLayout->add(renderingBackButton);
    renderingLayout->addSpace(0.025);

    parallelButton->onClick([&cam]() { cam.setProjection(Projection::Parallel); });
    perspectiveButton->onClick([&cam]() { cam.setProjection(Projection::Perspective); });
    renderingButton->onClick([=]() {
        layoutMain->setVisible(false);
        renderingLayout->setVisible(true);
    });
    projectionButton->onClick([=]() {
        renderingLayout->setVisible(false);
        layoutGraphInput->setVisible(false);
        layoutProjection->setVisible(true);
    });
    transformationButton->onClick([=]() {
        layoutMain->setVisible(false);
        layoutGraphInput->setVisible(false);
        layoutTransformation->setVisible(true);
    });
    figuresButton->onClick([=]() {
        layoutMain->setVisible(false);
        layoutGraphInput->setVisible(false);
        layoutFigures->setVisible(true);
    });
    graphButton->onClick([=]() {
        layoutGraphInput->setVisible(true);
    });
    backButtonProj->onClick([=]() {
        layoutProjection->setVisible(false);
        renderingLayout->setVisible(true);
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
    loadSaveBackButton->onClick([=]() {
        layoutLoadSave->setVisible(false);
        layoutMain->setVisible(true);
    });
    cubeButton->onClick([&cube](){
        cube = createCube();
    });
    rotateButton->onClick([=](){
        layoutInput->setVisible(true);
        layoutTransformation->setVisible(false);
        layoutRotation->setVisible(true);
        text4->setVisible(false);
        text5->setVisible(false);
        text6->setVisible(false);
        rotateArea4->setVisible(false);
        rotateArea5->setVisible(false);
        rotateArea6->setVisible(false);
    });
    tetrahedronButton->onClick([&cube](){
        cube = parseFigure("figures/tetrahedron_poly.json");
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
    moveButton->onClick([=,&mode](){
        mode = Mode::Move;
    });
    mirrorButton->onClick([=,&mode](){
        mode = Mode::Mirror;
    });
    scaleButton->onClick([=,&mode](){
        mode = Mode::Scale;
    });
    rotatingFigButton->onClick([=](){
        layoutFigures->setVisible(false);
        layoutRotationFigure->setVisible(true);
        layoutRotFigInput->setVisible(true);
    });
    back4Button->onClick([=](){
       layoutRotationFigure->setVisible(false);
       layoutFigures->setVisible(true);
       layoutRotFigInput->setVisible(false);
    });
    addPointsButton->onClick([&mode](){
        mode = Mode::FiguresRotate;
    });
    loadSaveButton->onClick([=]() {
        layoutMain->setVisible(false);
        layoutLoadSave->setVisible(true);
        layoutGraphInput->setVisible(false);
    });
    zTestButton->onClick([=, &cam]() {
        cam.setZTest(!cam.getZTest());
        zTestButton->setText(cam.getZTest() ? "Z Test Off" : "Z Test On");
    });
    renderingBackButton->onClick([=]() {
        renderingLayout->setVisible(false);
        layoutMain->setVisible(true);
    });
}