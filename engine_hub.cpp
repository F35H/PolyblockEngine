#include "engine_hub.h"

namespace hub{
int	window = 0;
};
void engine(int argc,char **argv){	using namespace hub;
	initCallBackFunctions(argc,argv);
	gameCallBackFunctions();
	genCallBackFunctions();	};
void hub::initCallBackFunctions(int argc, char**argv){	glutInit(&argc,argv);
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH),glutGet(GLUT_SCREEN_WIDTH));
	glutInitWindowPosition(0,0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutCreateWindow(GAME_NAME);
	window = glutGetWindow();
//	glutDisplayFunc(display);
	};
void hub::gameCallBackFunctions(){};
void hub::genCallBackFunctions(){	glutInitErrorFunc(genCBF::errorReturn);
//		glutTimerFunc(CLOCKUP, timerUpdate, 0);
//		glutTimerFunc(LOGTIME, timerUpdate, 0);
		glutMainLoop();
};
void genCBF::genGLUTswitch(short int i){ switch(i){ 
	default:	return;	//Case 0 is error routing for outLog
	case 0:
	glutDestroyWindow(hub::window);	glutCreateWindow("GEN_GAME_ERROR");	glutReshapeWindow(400,200);
	glutPositionWindow(glutGet(GLUT_SCREEN_WIDTH)*(0.4),glutGet(GLUT_SCREEN_HEIGHT)*(0.4));
	glutDisplayFunc(hub::display);
	glutMainLoop();
	return;	}	};
void (*hub::display)() =	{hub::Display};
void hub::Display()
{
	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
