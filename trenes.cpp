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

typedef struct departure_matching{
	string departures;
	vector<string> arrivals;
	vector<string> train;
	vector<int> used_train;
	bool arr;
	//Costo asociado por dejar la salida sin cubrir
	float cost0;
	//Costo por seleccionar el tren i 
	vector <float> cost;
	int dom;
	bool used;
}depMatch;


vector <depMatch> test, final;


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

typedef struct platforms_departure{
};

typedef struct plataforms_arrival{
};

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
	float c;
	float cost0 = 0;
	int difd = 0;
	int difa = 0;
	int flag;
	int dom;
	size_t sz;
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
							match_aux.arrivals.push_back(arrivals[j][0]);
							match_aux.train.push_back(arrivals[j][1]);
							match_aux.used_train.push_back(0);
							flag = 1;
							
							//Se asigna un costo en caso de no cumplir con la reutilizacion de trenes
							for(size_t k = 1;k<reuses.size();k++){	
								if(departures[i][0].compare(reuses[k][1]) == 0){
									if(arrivals[j][0].compare(reuses[k][0])){
										match_aux.cost.push_back(atof(parameters[11][1].c_str()));
									}else{
										match_aux.cost.push_back(0);
									}	
								}
							}
						}
					}

				}

			}
			//Si la salida actual no esta en reuses se asigna costo 0 al vector de costos
			for(size_t k = 1;k<reuses.size();k++){	
				if(departures[i][0].compare(reuses[k][1])){
					for(size_t j= 0; j< match_aux.arrivals.size();j++){
						match_aux.cost.push_back(0);
					}
				}	
			}	

			
			if(flag == 1){
					match_aux.cost0 = cost0;
					match_aux.departures = departures[i][0];
					match_aux.dom = match_aux.train.size();
					match_aux.arr = 1;
					match_aux.used = 0;
					match.push_back(match_aux);
			}
			if(flag == 0){
				cout<<init_trains.size()<<endl;
				if((init_trains.size() - 1) > 0){
					match_aux.train.push_back(init_trains[1][0]);
					match_aux.arrivals.push_back("init");
					match_aux.departures = departures[i][0];
					match_aux.arr = 0;
					match_aux.cost0 = 0;
					match_aux.used = 0;
					match_aux.used_train.push_back(0);
					match_aux.cost.push_back(0);
					match_aux.dom = match_aux.train.size();
					match.push_back(match_aux);
					init_trains.erase(init_trains.begin() + 1);
				}					
				else{
					match_aux.departures = "uncov";
					match_aux.arr = 0;
					match_aux.arrivals.push_back("void");
					match_aux.train.push_back("void");
					match_aux.dom = 0;
					match_aux.used_train.push_back(-1);
					match_aux.cost0 = atof(parameters[16][1].c_str());
					match.push_back(match_aux);
				}
			}

			match_aux.train.clear();
			match_aux.arrivals.clear();
			match_aux.cost.clear();


		}
	}
	/*
	for(size_t a = 0; a < match.size(); a++){
		cout<<match[a].departures<<"|"<<endl;
		for(size_t b= 0; b<match[a].train.size();b++){
			cout<<match[a].arrivals[b]<<"|";}
		cout<<endl;	
		for(size_t d=0; d < match[a].cost.size();d++){
			cout<<"|"<<match[a].cost[d];
		}
	cout<<endl;	
	} */
}

