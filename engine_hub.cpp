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
	using InitCBF;
	Set();
	game_values::window =
		glutCreateWindow(GAME_NAME);
	glutDisplayFunc(display);	
};
void hub::mainMenCallBackFunctions(){
	MainMenCBF::Init();	};
void hub::gameCallBackFunctions()	{};
void hub::genCallBackFunctions()	{
	glutInitErrorFunc(errorHan::errorReturn);
//	glutTimerFunc(CLOCKUP, timerUpdate, 0);
//	glutTimerFunc(LOGTIME, timerUpdate, 0);
	};


