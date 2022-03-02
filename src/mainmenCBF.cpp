#include "CBF.h"


namespace MainMenCBF{
	const menBut* const win[] = {
		new menBut(1.5),
		new menBut(1.25),
		new menBut(1.0),
		new menBut(0.75),
		new menBut(0.5) }; 
	const auto* const winArr = win; };

//void (*MainMenCBF::display)() = {	MainMenCBF::Display	};
void (*MainMenCBF::mouse)(int but, int state, int x, int y) = {	MainMenCBF::Mouse	};
//void (*MainMenCBF::display)() = {	MainMenCBF::Display	};

MainMenCBF::menBut::menBut(float yMul) {
	xPos = glutGet(GLUT_SCREEN_WIDTH)/3;
	yPos = glutGet(GLUT_SCREEN_HEIGHT)/2;
	
	width = glutGet(GLUT_SCREEN_WIDTH)*0.333;
	height = glutGet(GLUT_SCREEN_HEIGHT)*(0.1);
	
	winInt = glutCreateSubWindow(game_values::window, xPos, (yPos*yMul), width, height); };

void MainMenCBF::Init(){
	
};
void MainMenCBF::Mouse(int but, int state, int xPos, int yPos){
	using namespace game_values;
	
	switch(glutGetWindow()) {
		default: std::cout << "Default" << std::endl;
		case (sceneCheck::playScreen): {
			scene = playScreen;
			break; } };
};
//void MainMenCBF::Display(){
//};
void MainMenCBF::resize(){
};
