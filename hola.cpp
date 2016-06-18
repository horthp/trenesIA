#include <stdio.h>
#include <iostream>
#include <cstring>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector> 

#include <iomanip>
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
using namespace std;

 
#define		FALSE 0
#define		TRUE  1
 
#define 	MAXLINEA 	255
#define 	MAXCAMPO	41
#define 	TOPEVECTOR 	200
 
//typedef int 	bool;
typedef char 	str40[MAXCAMPO];
typedef char 	str_linea[MAXLINEA];

struct plataform{
	unsigned int id;
	unsigned int length;
};

struct yard{
	unsigned int id;
	unsigned int capa;
};

struct facility{
	unsigned int id;
	char type;
	unsigned int length;
};

struct gate{
	unsigned int index;
	string resource;
	unsigned int resource_id;
	char side;
	string neighborGateResource;
	unsigned int neighborGateResource_id;
	char neighborGateSide;
	unsigned int neighborGateIndex;	
};

struct train{
	unsigned int id;
	unsigned int category;
	string resource;
	unsigned int remDBM;
	unsigned int remTBM;
	bool reused;
};

struct train_category{
	string id;
	//unsigned int id;
	unsigned int length;
	string catGroup;
	//unsigned int catGroup;
	unsigned int maxDBM;
	//unsigned int maxTBM;
	string maxTBM;
	//unsigned int maintTimeD;
	string maintTimeD;
	string maintTimeT;
	//unsigned int maintTimeT;
};

struct trackGroups{
	unsigned int id;
	//Revisar como guaradr las variables de tiempo
	unsigned int trTime;
	unsigned  int hwTime;
};

struct singleTrack{
	unsigned int id;
	unsigned int length;
	unsigned int capa;
};

struct arrival{
	unsigned int id;
	unsigned int train_id;
	//Revisar como guardar las variables de tiempo
	unsigned int arrTime;
	unsigned int arrSeq_id;
	unsigned int trainCategory;
	unsigned int maxDewell;
	unsigned int idealDwell;
	unsigned int remDBM;
	unsigned int remTBM;
	unsigned int prefPlat;
};

struct depature{
	unsigned int id;
	unsigned int train_id;
	//Revisar como guardar las variables de tiempo
	unsigned int depTime;
	unsigned int depSeq_id;
	//unsigned int trainCategory;
	unsigned int maxDewell;
	unsigned int idealDwell;
	unsigned int reqDBM;
	unsigned int reqTBM;
	unsigned int prefPlat;
};


int main(){
//int main(int argc, char **argv){	
	/*
	* Leer archivo. Se rescatan valores necesarios para poder trabajar
	*/
	/*
	string buffer;
	ifstream archivo (argv[1]);
	if(archivo.fail()){
    	cerr << "Error al abrir: "<< argv[1] << endl;
    	exit(1);
    }
    else {
    	archivo >> buffer;
    	cout << archivo << endl;
    }

	int x = 4;
	int y = 2;
	int i;
	//cout<< x + y << " "<< x*y <<endl;
	struct train a;
	a.id = 2;
	*/
	{
	FILE		*pArchivo;
	vector<train_category>  tc;
	char 		*token;
	str_linea 	linea;
	int 		i=0;
 
	//Abro el archivo en modo lectura
	pArchivo=fopen("myInstance/trainCategories.csv","rt");
	if(pArchivo==NULL){
		cout<<"No se pudo abrir el Archivo de Entrada.\n";
	}else{
			cout<<"El archivo se abrio correctamente.\n";
	}
	//Parseo el archivo hasta la marca de fin de archivo
	//y voy guardando los registros en el vector de registros
	cout <<"antes while"<<endl; 
	while (!feof(pArchivo)){
		
		cout<<"Dentro del while"<<endl;
		cout<<"id"<<endl;
		token = strtok(linea,";");
		cout<<token<<endl;
		//tc[i].id = atoi(token);
 		tc[i].id = token;
 		
 		cout<<"length"<<endl;
		token = strtok(NULL,";");
		tc[i].length = atoi(token);
		
		token = strtok(linea,";");
 		tc[i].catGroup = token;
 		
 		cout<<"maxDBM"<<endl;
		token = strtok(NULL,";");
		tc[i].maxDBM = atoi(token);
 		
 		cout<<"maxTBM"<<endl;
		token = strtok(NULL,";");
		tc[i].maxTBM = token;
 		
		token = strtok(NULL,";");
		tc[i].maintTimeD = token;

		token = strtok(NULL,";");
		tc[i].maintTimeT = token;
		i++;

	}
	//Cierro el archivo
	fclose(pArchivo);
	//Muestro parte del contenido para verificar el correcto parseo
	for(i=0;i<2;i++)
	{
		cout<<"id: "<<tc[i].id<<endl;
		cout<<"length: "<<tc[i].length<<endl;
		cout<<"maxDBM: "<<tc[i].maxDBM<<endl;
		cout<<"maxTBM: "<<tc[i].maxTBM<<endl;
		cout<<"maintTimeD: "<<tc[i].maintTimeD<<endl;;

		cout<<setw(8)<<setfill('-')<<"-"<<endl;
	}
	getchar();
}
return 0;
}