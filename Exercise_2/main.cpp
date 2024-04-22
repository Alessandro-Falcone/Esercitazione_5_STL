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

    unsigned int nVertices;
    unsigned int* v1 = nullptr;
    unsigned int* v2 = nullptr;
    unsigned int* v3 = nullptr;
    unsigned int numeroRigheCell2Ds = 0;
    unsigned int nTriangoli = 0;

    if(!letturaMesh(percorso, mesh)){
       cerr << "Errore: impossibile leggere la mesh" << endl;
       return 1;
    }

    ifstream fileCell0Ds(percorso + "/Cell0Ds.csv");
    string line;

    if(fileCell0Ds.fail()){
        cerr << "Errore impossibile aprire il file Cell0Ds" << endl;
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

    ifstream fileCell2Ds(percorso + "/Cell2Ds.csv");

    if(fileCell2Ds.fail()){
        cerr << "Errore: impossibile aprire il file Cell2Ds" << endl;
        return 1;
    }else{

        getline(fileCell2Ds, line);
        while(getline(fileCell2Ds, line)){
            numeroRigheCell2Ds++;
        }
    }

    string Cell2Ds = percorso + "/Cell2Ds.csv";

    if(!datiFileCell2Ds(Cell2Ds, nVertices, v1, v2, v3, numeroRigheCell2Ds, nTriangoli)){
        cerr << "Errore: impossibile aprire il file" << endl;
        return 1;
    }else{

        cout << nTriangoli << endl;
            for(unsigned int i = 0; i < nTriangoli; i++){
                for(unsigned int j = 0; j < numeroRigheCell0Ds; j++){
                    if(v1[i] == id[j]){
                        cout << i << " id: " << id[j] << "--";
                    }
                }
            }
    cout << endl;
    }
    delete[] id;
    delete[] x;
    delete[] y;

    delete[] v1;
    delete[] v2;
    delete[] v3;

    return 0;
}


