#ifndef  GENCBFVAR_H
#define GENCBFVAR_H
#include <string>
#include <ctime>
#include <stdarg.h>
#include <iostream>

extern void (*errorReturn)(const char *fmt,  va_list ap);
namespace genCBF{
	void ErrorReturn(const char *fmt, va_list ap);
	void printLog(std::string);
};


#endif
