#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector> 

using namespace std;

int buses; /*numero de buses*/
int capacidad; /*capacidad del bus*/
int cantidad_personas; /*cantidad de personas totales a evacuar*/
int estaciones; /*numero de estaciones*/
int puntos_encuentro; /*numero de puntos de encuentro*/
int refugios; /*cantidad de refugios*/
vector<int> personas_encuentro; /*vector que guarda cantidad de personas en cada punto de encuentro*/
vector<int> cap_refugios; /*vector que guarda la capacidad de cada refugios*/
vector<int> cap_refugios_inicial; /*capacidad inicial de los refugios.*/
vector<int> cap_refugios_final; /*capacidad final de los refugios de la solucion final*/
vector<int> buses_estaciones; /*vector que guarda la cantidad de buses por estacion*/
int flag_encuentro; /*flag que sirve para reconocer si estamos parados en un punto de encuentro*/
int flag_refugio; /*flag que sirve para reconocer si estamos parados en un punto de un refugio*/

struct bus {
  int id;
  int partida;
  int final;
  int costo;
} ;
bus Bus;
bus Bus_aux;
vector<bus> viajes_buses; /*se guarda la estructura de bus, para almacenar los viajes*/
vector<bus> solucion; /*variable encargada de guardar la solucion final*/

struct costos{
	int id_bus;
	int costo_total;
};
costos costo;
vector<costos> costos_solucion; /*guarda los costos asociados a la solucion final*/
vector<costos> costos_aux; /*variable auxiliar para calcular que solucion es mejor*/

struct distancias{
	int partida;
	int final;
	int costo;
};
distancias dist;
vector<distancias> dist_est; /*vector con las distancias desde las estaciones a los puntos de encuentro*/
vector<distancias> dist_puntos_refugios; /*vector con las distancias desde los puntos de encuentro a los refugios*/




/*funcion que se encarga de encontrar la mejor solucion, compara la mejor solucion hasta el momento con la actual*/
void Mejor_Solucion(){
	int max=0; /*costo maximo de los viajes hechos por los buses*/
	int max_temp=0; /*costo maximo de la nueva solucion, para poder comparar*/
	/*for(i=0;i<viajes_buses.size();i++){
		cout << viajes_buses[i].id;
	}
	cout << "\n";*/
	if(costos_solucion.size() == 0){ /*si no hay ninguna solucion hasta el momento, se crea con la primera encontrada*/
		for(int j=0;j<buses;j++){
			costo.id_bus = j+1;
			costo.costo_total = 0;
			for(size_t i=0;i<viajes_buses.size();i++){
				if((j+1)==viajes_buses[i].id){
					costo.costo_total = costo.costo_total + viajes_buses[i].costo;
				}			
			}
			costos_solucion.push_back(costo);
		}
		for(size_t i=0;i<viajes_buses.size();i++){ /*se guarda la solucion*/
			Bus.id = viajes_buses[i].id;
			Bus.partida = viajes_buses[i].partida;
			Bus.final = viajes_buses[i].final;
			Bus.costo = viajes_buses[i].costo;
			solucion.push_back(Bus);
		}
		for(size_t i=0;i<cap_refugios.size();i++){ /*se guarda la capacidad de los refugios finales*/
			cap_refugios_final.push_back(cap_refugios[i]);
		}
		return;
	}
	else{ /*si hay solucion actual, se guardan los costos de la nueva solucion en un vector auxiliar para luego comparar cual es mejor*/
		for(int j=0;j<buses;j++){
			costo.id_bus = j+1;
			costo.costo_total = 0;
			for(size_t i=0;i<viajes_buses.size();i++){
				if((j+1)==viajes_buses[i].id){
					costo.costo_total = costo.costo_total + viajes_buses[i].costo;
				}				
			}
			costos_aux.push_back(costo);
		}
		for(size_t i=0;i<costos_solucion.size();i++){
			if(max<costos_solucion[i].costo_total){
				max = costos_solucion[i].costo_total;		
			}
		}
		for(size_t i=0;i<costos_aux.size();i++){
			if(max_temp<costos_aux[i].costo_total){
				max_temp = costos_aux[i].costo_total;		
			}
		}
		if(max_temp<max){ /*se compara si la solucion actual es mejor que la conseguida hasta el momento*/
			while(costos_solucion.size() != 0){
				costos_solucion.pop_back();
			}
			for(size_t i=0;i<costos_aux.size();i++){
				costos_solucion.push_back(costos_aux[i]);
			}
			while(costos_aux.size() != 0){
				costos_aux.pop_back();
			}
			while(solucion.size() != 0){
				solucion.pop_back();
			}
			while(cap_refugios_final.size() !=0){
				cap_refugios_final.pop_back();
			}
			for(size_t i=0;i<viajes_buses.size();i++){ /*se guarda la nueva mejor solucion*/
				solucion.push_back(viajes_buses[i]);
			}
			for(size_t i=0;i<cap_refugios.size();i++){ /*se guarda la capacidad de los refugios finales*/
				cap_refugios_final.push_back(cap_refugios[i]);
			}
		}
		else{ /*si la solucion actual no es mejor que la que tenemos guardada, entonces se vacia el vector auxiliar.*/
			while(costos_aux.size() != 0){
				costos_aux.pop_back();
			}			
		}
		return;
	}
}

