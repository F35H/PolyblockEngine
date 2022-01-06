#ifndef BAKE_H
#define BAKE_H
#include	"var.h"

namespace base{
//Function Pointers

extern void (*getScreenSize)(PSI &psi,SI &si);
extern bool (*bakeConfigure)(std::map<const char*,SI> &genBakeVar, PSI &psi);
//Functions
//Variables
}
#endif

