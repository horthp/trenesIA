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

typedef struct solution{
	string train;
	string time;
	string event_type;
	string resource;
	string gate;
	string Complement;
}sol;

typedef struct depature_matching{
	string depature;
	//int reqDBM;
	string train;
	//bool arrival;
	string arrival;
	int cost;
	//bool initial_train;
	//string train_category;
	//string train_actual_resource;
}depMatch;

typedef struct time{
	int hh;
	int mm;
	int ss;
	double totalmin;
}ttime;


typedef struct date{
	int day;
	int hh;
	int mm;
	int ss;
	double totalmin;
}date;


date get_date(string v){
	date d;
	string aux;
	aux= v.substr(1,2);
	d.day = atoi(aux.c_str());
	aux = v.substr(3,2);
	d.hh = atoi(aux.c_str());
	aux = v.substr(6,2);
	d.mm = atoi(aux.c_str());
	aux = v.substr(9,2);
	d.ss = atoi(aux.c_str());
	d.totalmin =60*d.hh + d.mm + d.ss/60; 
	return d;
}

ttime get_time(string v){
	ttime t;
	string aux;
	aux = v.substr(0,2);
	t.hh = atoi(aux.c_str());
	aux = v.substr(3,2);
	t.mm = atoi(aux.c_str());
	aux = v.substr(5,2);
	t.ss = atoi(aux.c_str());
	t.totalmin =60*t.hh + t.mm + t.ss/60; 
	return t;
}


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

// Se divide el problema en varios subproblemas.
/*
void matching_depature(vector< vector<string> > depature, vector< vector<string> > compCatDep, vector< vector<string> > initialTrains ){//, , vector< vector<string> > trainCategories, vector< vector<string> > compCatDep,  vector< vector<string> > reuses){
	
	int j = 1;
	int k;
	depMatch match_aux;
	vector<depMatch> match;
		for(size_t i=1;i < depature.size();i++){
			//cout<< "for!"<<i<<endl;
			//cout<< compCatDep[0][49] <<endl;
			//cout << compCatDep[48][0].empty()<<"-"<< endl;
			
			//Se guarda la compatibilidad entre salida y categoria de tren.
			while(depature[i][0].compare(compCatDep[j][0]) == 0 && j < compCatDep.size() - 1) {
				match_aux.depature = compCatDep[j][0];
				match_aux.train_category = compCatDep[j][1];
				match_aux.reqDBM = atoi(depature[i][6].c_str());
				match.push_back(match_aux);

				if(!(compCatDep[j+1][0].empty())){
					j++;
				}

			}
			if(j == compCatDep.size() - 1){
				match_aux.depature = compCatDep[j][0];
				match_aux.train_category = compCatDep[j][1];
				match.push_back(match_aux);
				match_aux.reqDBM = atoi(depature[i][6].c_str());
			}
		}

		j=1;
		for(size_t i = 0; i < match.size(); i++){
			//
			//cout<<match[i].train_category.compare(initialTrains[j][1])<<endl;
			while(match[i].train_category.compare(initialTrains[j][1]) == 0 && j < initialTrains.size() - 1){
				cout<<"while"<<j<<endl;
				if(atoi(initialTrains[j][3].c_str()) >= match[i].reqDBM){
					match[i].trains.push_back(initialTrains[j][0]);
					
				}
				j++;
			}
			//cout<<"largo trains"<< match[i].trains.size()<<endl;
			//cout<<"salida "<<match[i].depature << " Es compatible con un tren de categoria "<< match[i].train_category<< "||reqDBM||"<<match[i].reqDBM<<endl;
			
			/*for(size_t k = 0; match[i].trains.size();k++){
				cout<<"posibles trenes"<<endl;
				cout<<match[i].trains[k]<<"->";}  */
		//} 
//}	