/*Esta funcion nos dice si el bus no ha hecho ningun viaje aun, para saber si parte de la estacion o no.*/
bool Primero(int i){
	if(viajes_buses.size()==0){
		return true;
	}
	else{
		for(size_t x=0;x<viajes_buses.size();x++){
			if(viajes_buses[x].id == i+1){
				return false;
			} 
		};
		return true;
	}
}

/*funcion forward checking, avanza con recursion hasta que no queden mas personas por evacuar*/
void Forward_Checking(int k, int l){
	int flag;
	int buses_acumulados;
	for(int i=0;i<buses;i++){
		flag = 0;
		buses_acumulados=0;
		if(Primero(i) && personas_encuentro[k]>0 && k<puntos_encuentro){ /*se revisa si es el primer viaje y si aun hay gente por evacuar*/
			Bus.id = i+1;
			for(int j=0;j<estaciones;j++){
				buses_acumulados = buses_acumulados + buses_estaciones[j];
				if(((i+1)-buses_acumulados)<1){
					Bus.partida = j+1;
				}
			}
			Bus.final = k+1;
			for(size_t j=0;j<dist_est.size();j++){
				if(dist_est[j].partida == Bus.partida && dist_est[j].final == Bus.final){
					Bus.costo = dist_est[j].costo;
				}
			}
			viajes_buses.push_back(Bus);
			flag_encuentro=1;
			flag_refugio=0;
		}
		if(flag_encuentro==1 && personas_encuentro[k]>0 && k<puntos_encuentro){ /*movimiento pensado cuando el bus esta en un punto de encuentro*/
			Bus.id = i+1;
			Bus.partida = k+1;
			if(cap_refugios[l]>0){
				Bus.final=l+1;
				cap_refugios[l]=cap_refugios[l]-capacidad;
			}
			else{
				l=l+1;
				Bus.final=l+1;
				cap_refugios[l]=cap_refugios[l]-capacidad;
			}
			for(size_t j=0;j<dist_puntos_refugios.size();j++){
				if(dist_puntos_refugios[j].partida == Bus.partida && dist_puntos_refugios[j].final == Bus.final){
					Bus.costo = dist_puntos_refugios[j].costo;
				}
			}
			personas_encuentro[k]= personas_encuentro[k] - capacidad;
			if(personas_encuentro[k]==0 && k < puntos_encuentro){
				k=k+1;
			}
			flag_encuentro=0;
			flag_refugio=1;
			viajes_buses.push_back(Bus);
			Forward_Checking(k,l);/*se ocupa recursion con los valores de k y l usados hasta el momento*/
			Bus = viajes_buses.back();
			k=Bus.partida - 1;
			personas_encuentro[k]= personas_encuentro[k] + capacidad;/*cuando se devuelve, se aumenta la cantidad de personas en el punto de encuentro*/
			l = Bus.final - 1;
			cap_refugios[l] = cap_refugios[l]+capacidad;
			viajes_buses.pop_back();
			viajes_buses.pop_back();
			flag = 1;	
		}
		if(flag_refugio==1 && personas_encuentro[k]>0 && k<puntos_encuentro && flag ==0){ /*movimientos pensados cuando se esta parado en un refugio*/
			Bus.id = i+1;
			for(size_t z=0;z<viajes_buses.size();z++){
				if((i+1)==viajes_buses[z].id){
					Bus_aux = viajes_buses[z];
				}
			}
			Bus.partida = Bus_aux.final;
			Bus.final =k+1;
			for(size_t j=0;j<dist_puntos_refugios.size();j++){
				if(dist_puntos_refugios[j].final == Bus.partida && dist_puntos_refugios[j].partida == Bus.final){
					Bus.costo = dist_puntos_refugios[j].costo;
				}
			}
			flag_refugio=0;
			flag_encuentro=1;
			viajes_buses.push_back(Bus);	
		}
		if(flag_encuentro==1 && personas_encuentro[k]>0 && k<puntos_encuentro){/*movimiento pensado cuando el bus esta en un punto de encuentro*/
			Bus.id = i+1;
			Bus.partida = k+1;
			if(cap_refugios[l]>0){
				Bus.final=l+1;
				cap_refugios[l]=cap_refugios[l]-capacidad;
			}
			else{
				l=l+1;
				Bus.final=l+1;
				cap_refugios[l]=cap_refugios[l]-capacidad;
			}
			for(size_t j=0;j<dist_puntos_refugios.size();j++){
				if(dist_puntos_refugios[j].partida == Bus.partida && dist_puntos_refugios[j].final == Bus.final){
					Bus.costo = dist_puntos_refugios[j].costo;
				}
			}
			personas_encuentro[k]= personas_encuentro[k] - capacidad;
			if(personas_encuentro[k]==0 && k < puntos_encuentro){
				k=k+1;
			}
			flag_encuentro=0;
			flag_refugio=1;
			viajes_buses.push_back(Bus);
			Forward_Checking(k,l); /*se ocupa recursion con los valores de k y l usados hasta el momento*/
			Bus = viajes_buses.back();
			k=Bus.partida - 1;
			personas_encuentro[k]= personas_encuentro[k] + capacidad;/*cuando se devuelve, se aumenta la cantidad de personas en el punto de encuentro*/
			l = Bus.final - 1;
			cap_refugios[l] = cap_refugios[l]+capacidad; /*cuando se devuelve, se aumenta la capacidad del refugio*/
			viajes_buses.pop_back();
			viajes_buses.pop_back();			
		}
		if( k==puntos_encuentro ){ /*si k=puntos de encuentro quiere decir que no quedan personas para evacuar y nos devolvemos en la recursividad.*/
			Mejor_Solucion();
			return;
		}	
	}
	return;
}



