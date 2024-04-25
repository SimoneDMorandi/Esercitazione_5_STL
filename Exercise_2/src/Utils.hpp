#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace Polygonal_Library {

/* Definisco la funzione che controlla se l'import è avvenuto correttamente
ed esegue qualche test sulla mesh.*/
bool ImportMesh(const string &filepath, PolygonalMesh& mesh, double& scaling);

// Definisco la funzione che importa le informazioni sui nodi.
bool ImportCell0Ds(const string &filename, PolygonalMesh& mesh);

// Definisco la funzione che importa le informazioni sui vertici.
bool ImportCell1Ds(const string &filename, PolygonalMesh& mesh);

// Definisco la funzione che importa le informazioni sui poligoni.
bool ImportCell2Ds(const string &filename, PolygonalMesh& mesh);

// Definisco la funzione che controlla la lunghezza dei lati.
bool CheckLength(PolygonalMesh& mesh, double& scaling);

// Definisco la funzione che controlla la corrispondenza tra lati 1D e 2D.
bool CheckVertices2D(PolygonalMesh& mesh);

// Definisco la funzione che controlla le aree dei poligoni.
bool CheckArea(PolygonalMesh& mesh, double& scaling);

}
