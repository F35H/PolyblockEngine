#ifndef GAMEIO_H
#define GAMEIO_H
#include "bake.h"

namespace base{
//Function Pointers
extern void (*errorReturn)(const char *fmt, va_list ap);
extern void (*outLog)(std::string throwString, std::ofstream &out);
extern void (*failedOutLog)();
//Functions
void printLog(std::string &throwString, time_t &time);
void printLog(std::string &throwString,const std::string st, time_t &time);
//Variables
};
#endif


