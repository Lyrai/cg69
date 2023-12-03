#include <cmath>
#include <nlohmann/json.hpp>
#include <fstream>
#include <regex>
#include "figures.h"
#include "indexpolygon.h"

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
        std::vector<std::vector<std::pair<std::string, std::vector<std::string>>>> _polygons;
    };

    json to_json(const Object& obj) {
        raw_data rd;
        for (const auto &vertex: obj.vertices()) {
            rd._vertices.emplace_back(std::vector<std::string> {std::to_string(vertex.x), std::to_string(vertex.y), std::to_string(vertex.z)});
        }

        for (const auto &polygon: obj.polygons()) {
            std::vector<std::pair<std::string, std::vector<std::string>>> p;
            for (const auto idx: polygon.indices()) {
                p.push_back({std::to_string(idx.index()), {std::to_string(idx.coords().x), std::to_string(idx.coords().y)}});
            }

            rd._polygons.emplace_back(std::move(p));
        }

        return json{{"vertices", rd._vertices},
                 {"polygons", rd._polygons}};
    }

    void from_json(const json &j, raw_data &rd) {
        j.at("vertices").get_to(rd._vertices);
        for (const auto &item: j.at("polygons")) {
            auto str = to_string(item);
            std::vector<std::pair<std::string, std::vector<std::string>>> poly;
            for (const auto &vertex: item) {
                std::vector<std::string> coords;
                vertex.at("coords").get_to(coords);
                std::string idx;
                vertex.at("idx").get_to(idx);
                poly.emplace_back(idx, coords);
            }

            rd._polygons.emplace_back(std::move(poly));
        }
    }
}

