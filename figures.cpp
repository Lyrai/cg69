#include "figures.h"
#include "cmath"
#include <nlohmann/json.hpp>
#include <fstream>
#include <regex>

using json = nlohmann::json;


float eval(const std::string &s) {

    float phi = 1.6180;
    float coef = sqrt(1 + 1.6180 * 1.6180);
    float phi_t = 1 / phi;
    float coef_t = 1 / coef;
    float phi_coef = phi / coef;

    std::regex pattern("^\\s+|\\s+$");
    std::regex pattern_phi_t("1/phi");
    std::regex pattern_coef_t("1/coef");
    std::regex pattern_phi_coef("phi/coef");
    std::regex pattern_phi("phi");

    auto t = std::regex_replace(s, pattern, "");
    t = std::regex_replace(s, pattern_phi_t, std::to_string(phi_t));
    t = std::regex_replace(t, pattern_coef_t, std::to_string(coef_t));
    t = std::regex_replace(t, pattern_phi_coef, std::to_string(phi_coef));
    t = std::regex_replace(t, pattern_phi, std::to_string(phi));

    return std::stof(t);
}

namespace ns {
    using json = nlohmann::json;
    struct raw_data {
        std::vector<std::vector<std::string>> _vertices;
        std::vector<std::pair<std::string, std::string>> _edges;
    };

    void to_json(json &j, const raw_data &rd) {
        j = json{{"vertices", rd._vertices},
                 {"edges",    rd._edges}};
    }

    void from_json(const json &j, raw_data &rd) {
        j.at("vertices").get_to(rd._vertices);
        j.at("edges").get_to(rd._edges);
    }
}

std::vector<sf::Vector3f> parseVertices(const std::vector<std::vector<std::string>> &raw_vertices) {
    auto rval = std::vector<sf::Vector3f>();
    for (auto point_raw: raw_vertices) {
        sf::Vector3f point(eval(point_raw.at(0)), eval(point_raw.at(1)), eval(point_raw.at(2)));
        rval.emplace_back(point);
    }
    return rval;
}

std::vector<std::pair<int, int>> parseEdges(const std::vector<std::pair<std::string, std::string>> &raw_edges) {
    auto rval = std::vector<std::pair<int, int>>();
    for (const auto &edge_raw: raw_edges) {
        std::pair<int, int> edge(std::stoi(edge_raw.first), std::stoi(edge_raw.second));
        rval.emplace_back(edge);
    }
    return rval;
}

Object createCube() {
    std::vector<sf::Vector3f> cubeVertices{
            {-0.5, -0.5, -0.5},
            {0.5,  -0.5, -0.5},
            {0.5,  0.5,  -0.5},
            {-0.5, 0.5,  -0.5},
            {-0.5, -0.5, 0.5},
            {0.5,  -0.5, 0.5},
            {0.5,  0.5,  0.5},
            {-0.5, 0.5,  0.5},
    };
    std::vector<std::pair<int, int>> cubeEdges{
            {0, 1},
            {1, 2},
            {2, 3},
            {3, 0},
            {0, 4},
            {1, 5},
            {2, 6},
            {3, 7},
            {4, 5},
            {5, 6},
            {6, 7},
            {7, 4}
    };
    Object cube({0, 0, 0}, cubeVertices, cubeEdges);
    return cube;
}

Object createOctahedron() {
    std::vector<sf::Vector3f> octahedronVertices{
            {0.0,  0.0,  -1.0},
            {1.0,  0.0,  0.0},
            {0.0,  -1.0, 0.0},
            {-1.0, 0.0,  0.0},
            {0.0,  1.0,  0.0},
            {0.0,  0.0,  1.0}
    };

    std::vector<std::pair<int, int>> octahedronEdges{
            {0, 1},
            {0, 2},
            {0, 3},
            {0, 4},
            {1, 2},
            {2, 3},
            {3, 4},
            {4, 1},
            {1, 5},
            {2, 5},
            {3, 5},
            {4, 5}
    };

    Object octahedron({0, 0, 0}, octahedronVertices, octahedronEdges);
    return octahedron;
}

Object createTetrahedron() {
    std::vector<sf::Vector3f> tetrahedronVertices{
            {(float)sqrt(3)/2, 0, 0},
            {0, 0.5, 0},
            {0, -0.5, 0},
            {(float)sqrt(3)/6, 0, (float)std::sqrt((float)2/3)}
    };

    std::vector<std::pair<int, int>> tetrahedronEdges{
            {0, 1},
            {0, 2},
            {0, 3},
            {1, 2},
            {1, 3},
            {2, 3}
    };

    Object tetrahedron({0, 0, 0}, tetrahedronVertices, tetrahedronEdges);
    return tetrahedron;
}

