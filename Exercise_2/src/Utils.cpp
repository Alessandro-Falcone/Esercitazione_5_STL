#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace PolygonalLibrary{

    bool letturaMesh(const string &percorso, PolygonalMesh &mesh){

        string fileCell0Ds = percorso + "/Cell0Ds.csv";
        if(!letturaDatiFileCell0Ds(fileCell0Ds, mesh)){
            return false;
        }else{
            cout << "Cell0Ds markers: " << endl;
            for(auto it = mesh.MarkersCell0Ds.begin(); it != mesh.MarkersCell0Ds.end(); it++){

                cout << "key: " << it -> first << " values:";
                for(const unsigned int id : it -> second){
                    cout << " " << id;
                }
                cout << endl;
            }
        }

        string fileCell1Ds = percorso + "/Cell1Ds.csv";
        if(!letturaDatiFileCell1Ds(fileCell1Ds, mesh)){
            return false;
        }else{
            cout << "Cell1D marker:" << endl;
            for(auto it = mesh.MarkersCell1Ds.begin(); it != mesh.MarkersCell1Ds.end(); it++){
                cout << "key: " << it -> first << " values:";
                for(const unsigned int id : it -> second){
                    cout << " " << id;
                }
                cout << endl;
            }
        }

        string fileCell2Ds = percorso + "/Cell2Ds.csv";
        if(!letturaDatiFileCell2Ds(fileCell2Ds, mesh)){
            return false;
        }else{

        // Test:
        for(unsigned int c = 0; c < mesh.NumberCell2Ds; c++){
            vector<unsigned int> edges = mesh.EdgesCell2Ds[c];

            for(unsigned int e = 0; e < edges.size(); e++){

                const unsigned int origin = mesh.VerticesCell1Ds[edges[e]][0];
                const unsigned int end = mesh.VerticesCell1Ds[edges[e]][1];

                auto findOrigin = find(mesh.VerticesCell2Ds[c].begin(), mesh.VerticesCell2Ds[c].end(), origin);
                if(findOrigin == mesh.VerticesCell2Ds[c].end()){
                    cerr << "Wrong mesh" << endl;
                    return 2;
                }

                auto findEnd = find(mesh.VerticesCell2Ds[c].begin(), mesh.VerticesCell2Ds[c].end(), end);
                if(findEnd == mesh.VerticesCell2Ds[c].end()){
                    cerr << "Wrong mesh" << endl;
                    return 3;
                }
            }
        }
    }
    return true;
    }

    bool letturaDatiFileCell0Ds(const string &percorsoFileCell0Ds, PolygonalMesh &mesh){


            ifstream fileCell0Ds;
            fileCell0Ds.open(percorsoFileCell0Ds);

            if(fileCell0Ds.fail()){
                return false;
            }else{

            list<string> listLines;
            string line;
            while(getline(fileCell0Ds, line)){
                listLines.push_back(line);
            }
            fileCell0Ds.close();

            listLines.pop_front();
            mesh.NumberCell0Ds = listLines.size();
            if(mesh.NumberCell0Ds == 0){
                cerr << "Non ci sono cell 0Ds" << endl;
                return false;
            }

            mesh.IdCell0Ds.reserve(mesh.NumberCell0Ds);
            mesh.CoordinatesCell0Ds.reserve(mesh.NumberCell0Ds);


            for(string &line : listLines){

                istringstream converter(line);
                unsigned int id;
                unsigned int marker;
                string riga;
                Vector2d coord;

                getline(converter,riga,';');
                id = stoi(riga);

                getline(converter,riga,';');
                marker = stoi(riga);

                getline(converter,riga,';');
                coord(0) = stod(riga);

                getline(converter,riga,';');
                coord(1) = stod(riga);

                mesh.IdCell0Ds.push_back(id);
                mesh.CoordinatesCell0Ds.push_back(coord);

                if(marker != 0){
                    if(mesh.MarkersCell0Ds.find(marker) == mesh.MarkersCell0Ds.end())
                        mesh.MarkersCell0Ds.insert({marker, {id}});
                    else
                        mesh.MarkersCell0Ds[marker].push_back(id);
                }
            }
            return true;
        }
    }

    bool letturaDatiFileCell1Ds(const string &percorsoFileCell1Ds, PolygonalMesh &mesh){

        ifstream fileCell1Ds;
        fileCell1Ds.open(percorsoFileCell1Ds);

        if(fileCell1Ds.fail()){
            return false;
        }else{
            return true;
        }
    }

    bool letturaDatiFileCell2Ds(const string &percorsoFileCell2Ds, PolygonalMesh &mesh){

        ifstream fileCell2Ds;
        fileCell2Ds.open(percorsoFileCell2Ds);

        if(fileCell2Ds.fail()){
            return false;
        }else{
            return true;
        }
    }
}

