#ifndef  ENGINE_HUB_H
#define ENGINE_HUB_H
#include "hubCBF.h"
void engine(int argc,char **argv);
	
namespace hub{
	void initCallBackFunctions(int argc,char **argv);
	void gameCallBackFunctions();
	void genCallBackFunctions();	};
#endif
