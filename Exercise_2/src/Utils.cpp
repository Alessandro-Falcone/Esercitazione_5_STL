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

            cout << "Cell0Ds markers:" << endl;
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

            cout << "Cell1Ds markers:" << endl;
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

            // cout << "Cell2Ds markers:" << endl;
            // for(auto it = mesh.MarkersCell2Ds.begin(); it != mesh.MarkersCell2Ds.end(); it++){
            //     cout << "key: " << it -> first << " values:";
            //     for(const unsigned int id : it -> second){
            //         cout << " " << id;
            //     }
            //     cout << endl;
            // }

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

            // double diffx = 0, diffy = 0, lunghezzaLato = 0;
            unsigned int latiPoligoni = 0, distanzaPunti = 0, areaNonNulla = 0;
            double tolL = 1e-4, tolA = 1e-4, tolLato = 0, tolArea = 0;
            double tol = 1e+10 * numeric_limits<double>::epsilon();

            if(tolL > tol){
                tolLato = tolL;
                cout << "tolleranza lato: " << tolLato << endl;
            }else{
                tolLato = tol;
                cout << "tolleranza: " << tolLato << endl;
            }

            if(tolA > tol){
                tolArea = tolA;
                cout << "tolleranza area: " << tolArea << endl;
            }else{
                tolArea = tol;
                cout << "tolleranza: " << tolArea << endl;
            }

            // for(unsigned int i = 0; i < mesh.NumberCell1Ds; i++){

            //     Vector2i origine = mesh.VerticesCell1Ds[i];
            //     diffx = abs(mesh.CoordinatesCell0Ds[origine[0]][0] - mesh.CoordinatesCell0Ds[origine[1]][0]);
            //     diffy = abs(mesh.CoordinatesCell0Ds[origine[0]][1] - mesh.CoordinatesCell0Ds[origine[1]][1]);

            //     lunghezzaLato = sqrt(diffx*diffx + diffy*diffy);
            //     // if(lunghezzaLato < tolLato){
            //     // cout << origine[0] << " " << origine[1] << " distanza x: " << fixed << setprecision(9) << diffx
            //        // << " distanza y: "  << fixed << setprecision(9) << diffy << " lunghezza lato: " << lunghezzaLato << endl;
            //     // }
            // }


            for(unsigned int i = 0; i < mesh.NumberCell2Ds; i++){

                vector<unsigned int> vertices = mesh.VerticesCell2Ds[i];
                latiPoligoni += vertices.size();

                // Test lati:

                double diffx = 0, diffy = 0, lunghezzaLato = 0;

                for(unsigned int j = 0; j < vertices.size(); j++){

                    if(j < vertices.size() - 1){

                        diffx = abs(mesh.CoordinatesCell0Ds[vertices[j]][0] - mesh.CoordinatesCell0Ds[vertices[j+1]][0]);
                        diffy = abs(mesh.CoordinatesCell0Ds[vertices[j]][1] - mesh.CoordinatesCell0Ds[vertices[j+1]][1]);
                        lunghezzaLato = sqrt(diffx*diffx + diffy*diffy);
                        if(lunghezzaLato < tolLato){
                            cout << "Lato poligono circa uguale a zero: " << "vertici " << vertices[j] << " " << vertices[j+1]
                                 << ", riga file Cell2Ds " << i+2 << ", lunghezza: "
                                 << fixed << setprecision(9) << lunghezzaLato << endl;
                        }

                    }else{

                        diffx = abs(mesh.CoordinatesCell0Ds[vertices[j]][0] - mesh.CoordinatesCell0Ds[vertices[vertices.size()-j-1]][0]);
                        diffy = abs(mesh.CoordinatesCell0Ds[vertices[j]][1] - mesh.CoordinatesCell0Ds[vertices[vertices.size()-j-1]][1]);
                        lunghezzaLato = sqrt(diffx*diffx + diffy*diffy);
                        if(lunghezzaLato < tolLato){
                            cout << "Lato poligono circa uguale a zero: " << "vertici " << vertices[j] << " " << vertices[j+1]
                                 << ", riga file Cell2Ds " << i+2 << ", lunghezza: "
                                 << fixed << setprecision(9) << lunghezzaLato << endl;
                        }
                    }

                    if(lunghezzaLato > tolLato){
                        distanzaPunti++;
                    }
                }

                // Test area poligoni:

                double x0 = 0, x1 = 0, y0 = 0, y1 = 0;
                double sommatoria = 0, area = 0;

                for(unsigned int j = 0; j < vertices.size(); j++){

                    if(j < vertices.size() - 1){

                        x0 = mesh.CoordinatesCell0Ds[vertices[j]][0];
                        y1 = mesh.CoordinatesCell0Ds[vertices[j+1]][1];
                        x1 = mesh.CoordinatesCell0Ds[vertices[j+1]][0];
                        y0 = mesh.CoordinatesCell0Ds[vertices[j]][1];
                        sommatoria += (x0*y1) - (x1*y0);

                    }else{

                        x0 = mesh.CoordinatesCell0Ds[vertices[j]][0];
                        y1 = mesh.CoordinatesCell0Ds[vertices[vertices.size()-j-1]][1];
                        x1 = mesh.CoordinatesCell0Ds[vertices[vertices.size()-j-1]][0];
                        y0 = mesh.CoordinatesCell0Ds[vertices[j]][1];
                        sommatoria += (x0*y1) - (x1*y0);
                    }

                    // distanza = mesh.CoordinatesCell0Ds[vertices[j]][0]*mesh.CoordinatesCell0Ds[vertices[j+1]][1];
                    // cout << vertices[j] << " x0: " << mesh.CoordinatesCell0Ds[vertices[j]][0] << " " << vertices[j+1] << " y1: " << mesh.CoordinatesCell0Ds[vertices[j+1]][1]
                    //     << " prodotto: "<< mesh.CoordinatesCell0Ds[vertices[j]][0]*mesh.CoordinatesCell0Ds[vertices[j+1]][1] << endl;
                    // cout << vertices[j] << " x0: " << x0 << " " << vertices[j+1] << " y1: " << y1 << " "
                    //      << vertices[j+1] << " x1: " << x1 << " " << vertices[j] << " y0: " << y0 << endl;
                }

                area = 1./2. * abs(sommatoria);

                if(area > tolArea){
                    areaNonNulla++;
                }else{
                    cout << fixed << setprecision(9) << "area circa uguale a zero: "<< area
                         << " riga file Cell2Ds " << i+2 << endl;
                }
            }

            if(distanzaPunti == latiPoligoni && areaNonNulla == mesh.NumberCell2Ds){
                cout << "Test lunghezza dei lati diversa da zero superato" << endl;
                cout << "Test area dei poligoni diversa da zero superato" << endl;
            }else{
                cout << "Test lunghezza dei lati diversa da zero potrebbe non essere stato superato" << endl;
                cout << "Lati che hanno lunghezza diversa da zero: " << distanzaPunti << " su " << mesh.NumberCell2Ds << " poligoli totali" << endl;
                cout << "Test area dei poligoni diversa da zero potrebbe non essere stato superato" << endl;
                cout << "Aree dei poligoni diverse da zero: " << areaNonNulla << " su " << mesh.NumberCell2Ds << " aree totali" << endl;
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
                string riga;
                unsigned int id, marker;
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
                string riga;
                unsigned int id, marker;
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
                unsigned int id, marker, nVertices, nEdges;
                vector<unsigned int> vertices, edges;

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

                // if(mesh.NumberCell2Ds != 0){
                //     cout << line << " " << endl;
                // }

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

            fileCell2Ds.close();
            return true;
        }
    }
}

