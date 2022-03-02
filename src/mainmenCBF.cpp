#include "CBF.h"


namespace MainMenCBF{
 const short* const win = {
	  new menBut(1.5),
		new menBut(1.25),
		new menBut(1),
		new menBut(0.75),
		new menBut(0.5) }; };

void (*MainMenCBF::display)() = {	MainMenCBF::Display	};
//void (*MainMenCBF::display)() = {	MainMenCBF::Display	};
//void (*MainMenCBF::display)() = {	MainMenCBF::Display	};

void MainMenCBF::menBut::menBut(yMul) {
	xPos = glutGet(GLUT_SCREEN_WIDTH)/3;
	yPos = glutGet(GLUT_SCREEN_HEIGHT)/2;
	
	width = glutGet(GLUT_SCREEN_WIDTH)*0.333;
	height = glutGet(GLUT_SCREEN_HEIGHT)*(0.1);
	
	winInt = glutCreateSubWindow(game_values::window, x, (y*yMul), width, height); };

void MainMenCBF::Init(){
	
};
void MainMenCBF::mouse(){
};
void MainMenCBF::Display(){
};
void MainMenCBF::resize(){
};
