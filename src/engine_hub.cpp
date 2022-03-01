#include "engine_hub.h"


void engine(int argc,char **argv)	{	
	using namespace hub;
	if	(	errorHan::initCheck )	{
		glutInit(&argc,argv);
		initCallBackFunctions(argc,argv);
		mainMenCallBackFunctions();
		gameCallBackFunctions();
		genCallBackFunctions();
		glutMainLoop();	} };
		
void hub::initCallBackFunctions(int argc, char**argv){
	using namespace InitCBF;
	
	settings();
	vars();
	
	glutDisplayFunc(display);	
};

void hub::mainMenCallBackFunctions(){
	using namespace MainMenCBF;
	
	init();
	
	glutDisplayFunc(display);
	glutReshapeFunc(NULL);
	glutMouseFunc(NULL);	};
	
	
void hub::gameCallBackFunctions()	{};
void hub::genCallBackFunctions()	{
	glutInitErrorFunc(errorHan::errorReturn);
//	glutTimerFunc(CLOCKUP, timerUpdate, 0);
//	glutTimerFunc(LOGTIME, timerUpdate, 0);
	};


