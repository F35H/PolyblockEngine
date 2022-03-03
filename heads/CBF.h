#ifndef  CBF_H
#define CBF_H
#include "errorhandling.h"


namespace InitCBF{
	extern void (*display)();
	
	void settings();
	void vars();
	void Display();	};
	
namespace MainMenCBF{
	struct menBut{
		short xPos, yPos,
			width, height, winInt;
		
		void constr(float yMul); };
	
	
	extern void (*display)();
	extern void (*mouse)(int but, int state, int xPos, int yPos);
	
	void Mouse(int but, int state, int xPos, int yPos);
	void Display();
	
	void Init();
	void resize();
	
	
	void winFunc();
};
namespace GameCBF{
};
namespace GenCBF{
};
#endif
