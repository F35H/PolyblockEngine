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

#include <iostream>
#include <fstream>
#include <map>
#include <ctime>
#include <cstdarg>
#include <io.h>
#include <vector>

#include "ft2build.h"
#include "GL/freeglut.h"

#include FT_FREETYPE_H

#define GAME_NAME "GAME_NAME"

namespace game_values{
	enum osCheck : short	{
		win64,	win32,
		unix,	mac,	
			badOS	};
	
	enum sceneCheck : short {
		mainMen, playScreen };
	
	extern short scene;
	extern short window;
	extern FT_Library library;
}

#endif
