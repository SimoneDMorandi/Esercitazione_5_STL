#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


namespace Polygonal_Library{

struct PolygonalMesh
{
    // Definisco le strutture dati dove verranno salvate le informazioni delle varie celle.

    // Cell 0D.
    unsigned int n0D = 0;
    vector<unsigned int> Id0D = {};
    vector<Vector2d> Coord0D = {};
    map<unsigned int, list<unsigned int>> Marker0D = {};

    // Cell 1D.
    unsigned int n1D = 0;
    vector<unsigned int> Id1D = {};
    vector<Vector2i> Vertices1D = {};  // So che i lati avranno due componenti, quindi uso un vettore di dimensione fissata.
    map<unsigned int, list<unsigned int>> Marker1D = {};

    // Cell 2D.
    unsigned int n2D = 0;
    vector<unsigned int> Id2D = {};

    // Non conosco a priori la dimensione del poligono, quindi uso un vettore di dimensione variabile.
    vector<VectorXi> Vertices2D = {};
    vector<VectorXi> Edges2D = {};
};
}
