#ifndef  GAME_VALUES_H
#define GAME_VALUES_H

#ifdef _WIN64
#include <direct.h>
#define CompileDirectory _getcwd
#define Check true
#elif _WIN32
#include <direct.h>
#define CompileDirectory _getcwd
#define Check true
#elif unix
#include <unistd.h>
#define CompileDirectory getcwd
#define Check true
#elif __MACH__
#include <unistd.h>
#define CompileDirectory getcwd
#define Check true
#else
#define CompileDirectory getcwd
#define Check false
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
	
	enum sceneCheck : short {
		mainMen, playScreen };
	
	extern short scene;
	extern short window;
	extern FT_Library library;
}

#endif
