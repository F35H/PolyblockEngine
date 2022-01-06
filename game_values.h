#ifndef  GAME_VALUE_H
#define GAME_VALUE_H

#ifdef _WIN64
#include <direct.h>
#define CompileDirectory _getcwd
#elifdef _WIN32
#include <direct.h>
#define CompileDirectory _getcwd
#elifdef unix
#include <unistd.h>
#define CompileDirectory getcwd
#elifdef __MACH__
#include <unistd.h>
#define CompileDirectory getcwd
#else
#define CompileDirectory badOS
#endif

#include "GL/freeglut.h"
#include "initCBFvar.h"
#include "gameCBFvar.h"
#include "genCBFvar.h"
#include <iostream>
#include <fstream>
#include <map>
#include <ctime>
#include <cstdarg>
#include <io.h>


//Graphical Names
#define WINDOWHEIGHT	"HEIGHT"
#define WINDOWWIDTH	"WIDTH"
#define CENTERHEIGHT	"CENTERY"
#define CENTERWIDTH	"CENTERX"
//Engine Statistics
#define LOGTIME 600000 //10 Minutes
#define CLOCKUP 60000 //1 minute
//Game Statistics
#define GAME_NAME "GAME_NAME"
//Error Statistics
#define FAILEDLOGNAME "FAILED LOG ERROR"


#endif
