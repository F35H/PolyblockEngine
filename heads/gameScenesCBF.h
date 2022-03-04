#ifndef  gameScenesCBF_H
#define gameScenesCBF_H

#include "errorHandling.h"

namespace MainMenCBF{
	vector<menBut*> win;
	
	struct menBut{
		short xPos, yPos,
			width, height, winInt;
		
		menBut(float yMul); };
	
	extern void (*display)();
	extern void (*mouse)(int but, int state, int xPos, int yPos);
	
	void Mouse(int but, int state, int xPos, int yPos);
	void Display();
	void Resize();
	
	void resize(); };

#endif
