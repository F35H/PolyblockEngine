#ifndef ENGINE_COLLECT_H
#define ENGINE_COLLECT_H
#include "timers.h"


//Functions
void engine(int argc, char** argv);

namespace base{
//Function Pointers
extern void (*initCallBackFunctions)(const std::map<std::string,SI> &genBakeVar);
extern void  (*gameCallBackFunctions)(const std::map<std::string,SI> &genBakeVar);
extern void  (*genCallBackFunctions)(const std::map<std::string,SI> &genBakeVar);
extern void   (*display) ();

typedef void (*egg) ();
egg Egg;
//Variables
}
#endif
