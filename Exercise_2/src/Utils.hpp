#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary{

bool letturaMesh(const string &percorso, PolygonalMesh &mesh);

bool letturaDatiFileCell0Ds(const string &percorsoFileCell0Ds, PolygonalMesh &mesh);

bool letturaDatiFileCell1Ds(const string &percorsoFileCell1Ds, PolygonalMesh &mesh);

bool letturaDatiFileCell2Ds(const string &percorsoFileCell2Ds, PolygonalMesh &mesh);

bool datiFileCell0Ds(const string& percorsoCell0Ds, unsigned int*& id, double*& x, double*& y, const unsigned int numeroRigheCell0Ds);

bool datiFileCell2Ds(const string& percorsoCell2Ds, unsigned int& nVertices, unsigned int& nEdges, Vector3i& vertices, Vector3i& edges);

}

