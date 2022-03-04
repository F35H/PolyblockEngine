#ifndef  CBF_H
#define CBF_H
#include "gameScenesCBF.h"


namespace InitCBF{
	extern void (*display)();
	
	void settings();
	void vars();
	void Display();	};

namespace GameCBF{
	void Init();
};
namespace GenCBF{
	extern void (*display)();
	
	void mainMenCBF();
	
	void Display();
};
#endif
