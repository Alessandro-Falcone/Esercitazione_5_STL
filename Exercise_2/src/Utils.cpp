#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

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
                        cerr << "Mesh sbagliata" << endl;
                        return 2;
                    }

                    auto findEnd = find(mesh.VerticesCell2Ds[c].begin(), mesh.VerticesCell2Ds[c].end(), end);
                    if(findEnd == mesh.VerticesCell2Ds[c].end()){
                        cerr << "Mesh sbagliata" << endl;
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
                    // if(mesh.MarkersCell0Ds.find(marker) == mesh.MarkersCell0Ds.end())
                    //     mesh.MarkersCell0Ds.insert({marker, {id}});
                    // else
                    //     mesh.MarkersCell0Ds[marker].push_back(id);
                    auto ret = mesh.MarkersCell0Ds.insert({marker, {id}});
                    if(!ret.second)
                        (ret.first) -> second.push_back(id);
                }
            }

            fileCell0Ds.close();
            return true;
        }
    }

    bool letturaDatiFileCell1Ds(const string &percorsoFileCell1Ds, PolygonalMesh &mesh){

        ifstream fileCell1Ds;
        fileCell1Ds.open(percorsoFileCell1Ds);

        if(fileCell1Ds.fail()){
            return false;
        }else{

            list<string> listLines;
            string line;
            while(getline(fileCell1Ds, line)){
                listLines.push_back(line);
            }

            fileCell1Ds.close();

            listLines.pop_front();
            mesh.NumberCell1Ds = listLines.size();
            if(mesh.NumberCell1Ds == 0){
                cerr << "Non ci sono cell 1Ds" << endl;
                return false;
            }

            mesh.IdCell1Ds.reserve(mesh.NumberCell1Ds);
            mesh.VerticesCell1Ds.reserve(mesh.NumberCell1Ds);

            for(const string& line : listLines){

                istringstream converter(line);
                unsigned int id;
                unsigned int marker;
                string riga;
                Vector2i vertices;

                getline(converter,riga,';');
                id = stoi(riga);

                getline(converter,riga,';');
                marker = stoi(riga);

                getline(converter,riga,';');
                vertices(0) = stoi(riga);

                getline(converter,riga,';');
                vertices(1) = stoi(riga);

                mesh.IdCell1Ds.push_back(id);
                mesh.VerticesCell1Ds.push_back(vertices);

                if(marker != 0){
                    // if(mesh.MarkersCell1Ds.find(marker) == mesh.MarkersCell1Ds.end()){
                    //     mesh.MarkersCell1Ds.insert({marker, {id}});
                    // }else{
                    //     mesh.MarkersCell1Ds[marker].push_back(id);
                    // }
                    auto ret = mesh.MarkersCell1Ds.insert({marker, {id}});
                    if(!ret.second)
                        (ret.first) -> second.push_back(id);
                }
            }

            fileCell1Ds.close();
            return true;
        }
    }

    bool letturaDatiFileCell2Ds(const string &percorsoFileCell2Ds, PolygonalMesh &mesh){

        ifstream fileCell2Ds;
        fileCell2Ds.open(percorsoFileCell2Ds);

        if(fileCell2Ds.fail()){
            return false;
        }else{

            list<string> listLines;
            string line;
            while(getline(fileCell2Ds, line)){
                listLines.push_back(line);
            }

            fileCell2Ds.close();

            listLines.pop_front();

            mesh.NumberCell2Ds = listLines.size();

            if(mesh.NumberCell2Ds == 0){
                cerr << "Non ci sono cell 2D" << endl;
                return false;
            }

            mesh.IdCell2Ds.reserve(mesh.NumberCell2Ds);
            mesh.VerticesCell2Ds.reserve(mesh.NumberCell2Ds);
            mesh.EdgesCell2Ds.reserve(mesh.NumberCell2Ds);

            for(const string& line : listLines){

                istringstream converter(line);
                string riga;
                unsigned int id;
                unsigned int marker;
                unsigned int nVertices;
                unsigned int nEdges;
                vector<unsigned int> vertices;
                vector<unsigned int> edges;

                getline(converter,riga,';');
                id = stoi(riga);

                getline(converter,riga,';');
                marker = stoi(riga);

                getline(converter,riga,';');
                nVertices = stoi(riga);

                for(unsigned int i = 0; i < nVertices; i++){
                    getline(converter, riga, ';');
                    vertices.push_back(stoi(riga));
                }

                getline(converter, riga, ';');
                nEdges = stoi(riga);

                for(unsigned int i = 0; i < nEdges; i++){
                    getline(converter, riga, ';');
                    edges.push_back(stoi(riga));
                }

                if(nVertices == 3 && nEdges == 3){
                    cout << "\ntriangolo alla riga: " << id << endl;
                    cout << "vertici: ";
                }

                // if(mesh.NumberCell2Ds != 0){
                //    cout << line << " ";
                // }

                for(unsigned int i = 0; i < nVertices; i++){
                    if(nVertices == 3 && nEdges == 3){
                        cout << vertices[i] << " ";
                    }
                }

                if(nVertices == 3 && nEdges == 3){
                    cout << "lati: ";
                }

                for(unsigned int i = 0; i < nEdges; i++){
                    if(nVertices == 3 && nEdges == 3){
                        cout << edges[i] << " ";
                    }
                }
                // cout << "\n";

                mesh.IdCell2Ds.push_back(id);
                mesh.VerticesCell2Ds.push_back(vertices);
                mesh.EdgesCell2Ds.push_back(edges);

                if(marker != 0){
                    // if(mesh.MarkersCell2Ds.find(marker) == mesh.MarkersCell2Ds.end())
                    //     mesh.MarkersCell2Ds.insert({marker, {id}});
                    // else
                    //     mesh.MarkersCell2Ds[marker].push_back(id);
                    auto ret = mesh.MarkersCell2Ds.insert({marker, {id}});
                    if(!ret.second)
                        (ret.first) -> second.push_back(id);
                }
            }
            cout << endl;
            fileCell2Ds.close();
            return true;
        }
    }
    bool datiFileCell0Ds(const string& percorsoCell0Ds, unsigned int*& id, double*& x, double*& y, const unsigned int numeroRigheCell0Ds){

        ifstream Cell0Ds;
        Cell0Ds.open(percorsoCell0Ds);

        if(Cell0Ds.fail()){
            return false;
        }else{
            string line;

            id = new unsigned int[numeroRigheCell0Ds];
            x = new double[numeroRigheCell0Ds];
            y = new double[numeroRigheCell0Ds];
            int marker;

            getline(Cell0Ds, line, ';');
            getline(Cell0Ds, line, ';');
            getline(Cell0Ds, line, ';');
            getline(Cell0Ds, line);

            for(unsigned int i = 0; i < numeroRigheCell0Ds; i++){
                getline(Cell0Ds, line, ';');
                id[i] = stoi(line);
                // cout << id[i] << " ";

                getline(Cell0Ds, line, ';');
                marker = stoi(line);
                cout << marker << " ";

                getline(Cell0Ds, line, ';');
                x[i] = stod(line);
                // cout << fixed << scientific << x[i] << " ";

                getline(Cell0Ds, line);
                y[i] = stod(line);
                // cout << fixed << scientific << y[i] << endl;
            }

            cout << endl;
            Cell0Ds.close();
            return true;
        }
    }
}

