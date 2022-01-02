#include "genCBFvar.h"


void genCBF::ErrorReturn(const char *fmt,  va_list ap){

printLog((std::string)fmt);
	
};
void (*errorReturn)(const char *fmt,  va_list ap) = genCBF::ErrorReturn;
void genCBF::printLog(const std::string st)
{
	time_t time;  std::string throwString;
	time = std::time(nullptr);
	throwString = std::ctime(&time);
	throwString += ":";
	throwString += st;
	throwString += "\n";
	std::cout << throwString;
};

