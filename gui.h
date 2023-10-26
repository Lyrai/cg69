#ifndef CG6_GUI_H
#define CG6_GUI_H

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "object.h"
#include "camera.h"

enum class Mode{
    Axis,
    Line,
    Center,
    Scale,
    Move,
    Mirror
};

void setupGui(tgui::Gui& gui, Object& cube, Camera& cam);

#endif //CG6_GUI_H