void forward_checking_depature(vector< vector<string> > depature, vector< vector<string> > arrival, vector< vector<string> > init_trains, vector< vector<string> > parameters, int c){
	//:D  Se seleciona un tren de los arrivals
	int cost = c;
	int difd = 0;
	int difa = 0;
	vector <depMatch> match;
	depMatch match_aux;
	ttime dept, arrt, restime;
	date depd, arrd;	
	restime = get_time(parameters[12][1]);
	cout<<":)"<<endl;
	for(size_t i=1; i < depature.size(); i++){
		//cout<<"primer for- iteracion "<<i<<endl;
		depd = get_date(depature[i][1]);
		for(size_t j=1; j< arrival.size();j++){
			//cout<<"segundo for- iteracion "<<j<<endl;
			arrd = get_date(arrival[j][2]);
			/*Se verifica que la llegada con la salida pertenescan al mismo dia 
			y que el tiempo de salida sea mayor que el tiempo de llegada*/
			if( depd.day == arrd.day && depd.totalmin > arrd.totalmin){
				
				/*Se verifica que el tiempo entre llegada y salida sea mayor o igual 
				al tiempo minimo que se debe estar en el recuerso*/
				if(depd.totalmin - arrd.totalmin >= restime.totalmin ){
					dept = get_time(depature[i][5]);
					arrt = get_time(arrival[j][8]);
					
					/*Se verifica que el tiempo entre llegada y salida sea menor/igual
					 que el tiempo maximo que puede estar el tren en la plataforma tras una llegada* /
					/*Se verifica que el tiempo entre llegada y salida sea menor/igual
					 que el tiempo maximo que puede estar el tren en la plataforma antes de la salida */
					if((depd.totalmin - arrd.totalmin <= arrt.totalmin) && (depd.totalmin - arrd.totalmin <=  dept.totalmin)){
						dept = get_time(depature[i][4]);
						arrt = get_time(arrival[j][7]);
						match_aux.depature = depature[i][0];
						match_aux.arrival = arrival[j][0];
						match_aux.train = arrival[j][1];
						if((depd.totalmin - arrd.totalmin) > arrt.totalmin){
							difa = (depd.totalmin - arrd.totalmin) - arrt.totalmin;
							cost = (difd*60 + difa*60)*atoi(parameters[10][0].c_str());
						}	
						if((depd.totalmin - arrd.totalmin) > dept.totalmin){	
							difd = (depd.totalmin - arrd.totalmin) - dept.totalmin;
							cost = (difd*60 + difa*60)*atoi(parameters[10][0].c_str());
						}

						match_aux.cost = cost;
						cout<<"Salida "<<match_aux.depature
						<<" cubierta por el tren "<<match_aux.train
						<<" De la salida "<<  match_aux.arrival
						<< " a un costo extra de "<< match_aux.cost<<endl;
						//cout<<"i: "<<i<<" j: "<<j<<" Resta "<< depd.totalmin - arrd.totalmin<<" min "<<restime.totalmin<<" minutos"<<" Costo: "<<cost<< endl; 
					}
				}
			}
		}
	}
}
	//cout<< depature[1][0] << endl;
	//Si se usa un tren originalmente en el sistema
		//-> Enter system at d1 00:00:00 
		//-> Enter resource

	//Si se usa un tren que viene de un arrival
		//-> Enter resource
		//-> Exit resource

void platform_assignment(){
}

void arrival_depature_sequence_assigner(){
}

void forward_checking_gbj(){
}


int main(int argc, char **argv){
	clock_t tStart = clock();
	string instance = argv[1];
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
	//matching_depature(departuresData,compCatDepData, initialTrainsData);
	 /*cout<<departuresData[2][1][0]<<endl
		<<departuresData[2][1][1]<<endl
		<<departuresData[2][1][2]<<endl
		<<departuresData[2][1][3]<<endl
		<<departuresData[2][1][4]<<endl
		<<departuresData[2][1][5]<<endl
		<<departuresData[2][1][6]<<endl
		<<departuresData[2][1][7]<<endl
		<<departuresData[2][1][8]<<endl
		<<departuresData[2][1][9]<<endl
		<<departuresData[2][1][10]<<endl
		<<departuresData[2][1][11]<<endl; */

	vector <int> test;
	/*
	int k = 0;
	for(k=0;k<10;k++){
		test.push_back(k);
	}
	
	test.pop_back();
	for(size_t i = 0; i<test.size();i++)
		cout<<test[i]<<endl;*/
	//string day = departuresData[1][1][0].c_str() + departuresData[1][1][1].c_str();
	//cout<< day <<endl;
	//date test;
	ttime t;
	//cout<<"T"<<endl;
	//t = get_time(departuresData[4][5]);
	/*
	cout<<"hora "<<t.hh<<endl;
	cout<<"min " <<t.mm<<endl;
	cout<<"seg " <<t.ss<<endl;
	cout<<"minutos totales " <<t.totalmin<<endl; */
	forward_checking_depature(departuresData,arrivalsData,initialTrainsData,parametersData,0);

	//cout<< "D"<<endl;	
	
	/*
	test = get_date(departuresData[2][1]);
	cout<<"Dia " <<test.day<<endl;
	cout<<"hora "<<test.hh<<endl;
	cout<<"min " <<test.mm<<endl;
	cout<<"seg " <<test.ss<<endl; */
	cout<< "la ejecucion fue de: "<<(double)(clock() - tStart)/CLOCKS_PER_SEC<<" segundos"<<endl; 
	return 0;
}