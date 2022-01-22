#ifndef  GAME_VALUES_H
#define GAME_VALUES_H

#ifdef _WIN64
#include <direct.h>
#define CompileDirectory _getcwd
#define Check 0
#elif _WIN32
#include <direct.h>
#define CompileDirectory _getcwd
#define Check 1
#elif unix
#include <unistd.h>
#define CompileDirectory getcwd
#define Check 2
#elif __MACH__
#include <unistd.h>
#define CompileDirectory getcwd
#define Check 3
#else
#define CompileDirectory getcwd
#define Check 4
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
	enum check : short	{
		win64,	win32,
		unix,	mac,	
			bados	};
	
	extern int window;
	extern FT_Library library;
}

#endif
