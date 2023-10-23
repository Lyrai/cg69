//
// Created by nikita on 23.10.23.
//
#include "figures.h"
#include "cmath"
Object createCube()
{
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

Object createOctahedron()
{
    std::vector<sf::Vector3f> octahedronVertices{
            {0.0, 0.0, -1.0},
            {1.0, 0.0, 0.0},
            {0.0, -1.0, 0.0},
            {-1.0, 0.0, 0.0},
            {0.0, 1.0, 0.0},
            {0.0, 0.0, 1.0}
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

Object createTetrahedron()
{
    std::vector<sf::Vector3f> tetrahedronVertices{
            {1.0, 1.0, 1.0},
            {1.0, -1.0, -1.0},
            {-1.0, 1.0, -1.0},
            {-1.0, -1.0, 1.0}
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

Object createIcosahedron()
{
    std::vector<sf::Vector3f> icosahedronVertices{
            {0.0, 1.0, M_PI},
            {0.0, 1.0, -M_PI},
            {0.0, -1.0, M_PI},
            {0.0, -1.0, -M_PI},
            {1.0, M_PI, 0.0},
            {1.0, -M_PI, 0.0},
            {-1.0, M_PI, 0.0},
            {-1.0, -M_PI, 0.0},
            {M_PI, 0.0, 1.0},
            {M_PI, 0.0, -1.0},
            {-M_PI, 0.0, 1.0},
            {-M_PI, 0.0, -1.0}
    };

    std::vector<std::pair<int, int>> icosahedronEdges{
            {0, 1},
            {0, 2},
            {0, 3},
            {0, 4},
            {0, 5},
            {1, 2},
            {1, 3},
            {1, 6},
            {2, 4},
            {2, 7},
            {3, 5},
            {3, 8},
            {4, 9},
            {4, 10},
            {5, 11},
            {5, 12},
            {6, 7},
            {6, 8},
            {6, 9},
            {7, 10},
            {7, 11},
            {8, 12},
            {8, 9},
            {9, 10},
            {10, 11},
            {11, 12},
            {12, 13},
            {13, 14},
            {13, 15},
            {14, 15}
    };
    Object icosahedron({0, 0, 0}, icosahedronVertices, icosahedronEdges);
    return icosahedron;
}