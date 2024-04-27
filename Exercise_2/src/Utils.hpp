#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary{

// Importa la mesh poligonale e verifica se la mesh è corretta
// mesh: una struct PolygonalMesh
// restituisce il risultato della lettura, vero se ha successo falso altrimenti
bool letturaMesh(const string &percorso, PolygonalMesh &mesh);

// Importa le proprietà della Cell0Ds dal file Cell0Ds.csv
// mesh: una struct PolygonalMesh
// restituisce il risultato della lettura, vero se ha successo falso altrimenti
bool letturaDatiFileCell0Ds(const string &percorsoFileCell0Ds, PolygonalMesh &mesh);

// Importa le proprietà della Cell1Ds dal file Cell1Ds.csv
// mesh: una struct PolygonalMesh
// restituisce il risultato della lettura, vero se ha successo falso altrimenti
bool letturaDatiFileCell1Ds(const string &percorsoFileCell1Ds, PolygonalMesh &mesh);

// Importa le proprietà della Cell2Ds dal file Cell2Ds.csv
// mesh: una struct PolygonalMesh
// restituisce il risultato della lettura, vero se ha successo falso altrimenti
bool letturaDatiFileCell2Ds(const string &percorsoFileCell2Ds, PolygonalMesh &mesh);

}

