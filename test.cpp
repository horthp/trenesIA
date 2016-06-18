#include <cstdlib>
#include <iostream>
#include <cstring>
//#include <cmath>
#include <fstream>
//#include <ctime>
#include <vector> 
#include <sstream>
//#include "recursos.h"
#include <cstdlib>

using namespace std;

typedef struct depature_matching{
	string asigned_train;
	string id_train;
}depMatch;




//funcion que lee los datos de las instancias y los guarda en vector de vectores
//recibe el nombre del archivo y las columnas de los archivos
vector< vector<string> > loadData(const char* filename, int value) {
    ifstream file(filename);
    vector< vector<string> > data;
    vector<string> row(value);
    string column, tmp;
    int cont = 0;
    if(file.is_open()) {
        while (getline(file, column, ';')) {
            if(cont == 0) { column.erase(0, 1); }
            row.push_back(column);
            cont++;
            if(cont == value) {
                data.push_back(row);
                row.clear();
                cont = 0;
            }
        }
        file.close();
    }
    return data;
}


void matching_depature(vector< vector<string> > depature){//, vector< vector<string> > initialTrains, vector< vector<string> >, trainCategories vector< vector<string> > compCatDep){
	cout<< depature[1][0] << endl;
	//Si se usa un tren originalmente en el sistema
		//-> Enter system at d1 00:00:00 
		//-> Enter resource

	//Si se usa un tren que viene de un arrival
		//-> Enter resource
		//-> Exit resource


}

void platform_assignment(){

}

void arrival_depature_sequence_assigner(){
	
}

int main(int argc, char **argv){

	string instance = "myInstance";
    vector< vector<string> > arrDepSequencesData, arrivalsData, compCatDepData, compCatResData, departuresData, facilitiesData, gatesData, 
    imposedConsumptionsData, initialTrainsData, jointArrDepData, parametersData, platformsData, prefPlatData, reusesData, singleTracksData, 
    trackGroupsData, trainCategoriesData, yardsData;

	arrDepSequencesData = loadData((instance + "/arrDepSequences.csv").c_str(), 3);
    arrivalsData = loadData((instance + "/arrivals.csv").c_str(), 11);
	compCatDepData = loadData((instance + "/compCatDep.csv").c_str(), 2);
	compCatResData = loadData((instance + "/compCatRes.csv").c_str(), 2);
	departuresData = loadData((instance + "/departures.csv").c_str(), 8);
	facilitiesData = loadData((instance + "/facilities.csv").c_str(), 3);
	gatesData = loadData((instance + "/gates.csv").c_str(), 6);
	imposedConsumptionsData = loadData((instance + "/imposedConsumptions.csv").c_str(), 7);
	initialTrainsData = loadData((instance + "/initialTrains.csv").c_str(), 5);
	parametersData = loadData((instance + "/parameters.csv").c_str(), 2);
	platformsData = loadData((instance + "/platforms.csv").c_str(), 2);
	prefPlatData = loadData((instance + "/prefPlat.csv").c_str(), 2);
	reusesData = loadData((instance + "/reuses.csv").c_str(), 2);
	singleTracksData = loadData((instance + "/singleTracks.csv").c_str(), 3);
	trackGroupsData = loadData((instance + "/trackGroups.csv").c_str(), 3);
	trainCategoriesData = loadData((instance + "/trainCategories.csv").c_str(), 7);
	yardsData = loadData((instance + "/yards.csv").c_str(), 2);	

    //cout<< initialTrainsData[1][4] << endl;
	matching_depature(departuresData);
	return 0;
}