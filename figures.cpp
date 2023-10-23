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
    float phi = 1.6180;
    float coef = sqrt(1+1.6180*1.6180);
    std::vector<sf::Vector3f> icosahedronVertices{
            {phi/coef,1.0f/coef,0},
            {phi/coef,-1.0f/coef,0},
            {-phi/coef,-1.0f/coef,0},
            {-phi/coef,1.0f/coef,0},
            {0,phi/coef,1.0f/coef},
            {0,-phi/coef,1.0f/coef},
            {0,-phi/coef,-1.0f/coef},
            {0,phi/coef,-1.0f/coef},
            {1.0f/coef,0,phi/coef},
            {1.0f/coef,0,-phi/coef},
            {-1.0f/coef,0,-phi/coef},
            {-1.0f/coef,0,phi/coef}
    };

    std::vector<std::pair<int, int>> icosahedronEdges{
            {7,0},
            {7,4},
            {7,3},//3
            {7,10},
            {7,9},
            {5,8},//6
            {5,11},
            {5,2},
            {5,6},//9
            {5,1},
            {0,8},
            {0,4},//12
            {8,4},
            {8,11},
            {4,11},//15
            {4,3},
            {11,3},
            {11,2},//18
            {3,2},
            {3,10},
            {2,10},//21
            {2,6},
            {10,6},
            {10,9},//24
            {6,9},
            {6,1},
            {9,1},//27
            {9,0},
            {1,0},
            {1,8}//30
    };
    Object icosahedron({0, 0, 0}, icosahedronVertices, icosahedronEdges);
    return icosahedron;
}

Object createShelestStar()
{
    float coef = sqrt(1+1.6180*1.6180);
    float phi = 1.6180*coef;
    std::vector<sf::Vector3f> dodecahedronVertices{
            {1.f/coef,1.f/coef,1.f/coef},//0
            {1.f/coef,1.f/coef,-1.f/coef},//1
            {1.f/coef,-1.f/coef,1.f/coef},//2
            {1.f/coef,-1.f/coef,-1.f/coef},//3
            {-1.f/coef,1.f/coef,1.f/coef},//4
            {-1.f/coef,1.f/coef,-1.f/coef},//5
            {-1.f/coef,-1.f/coef,1.f/coef},//6
            {-1.f/coef,-1.f/coef,-1.f/coef},//7
            {0,1/phi,phi},//8
            {0,1/phi,-phi},//9
            {0,-1/phi,phi},//10
            {0,-1/phi,-phi},//11
            {1/phi,phi,0},//12
            {1/phi,-phi,0},//13
            {-1/phi,phi,0},//14
            {-1/phi,-phi,0},//15
            {phi,0,1/phi},//16
            {phi,0,-1/phi},//17
            {-phi,0,1/phi},//18
            {-phi,0,-1/phi}//19
    };

    std::vector<std::pair<int, int>> dodecahedronEdges{
            {8,10},
            {8,0},
            {8,4},
            {10,2},
            {10,6},
            {9,11},
            {9,1},
            {9,5},
            {11,3},
            {11,7},
            //------------------
            {12,14},
            {13,15},
            {12,0},
            {12,1},
            {14,4},
            {14,5},
            {13,2},
            {13,3},
            {15,6},
            {15,7},
            //---------------------------
            {16,17},
            {18,19},
            {16,0},
            {16,2},
            {17,1},
            {17,3},
            {18,4},
            {18,6},
            {19,5},
            {19,7}
    };
    Object dodecahedron({0, 0, 0}, dodecahedronVertices, dodecahedronEdges);
    return dodecahedron;
}

Object createDodecahedron()
{
    float phi = 1.6180;
    float coef = sqrt(1+1.6180*1.6180);
    std::vector<sf::Vector3f> dodecahedronVertices{
            {1,1,1},//0
            {1,1,-1},//1
            {1,-1,1},//2
            {1,-1,-1},//3
            {-1,1,1},//4
            {-1,1,-1},//5
            {-1,-1,1},//6
            {-1,-1,-1},//7
            {0,1/phi,phi},//8
            {0,1/phi,-phi},//9
            {0,-1/phi,phi},//10
            {0,-1/phi,-phi},//11
            {1/phi,phi,0},//12
            {1/phi,-phi,0},//13
            {-1/phi,phi,0},//14
            {-1/phi,-phi,0},//15
            {phi,0,1/phi},//16
            {phi,0,-1/phi},//17
            {-phi,0,1/phi},//18
            {-phi,0,-1/phi}//19
    };

    std::vector<std::pair<int, int>> dodecahedronEdges{
            {8,10},
            {8,0},
            {8,4},
            {10,2},
            {10,6},
            {9,11},
            {9,1},
            {9,5},
            {11,3},
            {11,7},
            //------------------
            {12,14},
            {13,15},
            {12,0},
            {12,1},
            {14,4},
            {14,5},
            {13,2},
            {13,3},
            {15,6},
            {15,7},
            //---------------------------
            {16,17},
            {18,19},
            {16,0},
            {16,2},
            {17,1},
            {17,3},
            {18,4},
            {18,6},
            {19,5},
            {19,7}
    };
    Object dodecahedron({0, 0, 0}, dodecahedronVertices, dodecahedronEdges);
    return dodecahedron;
}