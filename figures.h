

#ifndef CG6_FIGURES_H
#define CG6_FIGURES_H

#include <nlohmann/json.hpp>
#include "object.h"

using json = nlohmann::json;

Object createCube();
Object createTetrahedron();
Object createOctahedron();
Object createIcosahedron();

Object createDodecahedron();

Object createShelestStar();

Object parseFigure(const std::string& path);

std::vector<sf::Vector3f> parseVertices(const std::vector<std::vector<std::string>> &raw_vertices);

std::vector<std::pair<int, int>> parseEdges(const std::vector<std::pair<std::string, std::string>> &raw_edges);

Polygons parsePolygons(const std::vector<std::vector<std::string>> &raw_polygons);
#endif //CG6_FIGURES_H
