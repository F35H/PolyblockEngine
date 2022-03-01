#ifndef  CBF_H
#define CBF_H
#include "errorhandling.h"


namespace InitCBF{
	extern void (*display)();

	void settings();
	void Display();	};
namespace MainMenCBF{
	struct menBut{
		short xPos, yPos,
			width, height, win; };
	
	
	extern void (*display)();
	
	void Init();
	void resize();
	void mouse();
	
	void winFunc();
};
namespace GameCBF{
};
namespace GenCBF{
};
#endif
