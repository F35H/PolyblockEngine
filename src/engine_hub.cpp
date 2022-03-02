#include "engine_hub.h"


void engine(int argc,char **argv)	{	
	using namespace hub;
	using namespace game_values;
	
	if	(	errorHan::initCheck )	{
		glutInit(&argc,argv);
		initCallBackFunctions(argc,argv);
		switch( scene ) {
			case sceneCheck::mainMen: { mainMenCallBackFunctions(); 
			break; };
		gameCallBackFunctions();
		genCallBackFunctions();
		glutMainLoop();	} }; };
		
void hub::initCallBackFunctions(int argc, char**argv){
	using namespace InitCBF;
	
	settings();
	vars();
	
	glutDisplayFunc(display);	
};

void hub::mainMenCallBackFunctions(){
	using namespace MainMenCBF;
	
	try{
		Init();
		
		glutReshapeFunc(NULL);
		glutMouseFunc(mouse); }
		catch(std::exception &e) 
		{ std::string st = e.what(); st += "Found in Main Menu";
			errorHan::printLog(st); };	};
	
	
void hub::gameCallBackFunctions()	{};
void hub::genCallBackFunctions()	{
	glutInitErrorFunc(errorHan::errorReturn);
//	glutTimerFunc(CLOCKUP, timerUpdate, 0);
//	glutTimerFunc(LOGTIME, timerUpdate, 0);
	};


