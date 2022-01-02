#ifndef  ENGINE_HUB_H
#define ENGINE_HUB_H
#include "GL/freeglut.h"
#include "game_values.h"
#include "initCBFvar.h"
#include "gameCBFvar.h"
#include "genCBFvar.h"



	
//extern void (*errorReturn)(const char *fmt,  va_list ap);
void engine(int argc,char **argv);
void initCallBackFunctions(int argc,char **argv);
void gameCallBackFunctions();
void genCallBackFunctions();
#endif