void fc_gbj_match(vector<depMatch> &final,int l, int r){
	vector <depMatch> aux;
	int cost_i = 0;
	int a = l;
	int b = r;
	bool flag = 0;
	int sum = 0;
	depMatch mt;
	aux = test;
	int c,size;
	for(size_t i=0; i < aux.size(); i++){
		
		// Asignacion de tren a la primera salida.
		if(final.empty()){
			mt.departures = aux[i].departures;
			mt.arr = aux[i].arr;
			mt.cost0 = aux[i].cost0;
			//mt.arrivals.push_back(aux[i].arrivals[0]);
			//mt.train.push_back(aux[i].train[0]);
			//mt.cost.push_back(aux[i].cost[0]);
			//mt.used_train.push_back(1);
			//mt.dom = mt.train.size();
			
			//Se trata de asignar un tren con costo 0
			for(size_t j = 0; j< aux[i].train.size();j++){
				if(aux[i].cost[j] == 0){
					mt.arrivals.push_back(aux[i].arrivals[j]);
					mt.train.push_back(aux[i].train[j]);
					mt.cost.push_back(aux[i].cost[j]);
					mt.used_train.push_back(1);
					mt.dom = mt.train.size();
					//flag = 1;
					break;
				}
			}

			if(flag == 0){
				for(size_t j = 0; j<aux[i].train.size();j++){

				}

			}

			//Se guardan los valores en el archivo final
			final.push_back(mt);
			aux[i].used = 1;
			aux[i].used_train[0] = 1;
			a = i;
			b = 0;
			//Se resetean los vectores
			mt.used_train.clear();
			mt.train.clear();
			mt.arrivals.clear();
			mt.cost.clear();
			cout<<final[0].cost[0]<<endl;
			//si otra salida es compatible con el tren recien asignado se marca como usado.
			for(size_t j=0;j<aux.size();j++){
				for(size_t k=0; k< aux[j].train.size();k++){
					if(aux[j].train[k].compare(final[0].train[0]) == 0){
						aux[j].used_train[k] = 1;
					}
				}
			}		
		}
		else{
			c = -1;
			flag = 0;
			// Si la salida no est cubierta.
			if(aux[i].used == 0 ){
				/* Asignacion de datos */
				mt.departures = aux[i].departures;
				mt.arr = aux[i].arr;
				mt.used_train.push_back(1);
				mt.cost0 = aux[i].cost0;
				//se asigna el tren a la salida actual
				for(size_t s=0;s<aux[i].train.size();s++){
					//Se verifica que el tren no este en uso y se trata de asignar a uno que sea de costo 0
					if(aux[i].used_train[s] == 0 && aux[i].cost[s] == 0){
						//Se selecciona el primer tren disponible
						mt.train.push_back(aux[i].train[s]);
						mt.arrivals.push_back(aux[i].arrivals[s]);
						mt.cost.push_back(aux[i].cost[s]);
						aux[i].used_train[s] = 1;
						flag = 1;
						break;
					}
				}
				cout << flag<<endl;
				if(flag == 0){
					for(size_t s=0;s < aux[i].train.size();s++){
						if(aux[i].used_train[s] == 0){
							mt.train.push_back(aux[i].train[s]);
							mt.arrivals.push_back(aux[i].arrivals[s]);
							mt.cost.push_back(aux[i].cost[s]);
							aux[i].used_train[s] = 1;
							break;
						}
					}

				}

				mt.dom = mt.train.size();
				aux[i].used = 1;
				final.push_back(mt);
				size = final.size();
				c = final[size - 1].train.size();
				
				if(c > -1 ){
					//Se realiza el filtrado en base al tren seleccionado para casa uno de los dominios de las demás salidas
					for(size_t j=0;j<aux.size();j++){
						sum = aux[j].train.size();
						for(size_t k=0; k< aux[j].train.size()-1;k++){
							if(aux[j].train[k].compare(final[size-1].train[c-1]) == 0){
								aux[j].used_train[k] = 1;
							}
							//si hay alguna salida sin trenes en su dominio sum = 0
							if(aux[j].used_train[k] == 1){
								sum = sum - 1;
								a = k;
							}
						}
						//Se llama recursivamente a la funcion nuevamente
					/*	if(sum == 0){
							cout<<aux[j].departures<<"| dominio vacio"<<endl;
							fc_gbj_match(final,j,a,0);
						}*/
					}
				}	

				//Se resetean los vectores
				mt.used_train.clear();
				mt.train.clear();
				mt.arrivals.clear();
				mt.cost.clear();
			}	
		}
	}

	for(size_t i =0; i< final.size(); i++){
		for(size_t j=0; j<final[i].cost.size();j++){
			cost_i = cost_i + final[i].cost[j];
		}
	}
	cout<<cost_i<<endl;
	/*
	if(costo > cost_i){
		fc_gbj_match(final,b,a,cost_i);
	}
	else{
	} */

	return;	
}


