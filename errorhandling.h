#ifndef  ERRORHANDLING_H
#define ERRORHANDLING_H
#include "game_values.h"


//extern int window;
//extern void (*display)();
	namespace errorHan{
	//Function variables
	extern bool (*libInit)();
	extern void (*display)();
	extern void (*errorReturn)(const char *fmt,  va_list ap);
	//Function Variable Definitions
	bool LibInit();
	void Display();
	void ErrorReturn(const char *fmt);
	void ErrorReturn(const char *fmt, va_list ap);
	//Internal Functions
	void printLog(std::string st);
	void genGLUTswitch(short int i);
	void outLog(const std::string &st);	};
#endif
