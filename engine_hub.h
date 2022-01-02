#include "GL/freeglut.h"


struct gameVar{
	
	std::map<std::string,short int> initCBFvar;
	std::map<std::string,short int> gameCBFvar;
	std::map<std::string,short int> genCBFvar;
	
	gameVar();
}

void engine(int argc,char **argv, gameVar vars);
void initCallBackFunctions(int argc,char **argv, gameVar vars);
void gameCallBackFunctions();
void genCallBackFunctions();