Object createIcosahedron() {
    float phi = 1.6180;
    float coef = sqrt(1 + 1.6180 * 1.6180);
    std::vector<sf::Vector3f> icosahedronVertices{
            {phi / coef,   1.0f / coef,  0},
            {phi / coef,   -1.0f / coef, 0},
            {-phi / coef,  -1.0f / coef, 0},
            {-phi / coef,  1.0f / coef,  0},
            {0,            phi / coef,   1.0f / coef},
            {0,            -phi / coef,  1.0f / coef},
            {0,            -phi / coef,  -1.0f / coef},
            {0,            phi / coef,   -1.0f / coef},
            {1.0f / coef,  0,            phi / coef},
            {1.0f / coef,  0,            -phi / coef},
            {-1.0f / coef, 0,            -phi / coef},
            {-1.0f / coef, 0,            phi / coef}
    };

    std::vector<std::pair<int, int>> icosahedronEdges{
            {7,  0},
            {7,  4},
            {7,  3},//3
            {7,  10},
            {7,  9},
            {5,  8},//6
            {5,  11},
            {5,  2},
            {5,  6},//9
            {5,  1},
            {0,  8},
            {0,  4},//12
            {8,  4},
            {8,  11},
            {4,  11},//15
            {4,  3},
            {11, 3},
            {11, 2},//18
            {3,  2},
            {3,  10},
            {2,  10},//21
            {2,  6},
            {10, 6},
            {10, 9},//24
            {6,  9},
            {6,  1},
            {9,  1},//27
            {9,  0},
            {1,  0},
            {1,  8}//30
    };
    Object icosahedron({0, 0, 0}, icosahedronVertices, icosahedronEdges);
    return icosahedron;
}

Object createShelestStar() {
    float coef = sqrt(1 + 1.6180 * 1.6180);
    float phi = 1.6180 * coef;
    std::vector<sf::Vector3f> dodecahedronVertices{
            {1.f / coef,  1.f / coef,  1.f / coef},//0
            {1.f / coef,  1.f / coef,  -1.f / coef},//1
            {1.f / coef,  -1.f / coef, 1.f / coef},//2
            {1.f / coef,  -1.f / coef, -1.f / coef},//3
            {-1.f / coef, 1.f / coef,  1.f / coef},//4
            {-1.f / coef, 1.f / coef,  -1.f / coef},//5
            {-1.f / coef, -1.f / coef, 1.f / coef},//6
            {-1.f / coef, -1.f / coef, -1.f / coef},//7
            {0,           1 / phi,     phi},//8
            {0,           1 / phi,     -phi},//9
            {0,           -1 / phi,    phi},//10
            {0,           -1 / phi,    -phi},//11
            {1 / phi,     phi,         0},//12
            {1 / phi,     -phi,        0},//13
            {-1 / phi,    phi,         0},//14
            {-1 / phi,    -phi,        0},//15
            {phi,         0,           1 / phi},//16
            {phi,         0,           -1 / phi},//17
            {-phi,        0,           1 / phi},//18
            {-phi,        0,           -1 / phi}//19
    };

    std::vector<std::pair<int, int>> dodecahedronEdges{
            {8,  10},
            {8,  0},
            {8,  4},
            {10, 2},
            {10, 6},
            {9,  11},
            {9,  1},
            {9,  5},
            {11, 3},
            {11, 7},
            //------------------
            {12, 14},
            {13, 15},
            {12, 0},
            {12, 1},
            {14, 4},
            {14, 5},
            {13, 2},
            {13, 3},
            {15, 6},
            {15, 7},
            //---------------------------
            {16, 17},
            {18, 19},
            {16, 0},
            {16, 2},
            {17, 1},
            {17, 3},
            {18, 4},
            {18, 6},
            {19, 5},
            {19, 7}
    };
    Object dodecahedron({0, 0, 0}, dodecahedronVertices, dodecahedronEdges);
    return dodecahedron;
}

Object createDodecahedron() {
    float phi = 1.6180;
    float coef = sqrt(1 + 1.6180 * 1.6180);
    std::vector<sf::Vector3f> dodecahedronVertices{
            {1,        1,        1},//0
            {1,        1,        -1},//1
            {1,        -1,       1},//2
            {1,        -1,       -1},//3
            {-1,       1,        1},//4
            {-1,       1,        -1},//5
            {-1,       -1,       1},//6
            {-1,       -1,       -1},//7
            {0,        1 / phi,  phi},//8
            {0,        1 / phi,  -phi},//9
            {0,        -1 / phi, phi},//10
            {0,        -1 / phi, -phi},//11
            {1 / phi,  phi,      0},//12
            {1 / phi,  -phi,     0},//13
            {-1 / phi, phi,      0},//14
            {-1 / phi, -phi,     0},//15
            {phi,      0,        1 / phi},//16
            {phi,      0,        -1 / phi},//17
            {-phi,     0,        1 / phi},//18
            {-phi,     0,        -1 / phi}//19
    };

    std::vector<std::pair<int, int>> dodecahedronEdges{
            {8,  10},
            {8,  0},
            {8,  4},
            {10, 2},
            {10, 6},
            {9,  11},
            {9,  1},
            {9,  5},
            {11, 3},
            {11, 7},
            //------------------
            {12, 14},
            {13, 15},
            {12, 0},
            {12, 1},
            {14, 4},
            {14, 5},
            {13, 2},
            {13, 3},
            {15, 6},
            {15, 7},
            //---------------------------
            {16, 17},
            {18, 19},
            {16, 0},
            {16, 2},
            {17, 1},
            {17, 3},
            {18, 4},
            {18, 6},
            {19, 5},
            {19, 7}
    };
    Object dodecahedron({0, 0, 0}, dodecahedronVertices, dodecahedronEdges);
    return dodecahedron;
}

Object parseFigure(const std::string &path) {
    std::ifstream f(path);
    json data = json::parse(f);
    auto st = data.get<ns::raw_data>();
    std::vector<sf::Vector3f> Vertices = parseVertices(st._vertices);
    std::vector<std::pair<int, int>> Edges = parseEdges(st._edges);
    Object cube({0, 0, 0}, Vertices, Edges);
    return cube;
}
