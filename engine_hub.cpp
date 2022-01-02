#include "engine_hub.h"
short int windowWidth;
short int windowHeight;

void engine(int argc,char **argv)
{
	initCallBackFunctions(argc,argv);
	gameCallBackFunctions();
	genCallBackFunctions();
};
void initCallBackFunctions(int argc, char**argv){
	glutInit(&argc,argv);
	glutInitWindowSize(windowWidth,windowHeight);
	glutInitWindowPosition(0,0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutCreateWindow(GAME_NAME);
//		glutDisplayFunc(display);
};
void gameCallBackFunctions(){};
void genCallBackFunctions(){
	glutInitErrorFunc(errorReturn);
//		glutTimerFunc(CLOCKUP, timerUpdate, 0);
//		glutTimerFunc(LOGTIME, timerUpdate, 0); 
		
		glutMainLoop();
};
