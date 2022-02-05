#include "CBF.h"

void MainMenCBF::Init(){
	subwin::win1 = glutCreateSubWindow(game_values::window, 40,40, 100, 100);
	glutReshapeFunc(NULL);
	glutMouseFunc(NULL);	
};
void MainMenCBF::mouse(){
};
void MainMenCBF::resize(){
};
