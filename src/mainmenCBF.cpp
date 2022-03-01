#include "CBF.h"


namespace MainMenCBF{
 short* win = {
	  new subwin(1.5),
		new subwin(1.25),
		new subwin(1),
		new subwin(0.75),
		new subwin(0.5) }; };

void (*MainMenCBF::display)() = {	MainMenCBF::Display	};
//void (*MainMenCBF::display)() = {	MainMenCBF::Display	};
//void (*MainMenCBF::display)() = {	MainMenCBF::Display	};

void MainMenCBF::subwin::subwin(yMul) {
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
