#include <cstdlib>
#include <iostream>
#include <cstring>
//#include <cmath>
#include <fstream>
//#include <ctime>
#include <vector> 
#include <sstream>
#include "recursos.h"

using namespace std;


int main(int argc, char **argv){	
	/*
	* Leer archivos. Se rescatan valores necesarios para poder trabajar
	*/ 
	string buffer;
	vector<train_category> tc;
	vector<train> trains;
	vector<yard> yards;
	vector<plataform> plat;
	vector<facility> fac;
	vector<gate> gates;
	gate gate_aux;
	facility fac_aux;
	plataform plat_aux;
	yard yard_aux;
	train train_aux;
	train_category tc_aux;
	ifstream archivo (argv[1]);
	ifstream archivo2 (argv[2]);
	ifstream archivo3 (argv[3]);
	ifstream archivo4 (argv[4]);
	ifstream archivo5 (argv[5]);
	ifstream archivo6 (argv[6]);

	if(archivo.fail()){
    	cerr << "Error al abrir: "<< argv[1] << endl;
    	exit(1);
    }
    else {
    	archivo >> buffer;
    	archivo >> buffer;
		int i = 0;
		while(!archivo.eof()){	
			stringstream ss(buffer);
			tc_aux.id.erase();
			tc_aux.length = -1;
			tc_aux.catGroup.erase();
			tc_aux.maxDBM = -1;
			tc_aux.maxTBM.erase();
			tc_aux.maintTimeT.erase();
			tc_aux.maintTimeD.erase();
			while (!ss.eof())         
			{             
				getline( ss, buffer,';');  
				if (tc_aux.id.empty())
					tc_aux.id = buffer;
				else{
					if (tc_aux.length == -1)
						tc_aux.length = atoi(buffer.c_str());
					else{
						if (tc_aux.catGroup.empty())
							tc_aux.catGroup = buffer;
						else{
							if(tc_aux.maxDBM == -1)
								tc_aux.maxDBM = atoi(buffer.c_str());
							else{
								if (tc_aux.maxTBM.empty())
									tc_aux.maxTBM = buffer;
								else{
									if(tc_aux.maintTimeT.empty())
										tc_aux.maintTimeT = buffer;
									else{
										if(tc_aux.maintTimeD.empty())
											tc_aux.maintTimeD = buffer;
									}
								}
							}	
						}
					}
				}
				//cout << buffer << endl;
				//cout <<endl;
			}
	    	tc.push_back(tc_aux);	

	    	archivo >> buffer;
    	}
    	/*
    	for(size_t i=0;i<tc.size();i++){
	    	cout<< tc[i].id <<endl;
			cout<< tc[i].length<<endl;
			cout<< tc[i].catGroup <<endl;
			cout<< tc[i].maxDBM <<endl;
			cout<< tc[i].maxTBM <<endl;
			cout<< tc[i].maintTimeT <<endl;
			cout<< tc[i].maintTimeD <<endl;
		}*/
    	archivo.close();
    	buffer.erase();
    }
    if(archivo2.fail()){
    	cerr << "Error al abrir: "<< argv[2] << endl;
    	exit(1);
    }
    else {
    	archivo2 >> buffer;
    	archivo2 >> buffer;
    	while(!archivo2.eof()){		
	    	stringstream ss(buffer);
			
			trains.push_back(train_aux);
			train_aux.id.erase();
			train_aux.category.erase();
			train_aux.resource.erase();
			train_aux.remTBM.erase();
			train_aux.remDBM = -1;
			//Se realiza un split por cada linea del archivo
			while(!ss.eof()){
				getline( ss, buffer,';');  
				if(train_aux.id.empty())
					train_aux.id = buffer;
				else{
					if(train_aux.category.empty())
						train_aux.category = buffer;
						else{
							if(train_aux.resource.empty())
								train_aux.resource = buffer;
							else{
								if(train_aux.remDBM == -1)
									train_aux.remDBM = atoi(buffer.c_str());
								else{
									if(train_aux.remTBM.empty())
										train_aux.remTBM = buffer;
									}
								}

							}
						}
					}
			archivo2 >> buffer;
		}
		archivo2.close();
		buffer.erase();
		/*for(size_t i=0;i<trains.size();i++){
	    	cout<< trains[i].id << endl;
			cout<< trains[i].category << endl;
			cout<< trains[i].resource << endl;
			cout<< trains[i].remDBM << endl;
			cout<< trains[i].remTBM << endl;
			cout<< endl;*/
	}
	if(archivo3.fail()){
    	cerr << "Error al abrir: "<< argv[3] << endl;
    	exit(1);
    }
    else {
    	archivo3 >> buffer;
    	archivo3 >> buffer;
    	while(!archivo3.eof()){
    		stringstream ss(buffer);
    		yard_aux.id.erase();
    		yard_aux.capa = -1;
    		while(!ss.eof()){
    			getline( ss, buffer,';');
    			if(yard_aux.id.empty())
    				yard_aux.id = buffer;
    			else{
    				if(yard_aux.capa == -1)
    					yard_aux.capa = atoi(buffer.c_str());
    			} 
    		}
    	yards.push_back(yard_aux);
    	archivo3 >> buffer;
    	}
    	archivo3.close();
    	buffer.erase();
    	/*for(size_t i=0;i<yards.size();i++){
	    	cout<< yards[i].id << endl;
			cout<< yards[i].capa << endl;
			cout<< endl;}*/
		}
		
	if(archivo4.fail()){
	    	cerr << "Error al abrir: "<< argv[4] << endl;
	    	exit(1);
	    }
	    else {
	    	archivo4 >> buffer;
	    	archivo4 >> buffer;
	    	while(!archivo4.eof()){
	    		stringstream ss(buffer);
	    		plat_aux.id.erase();
	    		plat_aux.length = -1;
	    		while(!ss.eof()){
	    			getline( ss, buffer,';');
	    			if(plat_aux.id.empty())
	    				plat_aux.id = buffer;
	    			else{
	    				if(plat_aux.length == -1)
	    					plat_aux.length = atoi(buffer.c_str());
	    			} 
	    		}
	    	plat.push_back(plat_aux);
	    	archivo4 >> buffer;
	    	}
	    	archivo4.close();
	    	buffer.erase();
	    	/*for(size_t i=0;i<yards.size();i++){
		    	cout<< plat[i].id << endl;
				cout<< plat[i].length << endl;
				cout<< endl;
			}*/
	    }		

	if(archivo5.fail()){
	    	cerr << "Error al abrir: "<< argv[5] << endl;
	    	exit(1);
	    }
	    else {
	    	archivo5 >> buffer;
	    	archivo5 >> buffer;
	    	while(!archivo5	.eof()){
	    		stringstream ss(buffer);
	    		fac_aux.id.erase();
	    		fac_aux.type.erase();
	    		fac_aux.length = -1;
	    		while(!ss.eof()){
	    			getline( ss, buffer,';');
	    			if(fac_aux.id.empty())
	    				fac_aux.id = buffer;
	    			else{
	    				if(fac_aux.type.empty())
	    					fac_aux.type = buffer;
	    				else{
	    					if(fac_aux.length == -1)
	    						fac_aux.length = atoi(buffer.c_str());
	    				}

	    			} 
	    		}
	    	fac.push_back(fac_aux);
	    	archivo5 >> buffer;
	    	}
	    	archivo5.close();
	    	buffer.erase();
	    	/*for(size_t i=0;i<fac.size();i++){
		    	cout<< fac[i].id << endl;
				cout<< fac[i].type << endl;
				cout<< fac[i].length << endl;
				cout<< endl;
			}*/
	    }

	if(archivo6.fail()){
	    	cerr << "Error al abrir: "<< argv[6] << endl;
	    	exit(1);
	    }
	    else {
	    	archivo6 >> buffer;
	    	archivo6 >> buffer;
	    	while(!archivo6	.eof()){
	    		stringstream ss(buffer);
				gate_aux.resource_id.erase();
				gate_aux.side.erase();
				gate_aux.index = -1;
				gate_aux.neighborGateResource.erase();
				gate_aux.neighborGateSide.erase();
				gate_aux.neighborGateIndex = -1;
	    		while(!ss.eof()){
	    			getline( ss, buffer,';');
	    			if(gate_aux.resource_id.empty())
	    				gate_aux.resource_id = buffer;
	    			else{
	    				if(gate_aux.side.empty())
	    					gate_aux.side = buffer;
	    				else{
	    					if(gate_aux.index == -1)
	    						gate_aux.index = atoi(buffer.c_str());
	    					else{
	    						if(gate_aux.neighborGateResource.empty())
	    							gate_aux.neighborGateResource = buffer;
	    						else{
	    							if(gate_aux.neighborGateSide.empty())
	    								gate_aux.neighborGateSide = buffer;
	    							else{
	    								if(gate_aux.neighborGateIndex == -1)
	    									gate_aux.neighborGateIndex = atoi(buffer.c_str());
	    							}
	    						}
	    					}
	    				}
	    			}
	    		}
	    		gates.push_back(gate_aux);
	    		archivo6 >> buffer;
	    	}
	    archivo6.close();
	    buffer.erase();
	    }
	    for(size_t i=0;i < gates.size();i++){	
	    	cout<< gates[i].resource_id<< endl;
			cout<< gates[i].side<< endl;
			cout<< gates[i].index<< endl;
			cout<< gates[i].neighborGateResource<< endl;
			cout<< gates[i].neighborGateSide<< endl;
			cout<< gates[i].neighborGateIndex<< endl;
			cout<< endl;
		}

	return 0;
}
	
			


