cmake_minimum_required(VERSION 3.26)
project(cg6)

set(CMAKE_CXX_STANDARD 20)

add_executable(cg6 main.cpp pixbuf.cpp pixel.cpp polygon.cpp line.cpp
        matrix.h
        camera.cpp
        camera.h
        object.cpp
        object.h
        transform.cpp
        transform.h
        matrix.cpp
        algorithm.cpp
        algorithm.h
        figures.h
        figures.cpp
        gui.cpp
        gui.h)
target_link_libraries(cg6 sfml-graphics sfml-window sfml-system tgui)