/*
void forward_checking_gbj_matching(vector<depMatch> &final, int flag){
	int count;
	int cost = 0;
	vector<depMatch> imatch;
	vector<string> filtro,aux;
	depMatch fmatch;
	imatch = test;
	string uncover;
	for(size_t i = flag ;i<imatch.size();i++){	
		if(final.empty()){			
			//cout<<"assd"<<endl;
			fmatch.departures = imatch[i].departures;
			fmatch.arrivals = imatch[i].arrivals;
			fmatch.arr = imatch[i].arr;
			fmatch.cost = imatch[i].cost;
			fmatch.train = imatch[i].train;
			fmatch.revised = imatch[i].revised; 
			//cout<< "asdfg"<<endl;
			final.push_back(fmatch);
			cost = cost - imatch[i].cost;
			imatch[i].used = 1;
			imatch[i].revised = 1;
			count = 1;			
			cout<<"a-"<<final[i].departures<<"|"<<final[i].train <<endl;
			for(size_t j = 0; j< imatch.size();j++){
				for(size_t k = 0; k < final.size();k++){
					if(imatch[j].revised == 0 && imatch[j].used == 0){	
						if(final[k].train.compare(imatch[j].train)== 0 ){
							cout<<"Coincidencia tren"<<imatch[j].departures<<"|"<<imatch[j].train<<endl;  
							imatch[j].revised = 1;
						}
						if(final[k].departures.compare(imatch[j].departures)== 0 ){
							cout<<"Coincidencia salida "<<imatch[j].departures<<"|"<<imatch[j].train<<endl;  
							imatch[j].revised = 1;
						}	
					}
				}
			}
			//cout<<"used"<<imatch[0].used<<endl;
		}else{
			//cout<<"else"<<endl;
			if(imatch[i].used == 0 && imatch[i].revised == 0){
				//cout<<"if"<<endl;
				fmatch.departures = imatch[i].departures;
				fmatch.arrivals = imatch[i].arrivals;
				fmatch.arr = imatch[i].arr;
				fmatch.cost = imatch[i].cost;
				fmatch.train = imatch[i].train;
				imatch[i].used = 1;
				imatch[i].revised = 1;
				count = count + 1;
				//cout<<"ipush_back"<<endl;
				final.push_back(fmatch);
				//cout<<"fpush_back"<<endl;
				//final.push_back(fmatch);
				//filtro.push_back(fmatch.train);
				cost = cost - imatch[i].cost;
				//cout<<final[i].departures<<"|"<<final[i].train<<endl;
			
				cout<<imatch[i].departures <<" | "<< imatch[i].train << endl;
				for(size_t j = 0; j< imatch.size();j++){
					for(size_t k = 0; k < final.size();k++){
						if(imatch[j].revised == 0 && imatch[j].used == 0){	
							if(final[k].train.compare(imatch[j].train)== 0 ){
								cout<<"Coincidencia tren "<<imatch[j].departures<<"|"<<imatch[j].train<<endl;  
								imatch[j].revised = 1;
							}
							if(final[k].departures.compare(imatch[j].departures)== 0 ){
								cout<<"Coincidencia salida "<<imatch[j].departures<<"|"<<imatch[j].train<<endl;  
								imatch[j].revised = 1;
							}	
						}
					}
				}
			}	

		}
		for(size_t l = 0;l<imatch.size();l++){
			if(imatch[l].revised == 1)
				cout<<"salida->"<<imatch[l].departures<<"| train->"<<imatch[l].train<<endl;
		}

		//cout<<imatch[i].departures<<"|"<<imatch[i].train<<endl;   
		cout<<"fin iteracion: "<<i<<endl;
	}

	for(size_t i = 0;i<final.size();i++)
		cout<<"Asignacion final "<<final[i].departures<<"|"<<final[i].train<<endl;



	cout<<"Cantidad trenes asignados: "<<final.size()<<"| Count: "<< count<<endl;
	cout << "Departures: "<<departures.size() - 1 << endl;
}	
*/
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

 
	vector <depMatch> final;
	int a,b;

	//cout<<arrivals.size()<<endl;
	domain_dep(test);
	cout<<"Dominio depX"<<endl;
	for(size_t l=0;l<test.size();l++){
		//cost = cost + test[l].cost;
		cout<<"departures: "<<test[l].departures
		<<" |costo: "<<test[l].cost0<<endl;
		//cout<<test[l].train.size();
		for(size_t i=0; i< test[l].train.size();i++){
			cout<<"	|| train: "<<test[l].train[i]//<<endl;
				<<"| costo:  "<< test[l].cost[i]<<endl;
		}
		cout<<"	|| #dominio: "<<test[l].dom<<endl;
	}
	//cout<<"Costo total "<<cost<<endl;
	cout<<endl;
	fc_gbj_match(final,-1,-1);
	cout<<"asignaciones"<<endl;
	for(size_t l=0;l<final.size();l++){
		//cost = cost + test[l].cost;
		cout<<"departures: "<<final[l].departures
			<<" |costo "<<final[l].cost0<<endl;
		//cout<<test[l].train.size();
		for(size_t i=0; i< final[l].train.size();i++){
			if(final[l].used_train[i] == 1)
				cout<<"	|| train: "<<final[l].train[i]<<" Come from "
					<<final[l].arrivals[i]
					<<" Costo: "<<final[l].cost[i]<<endl;
		}
		//cout<<"	|| #dominio: "<<test[l].dom<<endl;
	}
	//forward_checking_gbj_matching(final,0);
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