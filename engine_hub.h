#ifndef  ENGINE_HUB_H
#define ENGINE_HUB_H
#include "errorhandling.h"




	extern int window;
	void engine(int argc,char **argv);
namespace hub{
	extern void (*display)();
	void Display();
	void initCallBackFunctions(int argc,char **argv);
	void gameCallBackFunctions();
	void genCallBackFunctions();	};
#endif
