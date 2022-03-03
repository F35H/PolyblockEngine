#include "CBF.h"


namespace MainMenCBF{
 menBut* win[] = {
		new menBut,
		new menBut,
		new menBut,
		new menBut,
		new menBut }; };

void (*MainMenCBF::display)() = {	MainMenCBF::Display	};
void (*MainMenCBF::mouse)(int but, int state, int x, int y) = {	MainMenCBF::Mouse	};
//void (*MainMenCBF::display)() = {	MainMenCBF::Display	};
	
void MainMenCBF::menBut::constr(float yMul) {
	xPos = glutGet(GLUT_SCREEN_WIDTH)/3;
	yPos = glutGet(GLUT_SCREEN_HEIGHT)/2;
	
	width = glutGet(GLUT_SCREEN_WIDTH)*0.333;
	height = glutGet(GLUT_SCREEN_HEIGHT)*(0.1);
	
	winInt = glutCreateSubWindow(game_values::window,
		xPos, (yPos*yMul), width, height); };

void MainMenCBF::Init(){
	float i_ = 0.5;
	for (short i = 0;i <= 4; i++) {
		win[i]->constr(i_);
		glutDisplayFunc(display);
		i_ += 0.25; }; 	};
		
void MainMenCBF::Mouse(int but, int state, int xPos, int yPos){
	using namespace game_values;
	
	switch(glutGetWindow()) {
		default: std::cout << "Default" << std::endl;
		case (sceneCheck::playScreen): {
			scene = playScreen;
			break; } }; };
void MainMenCBF::Display(){
};
void MainMenCBF::resize(){
};
