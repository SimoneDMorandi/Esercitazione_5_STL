#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <limits>

namespace Polygonal_Library {


// Corpo della funzione di verifica della validità della Mesh.
bool ImportMesh(const string& filepath, PolygonalMesh& mesh, double &scaling)
{
    // Controllo che l'importazione delle celle 0D vada a buon fine.
    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",mesh))
    {
        cerr << "Impossibile importare le celle 0D." << endl;
        return false;
    }
    else
    {
        // Test 1 : Verifico che i punti siano memorizzati correttamente, stampando ogni marker e i punti ad esso associati.
        cout << "Test 1"<< endl;
        cout << "Cell0D Markers:" << endl;
        for(auto iterator = mesh.Marker0D.begin(); iterator != mesh.Marker0D.end(); iterator++)
        {
            cout << "Key: " << iterator -> first << " Values:";
            for(const unsigned int id : iterator -> second)
            {
                cout << " " << id;
            }
            cout << endl;
        }
        cout << endl;
    }

    // Controllo che l'importazione delle celle 1D vada a buon fine.
    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",mesh))
    {
        cerr << "Impossibile importare le celle 1D." << endl;
        return false;
    }
    else
    {
        // TEST 2 : Verifico che i punti siano memorizzati correttamente, stampando ogni marker e i vertici ad esso associati.
        cout << "Test 2"<< endl;
        cout << "Cell1D Markers:" << endl;
        for(auto iterator = mesh.Marker1D.begin(); iterator != mesh.Marker1D.end(); iterator++)
        {
            cout << "Key: " << iterator -> first << " Values:";
            for(const unsigned int id : iterator -> second)
            {
                cout << " " << id;
            }
            cout << endl;
        }
        cout << endl;
    }

    // TEST 3 : Controllo che nessun lato abbia lunghezza nulla.
    cout << "Test 3: ";
    cout.flush();      // Il flush viene inserito per questioni di leggibilità dell'output in caso di errore.
    if(!CheckLength(mesh, scaling))
    {
        return false;
    }
    cout << "Lunghezza dei lati valida." << endl << endl;

    // Controllo che l'importazione delle celle 2D vada a buon fine.
    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",mesh))
    {
        return false;
    }
    cout << "Importazione delle celle 2D avvenuta con successo." << endl << endl;

    // TEST 4 : Controllo che ci sia corrispondenza tra i lati 1D e 2D.
    cout << "Test 4: ";
    cout.flush();
    if(!CheckVertices2D(mesh))
    {
        return false;
    }
    cout << "Corrispondenza tra lati 1D e 2D valida." << endl << endl;

    // TEST 5 : Controllo che nessun poligono abbia area nulla.
    cout << "Test 5: ";
    cout.flush();
    if(!CheckArea(mesh, scaling))
    {
        return false;
    }
    cout << "Area dei poligoni valida." << endl << endl;

    return true;
}

// Importo e memorizzo le informazioni relative alle celle 0D.
bool ImportCell0Ds(const string &filename,PolygonalMesh& mesh)
{
    // Apro il file e controllo che l'apertura vada a buon fine.
    ifstream file;
    file.open(filename);
    if(file.fail())
    {
        return false;
    }

    // Inizio a leggere il file, salvando i dati in una lista di stringhe.
    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        listLines.push_back(line);
    }

    // Rimuovo l'header.
    listLines.pop_front();

    // Memorizzo il numero di punti.
    mesh.n0D = listLines.size();

    // Verifico che il file letto non fosse vuoto.
    if (mesh.n0D == 0)
    {
        cerr << "Non ci sono celle 0D." << endl;
        return false;
    }

    // Inizializzo la memoria per le informazioni successive.
    mesh.Id0D.reserve(mesh.n0D);
    mesh.Coord0D.reserve(mesh.n0D);

    // Leggo ogni stringa, memorizzando i dati.
    for (const string& line : listLines)
    {
        istringstream converter(line);
        string token;
        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        getline(converter,token,';');
        istringstream(token)>>id;

        getline(converter,token,';');
        istringstream(token)>>marker;

        getline(converter,token,';');
        istringstream(token)>>coord(0);

        getline(converter,token,';');
        istringstream(token)>>coord(1);

        mesh.Id0D.push_back(id);
        mesh.Coord0D.push_back(coord);

        // Se non mi trovo sul lato del dominio (marker == 0), memorizzo marker e ID.
        if(marker != 0)
        {
            auto ret = mesh.Marker0D.insert({marker, {id}});

            // Se la chiave esiste già, aggiungo il valore di ID.
            if(!ret.second)
            {
                (ret.first)->second.push_back(id);
            }
        }
    }

    // Chiudo il file di lettura.
    file.close();

    return true;
}

