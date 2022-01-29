#ifndef  ERRORHANDLING_H
#define ERRORHANDLING_H
#include "game_values.h"


	namespace errorHan{
	//Function variables
	extern bool (*initCheck)();
	extern void (*display)();
	extern void (*errorReturn)(const char *fmt,  va_list ap);
	//Function Variable Definitions
	bool InitCheck();
	void Display();
	void ErrorReturn(const char *fmt);
	void ErrorReturn(const char *fmt, va_list ap);
	//Internal Functions
	void printLog(std::string st);
	void errorSwitch(short int i, std::string st);
	void outLog(const std::string &st);	};
#endif
