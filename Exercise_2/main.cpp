#include <iostream>
#include <fstream>
#include <iomanip>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "Eigen/Eigen"
// #include <math.h>

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

double lunghezzaLato(double diffx, double diffy){

    double lunLato = 0;

    lunLato = sqrt((diffx*diffx) + (diffy*diffy));

    return lunLato;
}


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
    double lato1 = 0;
    double lato2 = 0;
    double lato3 = 0;

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
        cout << "Nel file Cell0Ds sono stati individuati " << nTriangoli << " triangoli"<< endl;
            for(unsigned int i = 0; i < nTriangoli; i++){
                // cout << "v1: " << v1[i] << " " << x[v1[i]] << " " << "v2: " << v2[i] << " x2: " << x[v2[i]] << " y: " << y[v1[i]] << " " << y[v2[i]] << endl;
                lato1 = lunghezzaLato(abs(x[v1[i]] - x[v2[i]]), abs(y[v1[i]] - y[v2[i]]));
                if(lato1 <= 1e-27){
                    cerr << "i punti sono uguali" << endl;

                }else{
                    cout << fixed << setprecision(12) << i << " lato 1: "  << lato1 << endl;
                }
                lato2 = lunghezzaLato(abs(x[v1[i]] - x[v3[i]]), abs(y[v1[i]] - y[v3[i]]));
                cout << fixed << setprecision(12) << i <<" lato 2: " << lato2 << endl;
                lato3 = lunghezzaLato(abs(x[v2[i]] - x[v3[i]]), abs(y[v1[i]] - y[v3[i]]));
                cout << fixed << setprecision(12) << i << " lato 3: " << lato3 << endl;
            }

    }

    delete[] id;
    delete[] x;
    delete[] y;

    delete[] v1;
    delete[] v2;
    delete[] v3;

    return 0;
}


