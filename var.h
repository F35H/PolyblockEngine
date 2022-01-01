#ifndef  VAR_H
#define VAR_H
#include "GL/freeglut.h"
#include "game_def.h"
#include <string>
#include <stdarg.h>
#include <ostream>
#include <fstream>
#include <iostream>
#include <stdarg.h>
#include <ctime>
#include <map>


//Namespaces
namespace base{};
//Type Defines
#define SI signed short
#define I signed int
#define LI signed long
#define LLI signed long long
#define PSI unsigned short
#define PI unsigned int
#define PLI unsigned long
#define PLLI unsigned long long
//Variable Defines

//Put Generic Static Vunctions Here
static std::string throwString;
//signed ints;
static signed short si;
static signed int i;
//static signed long li;
//static signed long long lli;
//unsigned ints;
static unsigned short psi;
//static unsigned int pi;
//static unsigned long pli;
//static unsigned long long plli;
//streams
//static std::fstream inout;
//static std::ifstream in;
static std::ofstream out;
static time_t clocker;
//Game Varibles
static std::map<std::string,SI> genBakeVar;
//General Defines
#endif
