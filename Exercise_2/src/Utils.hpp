#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary{

bool letturaMesh(const string &percorso, PolygonalMesh &mesh);

bool letturaDatiFileCell0Ds(const string &percorsoFileCell0Ds, PolygonalMesh &mesh);

bool letturaDatiFileCell1Ds(const string &percorsoFileCell1Ds, PolygonalMesh &mesh);

bool letturaDatiFileCell2Ds(const string &percorsoFileCell2Ds, PolygonalMesh &mesh);

}

