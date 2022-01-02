#include "engine_collect.h"
void (*display)() = [](){};

void (*initCallBackFunctions)(const std::map<std::string,SI> &genBakeVar) = [](const std::map<std::string,SI> &genBakeVar){

};
void  (*gameCallBackFunctions)(const std::map<std::string,SI> &genBakeVar) = [](const std::map<std::string,SI> &genBakeVar){
};
 void (*genCallBackFunctions)(const std::map<std::string,SI> &genBakeVar) = [](const std::map<std::string,SI> &genBakeVar){

};
void engine(int argc, char** argv){
	using namespace base;
	if(bakeConfigure){
	glutInit(&argc,argv);
	initCallBackFunctions;
	gameCallBackFunctions;
	genCallBackFunctions;
	}
};
