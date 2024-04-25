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

int main(){

    PolygonalMesh mesh;

    string percorso = "PolygonalMesh";

    if(!letturaMesh(percorso, mesh)){
       cerr << "Errore: impossibile leggere la mesh" << endl;
       return 1;
    }

    return 0;
}