// Importo e memorizzo le informazioni relative alle celle 2D.
bool ImportCell1Ds(const string &filename,PolygonalMesh& mesh)
{
    // Apro il file e controllo che l'apertura vada a buon fine.
    ifstream file;
    file.open(filename);
    if(file.fail())
    {
        return false;
    }

    // Inizio a leggere il file, salvando i dati in una lista di stringhe.
    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        listLines.push_back(line);
    }

    // Rimuovo l'header.
    listLines.pop_front();

    // Memorizzo il numero di lati.
    mesh.n1D = listLines.size();

    // Verifico che il file letto non fosse vuoto.
    if (mesh.n1D == 0)
    {
        cerr << "Non ci sono celle 1D." << endl;
        return false;
    }

    // Inizializzo la memoria per le informazioni successive.
    mesh.Id1D.reserve(mesh.n1D);
    mesh.Vertices1D.reserve(mesh.n1D);

    // Leggo ogni stringa, memorizzando i dati.
    for (const string& line : listLines)
    {
        istringstream converter(line);
        string token;
        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        getline(converter,token,';');
        istringstream(token)>>id;

        getline(converter,token,';');
        istringstream(token)>>marker;

        getline(converter,token,';');
        istringstream(token)>>vertices(0);

        getline(converter,token,';');
        istringstream(token)>>vertices(1);

        mesh.Id1D.push_back(id);
        mesh.Vertices1D.push_back(vertices);

        // Se non mi trovo sul lato del dominio (marker == 0), memorizzo marker e ID.
        if(marker != 0)
        {
            auto ret = mesh.Marker1D.insert({marker, {id}});

            // Se la chiave esiste già, aggiungo il valore di ID.
            if(!ret.second)
            {
                (ret.first)->second.push_back(id);
            }
        }
    }

    // Chiudo il file di lettura.
    file.close();

    return true;
}

// Corpo della funzione di controllo dei lati.
bool CheckLength(PolygonalMesh& mesh, double &scaling)
{
    // Definisco la precisione personalizzata.
    double eps = numeric_limits<double>::epsilon()*scaling;

    // Scorro il vettore che contiene per ogni lato il numero dei vertici associati.
    for(const auto& points : mesh.Vertices1D)
    {
        // Ricavo le coordinate dei singoli vertici e controllo la loro distanza.
        Vector2d coord_1 = mesh.Coord0D[points(0)];
        Vector2d coord_2 = mesh.Coord0D[points(1)];
        if(abs(coord_1(0)-coord_2(0)) < eps && abs(coord_1(1)-coord_2(1)) < eps)
        {
            cerr << "Ci sono dei lati di lunghezza nulla." << endl;
            return false;
        }
    }
    return true;
}

// Importo e memorizzo le informazioni relative alle celle 2D.
bool ImportCell2Ds(const string &filename,PolygonalMesh& mesh)
{
    // Apro il file e controllo che l'apertura vada a buon fine.
    ifstream file;
    file.open(filename);
    if(file.fail())
    {
        return false;
    }

    // Inizio a leggere il file, salvando i dati in una lista di stringhe.
    list<string> listLines;
    string line;
    while (getline(file, line))
    {
        listLines.push_back(line);
    }

    // Rimuovo l'header.
    listLines.pop_front();

    // Memorizzo il numero di poligoni.
    mesh.n2D = listLines.size();

    // Verifico che il file letto non fosse vuoto.
    if (mesh.n2D == 0)
    {
        cerr << "Non ci sono celle 2D." << endl;
        return false;
    }

    // Inizializzo la memoria per le informazioni successive.
    mesh.Id2D.reserve(mesh.n2D);
    mesh.Vertices2D.reserve(mesh.n2D);
    mesh.Edges2D.reserve(mesh.n2D);

    // Leggo ogni stringa, memorizzando i dati.
    for (const string& line : listLines)
    {
        istringstream converter(line);
        string token;
        unsigned int id;
        unsigned int sizeVer;
        unsigned int sizeEdg;
        VectorXi vertices;
        VectorXi edges;

        getline(converter,token,';');
        istringstream(token)>>id;

        getline(converter,token,';'); // I marker sono tutti nulli, non li memorizzo.

        // Riscalo il vettore dei vertici ora che conosco la lunghezza.
        getline(converter,token,';');
        istringstream(token)>>sizeVer;
        vertices.resize(sizeVer);

        // Memorizzo i vertici di ogni poligono.
        for(unsigned int i=0; i<sizeVer; i++)
        {
            getline(converter,token,';');
            istringstream(token)>>vertices(i);
        }

        // Riscalo il vettore dei lati ora che conosco la lunghezza.
        getline(converter,token,';');
        istringstream(token)>>sizeEdg;
        edges.resize(sizeEdg);

        // Memorizzo i lati di ogni poligono.
        for(unsigned int i=0; i<sizeEdg; i++)
        {
            getline(converter,token,';');
            istringstream(token)>>edges(i);
        }

        mesh.Id2D.push_back(id);
        mesh.Vertices2D.push_back(vertices);
        mesh.Edges2D.push_back(edges);

    }

    // Chiudo il file di lettura.
    file.close();

    return true;
}

