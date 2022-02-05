#ifndef  ENGINE_HUB_H
#define ENGINE_HUB_H
#include "CBF.h"
void engine(int argc,char **argv);
	
namespace hub{
	//Function Variables
	// Variable Definitions
	//General Functions
	void initCallBackFunctions(int argc,char **argv);
	void mainMenCallBackFunctions();
	void gameCallBackFunctions();
	void genCallBackFunctions();	};
#endif
