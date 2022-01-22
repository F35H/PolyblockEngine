#ifndef  GAME_VALUES_H
#define GAME_VALUES_H

#ifdef _WIN64
#include <direct.h>
#define CompileDirectory _getcwd
#define Check "WIN64"
#elif _WIN32
#include <direct.h>
#define CompileDirectory _getcwd
#define Check "WIN32"
#elif unix
#include <unistd.h>
#define CompileDirectory getcwd
#define Check "LINUX"
#elif __MACH__
#include <unistd.h>
#define CompileDirectory getcwd
#define Check "MAC"
#else
#define CompileDirectory getcwd
#define Check "BADOS"
#endif

#include "ft2build.h"
#include "GL/freeglut.h"
#include <iostream>
#include <fstream>
#include <map>
#include <ctime>
#include <cstdarg>
#include <io.h>

#include FT_FREETYPE_H

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

namespace game_values{
extern int window;
extern FT_Library library;
}

#endif