std::vector<Vertex> parseVertices(const std::vector<std::vector<std::string>> &raw_vertices) {
    auto rval = std::vector<Vertex>();
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

Polygons parsePolygons(const std::vector<std::vector<std::pair<std::string, std::vector<std::string>>>> &raw_polygons) {
    auto rval = Polygons();
    for (const auto &poly_raw: raw_polygons) {
        std::vector<IndexVertex> polygon {};
        for (const auto &poly_val: poly_raw) {
            sf::Vector2f vec { std::stof(poly_val.second[0]), std::stof(poly_val.second[1]) };
            polygon.emplace_back(std::stoi(poly_val.first), vec);
        }
        rval.emplace_back(polygon);
    }
    return rval;
}

Object createCube() {
    std::vector<Vertex> cubeVertices{
            {-0.5, -0.5, -0.5},
            {0.5,  -0.5, -0.5},
            {0.5,  0.5,  -0.5},
            {-0.5, 0.5,  -0.5},
            {-0.5, -0.5, 0.5},
            {0.5,  -0.5, 0.5},
            {0.5,  0.5,  0.5},
            {-0.5, 0.5,  0.5},
    };

    Polygons cubePolygons {
            {{4, 5, 6, 7}},
            {{0, 1, 2, 3}},
            {{0, 1, 5, 4}},
            {{0, 3, 7, 4}},
            {{1, 2, 6, 5}},
            {{3, 2, 6, 7}}
    };

    Object cube({0, 0, 0}, cubeVertices, cubePolygons);
    return cube;
}

Object createOctahedron() {
    std::vector<Vertex> octahedronVertices{
            {0.0,  0.0,  -1.0},
            {1.0,  0.0,  0.0},
            {0.0,  -1.0, 0.0},
            {-1.0, 0.0,  0.0},
            {0.0,  1.0,  0.0},
            {0.0,  0.0,  1.0}
    };

    /*std::vector<std::pair<int, int>> octahedronEdges{
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
    };*/

    Polygons octahedronPolygons {
            {{0, 1, 2}},
            {{0, 1, 4}},
            {{0, 2, 3}},
            {{0, 3, 4}},
            {{1, 2, 5}},
            {{1, 4, 5}},
            {{2, 3, 5}},
            {{3, 4, 5}}
    };

    Object octahedron({0, 0, 0}, octahedronVertices, octahedronPolygons);
    return octahedron;
}

Object createTetrahedron() {
    std::vector<Vertex> tetrahedronVertices{
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

    Polygons tetrahedronPolygons {
            {{0, 1, 2}},
            {{0, 1, 3}},
            {{0, 2, 3}},
            {{1, 2, 3}}
    };

    Object tetrahedron({0, 0, 0}, tetrahedronVertices, tetrahedronPolygons);
    return tetrahedron;
}

Object createIcosahedron() {
    float phi = 1.6180;
    float coef = sqrt(1 + phi * phi);
    std::vector<Vertex> icosahedronVertices{
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

    Polygons icosahedronPolygons {
            {{0, 1, 8}},
            {{0, 1, 9}},
            {{0, 4, 7}},
            {{0, 4, 8}},
            {{0, 7, 9}},
            {{1, 5, 6}},
            {{1, 5, 8}},
            {{1, 6, 9}},
            {{2, 3, 10}},
            {{2, 3, 11}},
            {{2, 5, 6}},
            {{2, 5, 11}},
            {{2, 6, 10}},
            {{3, 4, 7}},
            {{3, 4, 11}},
            {{3, 7, 10}},
            {{4, 8, 11}},
            {{5, 8, 11}},
            {{6, 9, 10}},
            {{7, 9, 10}}
    };

    Object icosahedron({0, 0, 0}, icosahedronVertices, icosahedronPolygons);
    return icosahedron;
}

Object createShelestStar() {
    float coef = sqrt(1 + 1.6180 * 1.6180);
    float phi = 1.6180f * coef;
    std::vector<Vertex> dodecahedronVertices{
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

    Polygons starPolygons {
            {{16, 2, 10, 8, 0}},
            {{12, 14, 4, 8, 0}},
            {{16, 17, 1, 12, 0}},
            {{17, 3, 11, 9, 1}},
            {{12, 14, 5, 9, 1}},
            {{13, 15, 6, 10, 2}},
            {{16, 17, 3, 13, 2}},
            {{13, 15, 7, 11, 3}},
            {{18, 6, 10, 8, 4}},
            {{18, 19, 5, 14, 4}},
            {{19, 7, 11, 9, 5}},
            {{18, 19, 7, 15, 6}}
    };

    Object dodecahedron({0, 0, 0}, dodecahedronVertices, starPolygons);
    return dodecahedron;
}

Object createDodecahedron() {
    float phi = 1.6180;
    float coef = sqrt(1 + phi * phi);
    std::vector<Vertex> dodecahedronVertices{
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

    Polygons dodecahedronPolygons {
            {{16, 2, 10, 8, 0}},
            {{12, 14, 4, 8, 0}},
            {{16, 17, 1, 12, 0}},
            {{17, 3, 11, 9, 1}},
            {{12, 14, 5, 9, 1}},
            {{13, 15, 6, 10, 2}},
            {{16, 17, 3, 13, 2}},
            {{13, 15, 7, 11, 3}},
            {{18, 6, 10, 8, 4}},
            {{18, 19, 5, 14, 4}},
            {{19, 7, 11, 9, 5}},
            {{18, 19, 7, 15, 6}}
    };

    Object dodecahedron({0, 0, 0}, dodecahedronVertices, dodecahedronPolygons);
    return dodecahedron;
}

Object parseFigure(const std::string &path) {
    std::ifstream f(path);
    json data = json::parse(f);
    auto st = data.get<ns::raw_data>();
    std::vector<Vertex> vertices = parseVertices(st._vertices);
//    std::vector<std::pair<int, int>> Edges = parseEdges(st._edges);
    Polygons polygons = parsePolygons(st._polygons);
    Object figure({0, 0, 0}, vertices, polygons);
    return figure;
}

void saveFigure(const Object& obj, const std::string& path) {
    std::ofstream file(path);
    file << to_string(ns::to_json(obj));
}

std::vector<Object> constructRoomPlanes() {
    std::vector<Object> result;
    result.reserve(6);

    Polygons planePolygons {std::vector<IndexVertex> {0, 1, 2, 3}};
    Object bottomPlane({0, 0, 0}, {{-10, 10, -10}, {10, 10, -10}, {10, 10, 50}, {-10, 10, 50}}, planePolygons , {0, -1, 0});
    result.push_back(bottomPlane);

    Object topPlane({0, 0, 0}, {{-10, -10, -10}, {10, -10, -10}, {10, -10, 50}, {-10, -10, 50}}, planePolygons , {0, 1, 0});
    result.push_back(topPlane);

    Object leftPlane({0, 0, 0}, {{-10, 10, -10}, {-10, 10, 50}, {-10, -10, 50}, {-10, -10, -10}}, planePolygons , {1, 0, 0});
    result.push_back(leftPlane);

    Object rightPlane({0, 0, 0}, {{10, 10, -10}, {10, 10, 50}, {10, -10, 50}, {10, -10, -10}}, planePolygons , {-1, 0, 0});
    result.push_back(rightPlane);

    Object frontPlane({0, 0, 0}, {{-10, 10, 50}, {10, 10, 50}, {10, -10, 50}, {-10, -10, 50}}, planePolygons , {0, 0, -1});
    result.push_back(frontPlane);

    Object backPlane({0, 0, 0}, {{-10, 10, -10}, {10, 10, -10}, {10, -10, -10}, {-10, -10, -10}}, planePolygons , {0, 0, 1});
    result.push_back(backPlane);

    return result;
}