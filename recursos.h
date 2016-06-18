#ifndef _RECURSOS_H_
#define _RECURSOS_H_


#include <iostream>
#include <cstring>

using namespace std;

struct plataform{
	string id;
	int length;
};

struct yard{
	string id;
	int capa;
};

struct facility{
	string id;
	string type;
	int length;
};

struct gate{
	string resource_id;
	string side;
	int index;
	string neighborGateResource;
	string neighborGateSide;
	int neighborGateIndex;	
};

struct train{
	string id;
	//unsigned int id;
	string category;
	string resource;
	int remDBM;
	string remTBM;
	//unsigned int remTBM;
	//bool reused;
};

struct train_category{
	string id;
	//unsigned int id;
	int length;
	string catGroup;
	//unsigned int catGroup;
	int maxDBM;
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


#endif