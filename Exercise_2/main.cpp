#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;

int main(){

    PolygonalMesh mesh;

    string percorso = "PolygonalMesh";

    if(!letturaMesh(percorso, mesh)){
       return 1;
    }

    // string fileCell0Ds = "PolygonalMesh/Cell0Ds";

    return 0;
}