// Corpo della funzione che verifica la corrispondenza tra i lati 1D e 2D.
bool CheckVertices2D(PolygonalMesh& mesh)
{
    for(const auto& vertices : mesh.Vertices2D)
    {
        // Imposto una flag per la conferma della corrispondenza.
        bool found = false;

        // Scorro il contenitore dei vertici per ogni poligono.
        for(unsigned int i = 0; i < vertices.size(); i++)
        {
            // Confronto le coppie di vertici con le coppie di lati.
            if(i != vertices.size() -1)
            {
                Vector2i to_find(vertices(i),vertices(i+1));
                for(const auto& vertices_couple : mesh.Vertices1D)
                {
                    // Considero anche le coppie di vertici invertite, dato che sono comunque valide.
                    Vector2i swapped(vertices_couple[1],vertices_couple[0]);
                    if(to_find == vertices_couple || to_find == swapped)
                    {
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    cerr << "I lati 2D non corrispondono ai lati 1D." << endl;
                    return false;
                }
            }

            // Confronto la coppia del primo vertice e l'ultimo con le coppie di lati allo stesso modo.
            else
            {
                Vector2i to_find(vertices(i),vertices(0));
                for(const auto& vertices_couple : mesh.Vertices1D)
                {
                    Vector2i swapped(vertices_couple[1],vertices_couple[0]);
                    if(to_find == vertices_couple || to_find == swapped)
                    {
                        found = true;
                        break;
                    }
                }
                if(!found)
                {
                    cerr << "I lati 2D non corrispondono ai lati 1D." << endl;
                    return false;
                }
            }
        }
    }
    return true;
}

// Corpo della funzione di controllo delle aree.
bool CheckArea(PolygonalMesh& mesh, double &scaling)
{
    // Definisco la precisione personalizzata
    double eps = numeric_limits<double>::epsilon()*scaling;

    // Scorro il contenitore dei lati di ogni poligono.
    for(const auto& edges : mesh.Edges2D)
    {
        // Definisco il contatore di lati paralleli (prodotto vettoriale == 0)
        unsigned int counter_zeros = 0;

        /* Calcolo il prodotto vettoriale tra lati successivi, accedendo prima al
           vettore contenente i lati, poi a quello dei punti, poi a quello delle coordinate.*/
        for(unsigned int i = 0; i <edges.size(); i++)

            // Calcolo il prodotto vettoriale dei primi n-1 lati.
            if(i != edges.size()-1)
            {
                Vector2i points_edge_a = mesh.Vertices1D[edges(i)];  // a = [A,B]
                Vector2d coord_point_A = mesh.Coord0D[points_edge_a(0)]; // A = [x1,y1]
                Vector2d coord_point_B = mesh.Coord0D[points_edge_a(1)];  // B = [x2,y2]
                Vector2d length_a = coord_point_B-coord_point_A;  // B-A
                Vector2i points_edge_b = mesh.Vertices1D[edges(i+1)]; // b = [D,C]
                Vector2d coord_point_C = mesh.Coord0D[points_edge_b(0)]; // C = [x3,y3]
                Vector2d coord_point_D = mesh.Coord0D[points_edge_b(1)]; // D = [x4,y4]
                Vector2d length_b = coord_point_D-coord_point_C;  // D-C
                double cross_prod = length_a.x() * length_b.y() - length_a.y() * length_b.x(); // (a)x(b)

                // Controllo se il prodotto scalare è sotto la tolleranza impostata.
                if(abs(cross_prod) < eps)
                {
                    counter_zeros ++;
                }
            }

            // Calcolo il prodotto vettoriale tra l'ultimo lato e il primo allo stesso modo.
            else
            {
                Vector2i points_edge_a = mesh.Vertices1D[edges(i)];
                Vector2d coord_point_A = mesh.Coord0D[points_edge_a(0)];
                Vector2d coord_point_B = mesh.Coord0D[points_edge_a(1)];
                Vector2d length_a = coord_point_B-coord_point_A;
                Vector2i points_edge_b = mesh.Vertices1D[edges(0)];
                Vector2d coord_point_C = mesh.Coord0D[points_edge_b(0)];
                Vector2d coord_point_D = mesh.Coord0D[points_edge_b(1)];
                Vector2d length_b = coord_point_D-coord_point_C;
                double cross_prod = length_a.x() * length_b.y() - length_a.y() * length_b.x();
                if(abs(cross_prod) < eps)
                {

                    counter_zeros ++;
                }
            }

        // Verifico se tutti i lati del poligono sono paralleli, cioè se il poligono è degenere.
        if(counter_zeros == edges.size())
        {
            cerr << "Ci sono dei poligoni con area nulla." << endl;
            return false;
        }
    }
    return true;
}

// Chiusura del Namespace.
}
