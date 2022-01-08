#include "engine_hub.h"



void engine(int argc,char **argv)	{	using namespace hub;	badOS:
	if	(	Check != "BADOS" )	{	initCallBackFunctions(argc,argv);
	gameCallBackFunctions();	genCallBackFunctions();	}
	else	{	errorHan::ErrorReturn("BADOS");	goto badOS;	}	};
void hub::initCallBackFunctions(int argc, char**argv){	glutInit(&argc,argv);
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_WIDTH));
	glutInitWindowPosition(0,0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutCreateWindow(GAME_NAME);
	window = glutGetWindow();
//	glutDisplayFunc(display);	
};
void hub::gameCallBackFunctions(){};
void hub::genCallBackFunctions(){	glutInitErrorFunc(errorHan::errorReturn);
//		glutTimerFunc(CLOCKUP, timerUpdate, 0);
//		glutTimerFunc(LOGTIME, timerUpdate, 0);
		glutMainLoop();	};
void hub::Display()	{	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	};
void (*hub::display)() = {	hub::Display	};
