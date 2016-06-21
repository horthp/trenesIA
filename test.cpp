#include <cstdlib>
#include <iostream>
#include <cstring>
#include <string>
//#include <cmath>
#include <fstream>
//#include <ctime>
#include <vector> 
#include <sstream>
//#include "recursos.h"
#include <cstdlib>

using namespace std;

 vector< vector<string> > arrDepSequences, arrivals, compCatDep, compCatRes, departures, facilities, gates, 
    imposedConsumptions, initialTrains, jointArrDep, parameters, platforms, prefPlat, reuses, singleTracks, 
    trackGroups, trainCategories, yards;

typedef struct solution{
	string train;
	string time;
	string event_type;
	string resource;
	string gate;
	string Complement;
}sol;

typedef struct departures_matching{
	string departures;
	string train;
	bool arr;
	string arrivals;
	float cost;
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
/*Genera un domino para cada departure guardando los posibles trenes con los que pueden ser asignados y sus respectivos costos*/
void domain_dep(vector<depMatch> &match){
	vector< vector<string> > init_trains = initialTrains;
	float cost = 0;
	int difd = 0;
	int difa = 0;
	int flag;
	size_t sz;
	//vector<depMatch> match;
	depMatch match_aux;
	ttime dept, arrt, restime;
	date depd, arrd;	
	restime = get_time(parameters[12][1]);
	if(departures.size()!=0){
		for(size_t i=1; i < departures.size(); i++){
			flag = 0; 
			depd = get_date(departures[i][1]);
			if(arrivals.size() != 0){
				for(size_t j=1; j< arrivals.size();j++){
					arrd = get_date(arrivals[j][2]);

					/*Se verifica que la llegada con la salida pertenescan al mismo dia 
					y que el tiempo de salida sea mayor que el tiempo de llegada*/
					if( depd.day == arrd.day && depd.totalmin > arrd.totalmin){

						/*Se verifica que el tiempo entre llegada y salida sea mayor o igual 
						al tiempo minimo que se debe estar en el recuerso*/
						if(depd.totalmin - arrd.totalmin >= restime.totalmin ){
							dept = get_time(departures[i][5]);
							arrt = get_time(arrivals[j][8]);
							//cout<<departures[i][0]<<endl;

							/*Se verifica que el tiempo entre llegada y salida sea menor/igual
							 que el tiempo maximo que puede estar el tren en la plataforma tras una llegada* /
							/*Se verifica que el tiempo entre llegada y salida sea menor/igual
							 que el tiempo maximo que puede estar el tren en la plataforma antes de la salida */
							if((depd.totalmin - arrd.totalmin <= arrt.totalmin) && (depd.totalmin - arrd.totalmin <=  dept.totalmin)){
								dept = get_time(departures[i][4]);
								arrt = get_time(arrivals[j][7]);
								if((depd.totalmin - arrd.totalmin) > arrt.totalmin){
									difa = (depd.totalmin - arrd.totalmin) - arrt.totalmin;
									cost = cost + (difa*60)*atof(parameters[10][1].c_str());
								}	
								if((depd.totalmin - arrd.totalmin) > dept.totalmin){	
									difd = (depd.totalmin - arrd.totalmin) - dept.totalmin;
									cost = cost + (difd*60)*atof(parameters[10][1].c_str());
									//cout<<"costo 2: "<<cost<<endl;
								}
								for(size_t k = 1;k<reuses.size();k++){
									if(departures[i][0].compare(reuses[k][1])== 1){
										if(arrivals[j][0].compare(reuses[k][0]) == 0){
											cost = cost + atof(reuses[11][1].c_str());
											//cout<<"costo 3: "<<cost<<endl;
										}
									}
								}
								match_aux.arrivals = arrivals[j][0];
								match_aux.train = arrivals[j][1];
								match_aux.cost = cost;
								match_aux.departures = departures[i][0];
								match_aux.arr = 1;
								match.push_back(match_aux);

								flag = 1;
							}
						}
					}
				}
				if(flag == 0){
					cout<<init_trains.size()<<endl;
					if((init_trains.size() - 1) > 0){
						match_aux.train = init_trains[1][0];
						match_aux.departures = departures[i][0];
						match_aux.arr = 0;
						match_aux.cost = 0;
						match.push_back(match_aux);
						init_trains.erase(init_trains.begin() + 1);
					}
					else{
						match_aux.departures = "uncov";
						match_aux.arr = 0;
						match_aux.arrivals = " ";
						match_aux.train = " ";
						match_aux.cost = atof(parameters[16][0].c_str());
						match.push_back(match_aux);
					}
				}	
			}
		}
	}
}


void forward_checking_gbj_matching(vector<depMatch> imatch, vector<depMatch> &fmatch, float cost, int flag){
	//int flag;
	//float cost2;
	for(size_t i = flag ;i<imatch.size();++i){
		if(imatch[i].cost < cost && imatch.size() != 0){
		   fmatch[i] = imatch[i];
		   flag = i;
		   cost = cost - fmatch[i].cost;
		}
		else{
			

		}   

	}


}

void platform_assignment(){
}

void arrivals_departures_sequence_assigner(){
}




int main(int argc, char **argv){
	clock_t tStart = clock();
	string instance = argv[1];
    /*vector< vector<string> > arrDepSequencesData, arrivalssData, compCatDepData, compCatResData, departuresData, facilitiesData, gatesData, 
    imposedConsumptionsData, initialTrainsData, jointArrDepData, parametersData, platformsData, prefPlatData, reusesData, singleTracksData, 
    trackGroupsData, trainCategoriesData, yardsData;*/

	arrDepSequences = loadData((instance + "/arrDepSequences.csv").c_str(), 3);
    arrivals = loadData((instance + "/arrivals.csv").c_str(), 11);
	compCatDep = loadData((instance + "/compCatDep.csv").c_str(), 2);
	compCatRes = loadData((instance + "/compCatRes.csv").c_str(), 2);
	departures = loadData((instance + "/departures.csv").c_str(), 8);
	facilities = loadData((instance + "/facilities.csv").c_str(), 3);
	gates = loadData((instance + "/gates.csv").c_str(), 6);
	imposedConsumptions = loadData((instance + "/imposedConsumptions.csv").c_str(), 7);
	initialTrains = loadData((instance + "/initialTrains.csv").c_str(), 5);
	parameters = loadData((instance + "/parameters.csv").c_str(), 2);
	platforms = loadData((instance + "/platforms.csv").c_str(), 2);
	prefPlat = loadData((instance + "/prefPlat.csv").c_str(), 2);
	reuses = loadData((instance + "/reuses.csv").c_str(), 2);
	singleTracks = loadData((instance + "/singleTracks.csv").c_str(), 3);
	trackGroups = loadData((instance + "/trackGroups.csv").c_str(), 3);
	trainCategories = loadData((instance + "/trainCategories.csv").c_str(), 7);
	yards = loadData((instance + "/yards.csv").c_str(), 2);	

 
	vector <depMatch> test,final;
	float cost = 0;
	depMatch m;
	m.departures = " ";
	m.cost = 0;
	//cout<<arrivals.size()<<endl;
	domain_dep(test);
	cout<<"Dominio depX"<<endl;
	for(size_t l=0;l<test.size();l++){
		cost = cost + test[l].cost;
		cout<<"departures: "<<test[l].departures<<"|| train: "<<test[l].train<<" || de arrivals: "
		<<test[l].arrivals<< " || costo: "<< test[l].cost<< endl;
	}
	cout<<"Costo total "<<cost<<endl;
	/*/cout<< "D"<<endl;	
	/
	date d;
	ttime t;
	cout<< "departures" <<endl;
	d = get_date(departuresData[1][1]);
	t = get_time(departuresData[1][5]);
	cout<<"date 1 "<< d.totalmin <<" Totaldwell " << t.totalmin<<endl;
	d = get_date(departuresData[2][1]);
	t = get_time(departuresData[2][5]);
	cout<<"date 2 "<< d.totalmin <<" Totaldwell " << t.totalmin<<endl; 
	d = get_date(departuresData[3][1]);
	t = get_time(departuresData[3][5]);
	cout<<"date 3 "<< d.totalmin <<" Totaldwell " << t.totalmin<<endl; 
	d = get_date(departuresData[4][1]);
	t = get_time(departuresData[4][5]);
	cout<<"date 4 "<< d.totalmin <<" Totaldwell " << t.totalmin<<endl; 
	d = get_date(departuresData[5][1]);
	t = get_time(departuresData[5][5]);
	cout<<"date 5 "<< d.totalmin <<" Totaldwell " << t.totalmin<<endl;

	cout<<endl<<"arrivalss"<<endl; 
	d = get_date(arrivalssData[1][2]);
	t = get_time(arrivalssData[1][8]);
	cout<<"date 1 "<< d.totalmin <<"Totaldwell " << t.totalmin<<endl;
	d = get_date(arrivalssData[2][2]);
	t = get_time(arrivalssData[2][8]);
	cout<<"date 2 "<< d.totalmin <<"Totaldwell " << t.totalmin<<endl; 
	d = get_date(arrivalssData[3][2]);
	t = get_time(arrivalssData[3][8]);
	cout<<"date 3 "<< d.totalmin <<" totaldwell " << t.totalmin<<endl; 
	d = get_date(arrivalssData[4][2]);
	t = get_time(arrivalssData[4][8]);
	cout<<"date 4 "<< d.totalmin <<"Totaldwell " << t.totalmin<<endl; 
	d = get_date(arrivalssData[5][2]);
	t = get_time(arrivalssData[5][8]);
	cout<<"date 5 "<< d.totalmin <<" Totaldwell " << t.totalmin<<endl;  
	*/
	
	cout<< "la ejecucion fue de: "<<(double)(clock() - tStart)/CLOCKS_PER_SEC<<" segundos"<<endl; 
	return 0;
}