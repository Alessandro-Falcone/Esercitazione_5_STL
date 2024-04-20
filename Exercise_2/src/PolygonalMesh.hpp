#pragma once

#include <iostream>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;


namespace PolygonalLibrary{

    struct PolygonalMesh{

        unsigned int NumberCell0Ds = 0;
        vector<unsigned int> IdCell0Ds = {};
        vector<Vector2d> CoordinatesCell0Ds = {}; // meglio accesso randomico in funzione della posizione
        map<unsigned int, list<unsigned int>> MarkersCell0Ds = {};
        // vertici e lati che hanno quel marker

        unsigned int NumberCell1Ds = 0;
        vector<unsigned int> IdCell1Ds = {};
        vector<Vector2i> VerticesCell1Ds = {}; //dimensione fissa vector<array<unsigned int, 2>> VerticesCell1Ds = {};
        map<unsigned int, list<unsigned int>> MarkersCell1Ds = {};

        unsigned int NumberCell2Ds = 0;
        vector<unsigned int> IdCell2Ds = {};
        vector<vector<unsigned int>> VerticesCell2Ds = {};
        vector<vector<unsigned int>> EdgesCell2Ds = {};
        map<unsigned int, list<unsigned int>> MarkersCell2Ds = {};

        // double tol = 10 * numeric_limits<double>::epsilon(); // in base posizione cambia la tolleranza
        // 3 tolleranze default specificato dall'utente e 1D
        // metodo della classe cambia dagli attributi in questo caso no

        // map<unsigned int, list<unsigned int>> VerticesMarker;
        // map<unsigned int, list<unsigned int>> EdgesMarker;

    };
}

