#include "CBF.h"


namespace MainMenCBF{
 win = {
	  subwin::subwin(1.5),
		subwin::subwin(1.25),
		subwin::subwin(1),
		subwin::subwin(0.75),
		subwin::subwin(0.5) }; };

void (*MainMenCBF::display)() = {	MainMenCBF::Display	};
//void (*MainMenCBF::display)() = {	MainMenCBF::Display	};
//void (*MainMenCBF::display)() = {	MainMenCBF::Display	};

void MainMenCBF::subwin::subwin(yMul) {
	this.x = glutGet(GLUT_SCREEN_WIDTH)/3;
	this.y = glutGet(GLUT_SCREEN_HEIGHT)/2;
	
	short width = glutGet(GLUT_SCREEN_WIDTH)*0.333;
	this.height = glutGet(GLUT_SCREEN_HEIGHT)*(0.1);
	
	return glutCreateSubWindow(game_values::window, x, (y*yMul), width, height); };

void MainMenCBF::Init(){
	winFunc();	
};
void MainMenCBF::mouse(){
};
void MainMenCBF::Display(){
};
void MainMenCBF::resize(){
};
