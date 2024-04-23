#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"

using namespace std;
using namespace Polygonal_Library;

int main()
{
    // Definisco un alias per la struttura.
    PolygonalMesh mesh;

    //Definisco il percorso per i file relativi alla mesh.
    string filepath = "PolygonalMesh";

    /* Definisco lo scalamento applicato all'epsilon di macchina per la tolleranza.
       La tolleranza sarà data da: eps*eps_scaling.*/
    double eps_scaling = 10.5;

    // Verifico che l'import dei file e i test della mesh vadano a buon fine.
    if(!ImportMesh(filepath,mesh,eps_scaling))
    {
        cerr << "Mesh non valida." << endl;
        return 1;
    }
    cout << "Mesh valida.";
    return 0;
}
