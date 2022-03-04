#include "engine_hub.h"


void engine(int argc,char **argv)	{	
	using namespace hub;
	using namespace game_values;
	
	glutInit(&argc,argv);
	
	if (errorHan::initCheck) {
		initCallBackFunctions(argc,argv);
		gameCallBackFunctions();
		genCallBackFunctions();
		glutMainLoop();	}; };
		
void hub::initCallBackFunctions(int argc, char**argv){
	using namespace InitCBF;
	settings();
	vars();
	glutDisplayFunc(display); };
	
void hub::gameCallBackFunctions() {
	using namespace GameCBF;
	Init();
	
	glutDisplayFunc(display); };
void hub::genCallBackFunctions()	{
//	glutInitErrorFunc(errorHan::errorReturn);
//	glutTimerFunc(CLOCKUP, timerUpdate, 0);
//	glutTimerFunc(LOGTIME, timerUpdate, 0);
	};


