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

            cout << "Cell0Ds markers:" << endl; // output markers file Cell0Ds.csv
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

            cout << "\nCell1Ds markers:" << endl; // output markers file Cell1Ds.csv
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

            // 3 contatori che mi servono per dire se il Test lati poligoni e il Test area poligoni sono stati superati
            unsigned int latiPoligoni = 0, distanzaPunti = 0, areaNonNulla = 0;

            double tolLato = 1e-6, tolArea = 1e-6; // tolleranze lunghezza lati e area
            double tol = numeric_limits<double>::epsilon(); // tolleranza definita con la epsilon di macchina

            if(tolLato > tol){
                tolLato = tolLato; // se è soddisfatta la condizione del if prendo come tolleranza del lato la tolleranza del lato definita dall'utente
            }else{
                tolLato = tol; // se non è soddisfatta la condizione del if prendo come tolleranza del lato la tolleranza definita con la epsilon di macchina
            }

            if(tolArea > tol){
                tolArea = tolArea; // se è soddisfatta la condizione del if prendo come tolleranza dell'area la tolleranza dell'area definita dall'utente
            }else{
                tolArea = tol; // se è soddisfatta la condizione del if prendo come tolleranza dell'area la tolleranza dell'area definita dall'utente
            }

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

            cout << "\nTest: i lati dei poligoni hanno lunghezza diversa da zero" << endl;
            // Test lati poligoni: i lati dei poligoni hanno lunghezza diversa da zero

            for(unsigned int l = 0; l < mesh.NumberCell2Ds; l++){

                vector<unsigned int> edges = mesh.EdgesCell2Ds[l];

                // sommo fino ad avere il totale del numero di lati dei poligoni presenti all'interno del file Cell2Ds.csv,
                // mi servirà per dire se il Test lati poligoni è stato superato oppure no
                latiPoligoni += edges.size();

                double diffx = 0, diffy = 0; // variabili double in cui salvo la differenza delle x (diffx) e delle y (diffy)
                double lunghezzaLato = 0; // variabile double in cui salvo la lunghezza dei lati

                for(unsigned int e = 0; e < edges.size(); e++){

                    const unsigned int origin = mesh.VerticesCell1Ds[edges[e]][0];
                    const unsigned int end = mesh.VerticesCell1Ds[edges[e]][1];

                    diffx = (mesh.CoordinatesCell0Ds[origin][0] - mesh.CoordinatesCell0Ds[end][0]);
                    diffy = (mesh.CoordinatesCell0Ds[origin][1] - mesh.CoordinatesCell0Ds[end][1]);
                    lunghezzaLato = sqrt(pow(diffx,2) + pow(diffy,2)); // formula per calcolare la distanza tra due punti

                    if(lunghezzaLato > tolLato){
                        // conto quanti sono i lati dei poligoni diversi da zero,
                        // mi servirà per dire se il Test lati poligoni è stato superato oppure no
                        distanzaPunti++;

                    }else{
                        // se non è soddisfatta la condizione dell'if vado a stampare quale lato del poligono che sto analizzando
                        // ha una lunghezza che per la tolleranza presa in considerazione risulta essere uguale a zero

                        cout << "Lato poligono uguale a zero per la tolleranza inserita: riga file Cell2Ds " << l+2
                             << ", vertici " << origin << " " << end
                             << " (lunghezza: " << fixed << setprecision(9) << lunghezzaLato << ")" << endl;
                    }
                }
            }

            if(distanzaPunti == latiPoligoni){ // contatori uguali Test lati poligoni superato
                cout << "Test superato" << endl;
            }else{

                // condizione dell'if non soddisfatta quindi contatori non uguali Test lati poligoni non superato
                cout << "Test non superato" << endl;
                // stampo quanti lati dei poligoni hanno lunghezza diversa da zero sul numero totale
                cout << "Lati che hanno lunghezza diversa da zero: " << distanzaPunti << " su " << latiPoligoni << " lati totali" << endl;
            }

            cout << "\nTest: l'area dei poligoni e' diversa da zero" << endl;
            // Test area poligoni: l'area dei poligoni è diversa da zero
            // formula area = 1/2 * abs(sommatoria((x0*y1) - (x1*y0)))

            for(unsigned int a = 0; a < mesh.NumberCell2Ds; a++){

                vector<unsigned int> vertices = mesh.VerticesCell2Ds[a];

                double x0 = 0, x1 = 0, y0 = 0, y1 = 0; // variabili double in cui salvo le coordinate dei punti
                double sommatoria = 0; // variabile double in cui salvo la sommatoria data da abs(sommatoria((x0*y1) - (x1*y0)))
                double area = 0; // variabile double in cui salvo l'area dei poligoni data da area = 1/2 * abs(sommatoria((x0*y1) - (x1*y0)))

                for(unsigned int i = 0; i < vertices.size(); i++){

                    if(i < vertices.size() - 1){
                        // se è soddisfatta la condizione dell'if salvo le coordinate dei punti per i = 0, 1, 2,...,vertices.size() - 2
                        // nelle variabili double x0, y0, x1, y1 e calcolo la sommatoria
                        x0 = mesh.CoordinatesCell0Ds[vertices[i]][0];
                        y1 = mesh.CoordinatesCell0Ds[vertices[i+1]][1];
                        x1 = mesh.CoordinatesCell0Ds[vertices[i+1]][0];
                        y0 = mesh.CoordinatesCell0Ds[vertices[i]][1];
                        sommatoria += (x0*y1) - (x1*y0);

                    }else{
                        // se non è soddisfatta la condizione dell'if salvo le coordinate del punto non considerato con i = vertices.size() - 1
                        // e del primo punto con i = 0 nelle variabili double x0, y0, x1, y1 e continuo con la sommatoria riferita allo stesso poligono
                        // e dopo proseguo con il poligono successivo e così via
                        x0 = mesh.CoordinatesCell0Ds[vertices[i]][0];
                        y1 = mesh.CoordinatesCell0Ds[vertices[vertices.size()-i-1]][1];
                        x1 = mesh.CoordinatesCell0Ds[vertices[vertices.size()-i-1]][0];
                        y0 = mesh.CoordinatesCell0Ds[vertices[i]][1];
                        sommatoria += (x0*y1) - (x1*y0);
                    }
                }

                area = 1./2. * abs(sommatoria); // calcolo l'area di ogni poligono

                if(area > tolArea){
                    // conto quante sono le aree dei poligoni diverse da zero,
                    // mi servirà per dire se il Test area poligoni è stato superato oppure no
                    areaNonNulla++;

                }else{
                    // se non è soddisfatta la condizione dell'if vado a stampare l'area del poligono che sto analizzando
                    // ha area che per la tolleranza presa in considerazione risulta essere uguale a zero
                    cout << fixed << setprecision(9) << "Area uguale a zero per la tolleranza inserita: riga file Cell2Ds " << a+2
                         << " (area: " << area << ")" << endl;
                }
            }

            if(areaNonNulla == mesh.NumberCell2Ds){ // contatore uguale al numero di righe del file Cell2Ds.csv Test area poligoni superato
                cout << "Test superato" << endl;
            }else{
                // condizione dell'if non soddisfatta quindi contatore non uguale al numero di righe del file Cell2Ds.csv Test area poligoni non superato
                cout << "Test non superato" << endl;
                // stampo quante aree dei poligoni sono diverse da zero sul numero totale
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
                cerr << "Non ci sono cell 2Ds" << endl;
                return false;
            }

            mesh.IdCell2Ds.reserve(mesh.NumberCell2Ds);
            mesh.VerticesCell2Ds.reserve(mesh.NumberCell2Ds);
            mesh.EdgesCell2Ds.reserve(mesh.NumberCell2Ds);
            unsigned int markerZero2Ds = 0; // contatore che conta quanti marker uguali a zero ci sono nel file Cell2Ds.csv

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
                }else{
                    markerZero2Ds++;
                }
            }

            if(markerZero2Ds == mesh.NumberCell2Ds){
                cout << "\nCell2Ds markers: " << endl;
                cout << "Tutti i markers nel file Cell2Ds.csv sono uguali a zero" << endl;
            }

            fileCell2Ds.close();
            return true;
        }
    }
}