int main(int argc, char **argv)
{
	clock_t tStart = clock();
	int k,l;
	int buses_acumulados;
	string buffer;
	/*
	* Leer archivo. Se rescatan valores necesarios para poder trabajar
	*/
	ifstream archivo (argv[1]);
	if(archivo.fail()){
    	cerr << "Error al abrir: "<< argv[1] << endl;
    	exit(1);
    }
    else {
    	archivo >> buffer;
    	buses = atoi(buffer.c_str());
    	archivo >> buffer;
    	capacidad = atoi( buffer.c_str() );
    	archivo >> buffer;
        estaciones = atoi( buffer.c_str() );
		for(int i=0;i<estaciones;i++){
    		archivo >> buffer;
    		buses_estaciones.push_back(atoi( buffer.c_str() ));
		}
    	archivo >> buffer;
    	puntos_encuentro = atoi( buffer.c_str() );
        archivo >> buffer;
        cantidad_personas = atoi( buffer.c_str() );
    	for(int i=0;i<puntos_encuentro;i++){
    		archivo >> buffer;
    		personas_encuentro.push_back(atoi( buffer.c_str() ));
		}
		archivo >> buffer;
		refugios = atoi(buffer.c_str());
		archivo >> buffer;
    	for(int i=0;i<refugios;i++){
    		archivo >> buffer;
    		cap_refugios.push_back(atoi( buffer.c_str() ));
    		cap_refugios_inicial.push_back(atoi( buffer.c_str() ));
		}
		archivo >> buffer;
		for(int i=0;i<estaciones;i++){
			for(int j=0;j<puntos_encuentro;j++){
				archivo >> buffer;
				dist.partida = i+1;
				dist.final = j+1;
				dist.costo = atoi( buffer.c_str());
				dist_est.push_back(dist);
			}
			archivo >> buffer;
		}
		for(int i=0;i<puntos_encuentro;i++){
			for(int j=0;j<refugios;j++){
				archivo >> buffer;
				dist.partida = i+1;
				dist.final = j+1;
				dist.costo = atoi( buffer.c_str());
				dist_puntos_refugios.push_back(dist);
			}
			archivo >> buffer;
		}
		k=0;
		l=0;
		for(int i=0;i<buses;i++){
			buses_acumulados=0;
			flag_refugio=0;
			flag_encuentro=0;
			if(Primero(i) && personas_encuentro[k]>0 && k<puntos_encuentro){/*se revisa si es el primer viaje y si aun hay gente por evacuar*/
				Bus.id = i+1;
				for(int j=0;j<estaciones;j++){
					buses_acumulados = buses_acumulados + buses_estaciones[j];
					if(((i+1)-buses_acumulados)<1){
						Bus.partida = j+1;
					}
				}
				Bus.final = k+1;
				for(size_t j=0;j<dist_est.size();j++){
					if(dist_est[j].partida == Bus.partida && dist_est[j].final == Bus.final){
						Bus.costo = dist_est[j].costo;
					}
				}
				viajes_buses.push_back(Bus);
				flag_encuentro=1;
				flag_refugio=0;
			}
			if(flag_encuentro==1 && personas_encuentro[k]>0 && k<puntos_encuentro){ /*movimiento pensado cuando el bus esta en un punto de encuentro*/
				Bus.id = i+1;
				Bus.partida = k+1;
				if(cap_refugios[l]>0){
					Bus.final=l+1;
					cap_refugios[l]=cap_refugios[l]-capacidad;
				}
				else{
					l=l+1;
					Bus.final=l+1;
					cap_refugios[l]=cap_refugios[l]-capacidad;
				}
				for(size_t j=0;j<dist_puntos_refugios.size();j++){
					if(dist_puntos_refugios[j].partida == Bus.partida && dist_puntos_refugios[j].final == Bus.final){
						Bus.costo = dist_puntos_refugios[j].costo;
					}
				}
				personas_encuentro[k]= personas_encuentro[k] - capacidad;
				if(personas_encuentro[k]==0 && k < puntos_encuentro){
					k=k+1;
				}
				flag_encuentro=0;
				flag_refugio=1;
				viajes_buses.push_back(Bus);			
			}
			Forward_Checking(k,l); /*se llama a la funcion forward_checking la cual se encargara de encontrar la solucion con recursion.*/
			Bus = viajes_buses.back();
			k=Bus.partida - 1;
			personas_encuentro[k]= personas_encuentro[k] + capacidad; /*se aumenta la cantidad de personas en la capacidad del bus, para devolverse.*/
			l = Bus.final - 1;
			cap_refugios[l] = cap_refugios[l]+capacidad;
			viajes_buses.pop_back();
			viajes_buses.pop_back();
		}
		
	}
	archivo.close();
	string instancia = "Solucion-" + std::string(argv[1]);
	ofstream myfile;
  	myfile.open (instancia.c_str());
  	int max=0;
  	for(int i=0;i<buses;i++){
  		if(max<costos_solucion[i].costo_total){
  			max=costos_solucion[i].costo_total;
		  }
	}
	myfile << "Duracion total de evacuacion: " << max << "\n";
	myfile << "\n";
	myfile << "Tiempo de ejecucion: " << (double)(clock() - tStart)/CLOCKS_PER_SEC << " segundos\n";
	myfile << "\n";
  	for(int i=0;i<buses;i++){
  		myfile << "Bus " << i+1 << ": ";
  		for(size_t j=1;j<solucion.size();j=j+2){
  			if(solucion[j].id==i+1){
				myfile << "(" << solucion[j].partida << "," << solucion[j].final << ")  ";  
			}
		}
		if(max==costos_solucion[i].costo_total){
			myfile << "Costo Total: " << costos_solucion[i].costo_total << " *\n";
		}
		else{
			myfile << "Costo Total: " << costos_solucion[i].costo_total << "\n";
		}
	}
	myfile << "\n";
	for(int i=0;i<refugios;i++){
		myfile << "Cantidad de personas en el refugio " << i+1 << ": " << cap_refugios_inicial[i] - cap_refugios_final[i] << "\n";
	}
  	myfile.close();
	return 0;
}
