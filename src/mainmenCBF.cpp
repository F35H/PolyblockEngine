#include "CBF.h"

namespace MainMenCBF{
	subwin subWin;
};

void MainMenCBF::winFunc(){
	short x = glutGet(GLUT_SCREEN_WIDTH)/3;
	short y = glutGet(GLUT_SCREEN_HEIGHT)/2;
	
	short width = glutGet(GLUT_SCREEN_WIDTH)*0.333;
	short height = glutGet(GLUT_SCREEN_HEIGHT)*(0.1);
	
	subWin.win1 = glutCreateSubWindow(game_values::window, x, (y*1.5), width, height);
	subWin.win2 = glutCreateSubWindow(game_values::window, x, (y * 1.25) , width, height);
	subWin.win3 = glutCreateSubWindow(game_values::window, x, y, width, height);
	subWin.winOpt = glutCreateSubWindow(game_values::window, x, (y * 0.75), width, height);
	subWin.winExit = glutCreateSubWindow(game_values::window, x, (y * 0.5), width, height);
};

void MainMenCBF::Init(){
	winFunc();
	
	glutReshapeFunc(NULL);
	glutMouseFunc(NULL);	
};
void MainMenCBF::mouse(){
};
void MainMenCBF::resize(){
};
