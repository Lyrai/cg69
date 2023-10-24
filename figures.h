

#ifndef CG6_FIGURES_H
#define CG6_FIGURES_H

#include <nlohmann/json.hpp>
#include "object.h"

using json = nlohmann::json;

Object createCube();

Object parseCube();

Object createOctahedron();

Object parseOctahedron();

Object createTetrahedron();

Object parseTetrahedron();

Object createIcosahedron();

Object parseIcosahedron();

Object createDodecahedron();

Object parseDodecahedron();

Object createShelestStar();

std::vector<sf::Vector3f> parseVertices(const std::vector<std::vector<std::string>> &raw_vertices);

std::vector<std::pair<int, int>> parseEdges(const std::vector<std::pair<std::string, std::string>> &raw_edges);

#endif //CG6_FIGURES_H
