#ifndef  ENGINE_HUB_H
#define ENGINE_HUB_H
#include "error_handling.h"




void engine(int argc,char **argv);
namespace hub{
	extern void (*display)();
//	extern void (*errorReturn)(const char *fmt,  va_list ap);
	void Display();
	void initCallBackFunctions(int argc,char **argv);
	void gameCallBackFunctions();
	void genCallBackFunctions();	};
#endif
