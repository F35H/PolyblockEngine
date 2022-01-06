#ifndef  ERRORHANDLING_H
#define ERRORHANDLING_H
#include "game_values.h"


extern int window;
//extern void (*display)();
	namespace errorHan{
	//Functions Used Outside CPP
	extern void (*errorReturn)(const char *fmt,  va_list ap);
	extern void (*display)();
	void Display();
	//Functions Used && Define Inside CPP
	void ErrorReturn(const char *fmt, va_list ap);
	void printLog(std::string st);
	void outLog(const std::string &st);
	//Functions Defined In Hub
	void genGLUTswitch(short int i);
	};
#endif
