#ifndef  GENCBFVAR_H
#define GENCBFVAR_H
#include <iostream>
#include <ctime>
#include <string> 
#include <cstdarg>
#include <fstream>
#include <io.h>

namespace genCBF{
	//Functions Used Outside CPP
	extern void (*errorReturn)(const char *fmt,  va_list ap);
	//Functions Used && Define Inside CPP
	void ErrorReturn(const char *fmt, va_list ap);
	void printLog(std::string st);
	void outLog(const std::string &st);
	//Functions Defined In Hub
	void genGLUTswitch(short int i);
};
#endif
