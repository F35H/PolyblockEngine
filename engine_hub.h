#ifndef  ENGINE_HUB_H
#define ENGINE_HUB_H
#include "errorhandling.h"

	void engine(int argc,char **argv);
	
namespace hub{
	//Function Variables
	extern void (*display)();
	// Variable Definitions
	void Display();
	//General Functions
	void initCallBackFunctions(int argc,char **argv);
	void gameCallBackFunctions();
	void genCallBackFunctions();	};
#endif
