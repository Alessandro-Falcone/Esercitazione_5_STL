#include <iostream>
#include <fstream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;


int main(){

    PolygonalMesh mesh;

    string percorso = "PolygonalMesh";

    unsigned int* id = nullptr;
    double* x = nullptr;
    double* y = nullptr;
    unsigned int numeroRigheCell0Ds = 0;

    if(!letturaMesh(percorso, mesh)){
       cerr << "Errore: impossibile leggere la mesh" << endl;
       return 1;
    }

    ifstream fileCell0Ds(percorso + "/Cell0Ds.csv");
    string line;

    if(fileCell0Ds.fail()){
        cerr << "Errore impossibile aprire il file" << endl;
        return 1;
    }else{

        getline(fileCell0Ds, line);
        while(getline(fileCell0Ds, line)){
            numeroRigheCell0Ds++;
        }
    }

    string Cell0Ds = percorso + "/Cell0Ds.csv";

    if(!datiFileCell0Ds(Cell0Ds, id, x, y, numeroRigheCell0Ds)){
        cerr << "Errore: impossibile aprire il file" << endl;
        return 1;
    }

    delete[] id;
    delete[] x;
    delete[] y;

    return 0;
}


