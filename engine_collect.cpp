<<<<<<< HEAD
#include "engine_collect.h"
void eDisplay = [](){};

void (*initCallBackFunctions)(const std::map<std::string,SI> &genBakeVar) = [](const std::map<std::string,SI> &genBakeVar){
		glutInitWindowSize(genBakeVar.find(WINDOWWIDTH)->second,genBakeVar.find(WINDOWHEIGHT)->second);
		glutInitWindowPosition(genBakeVar.find(CENTERWIDTH)->second,genBakeVar.find(CENTERHEIGHT)->second);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
		glutCreateWindow(GAME_NAME);
		glutDisplayFunc(display);
};
void eGameCallBackFunctions = [](const std::map<std::string,SI> &genBakeVar){
};
eGenCallBackFunctions = [](const std::map<std::string,SI> &genBakeVar){
		glutInitErrorFunc(base::errorReturn);
		glutTimerFunc(CLOCKUP, base::timerUpdate, 0);
		glutTimerFunc(LOGTIME, base::timerUpdate, 0); 
		
		glutMainLoop();
};
void engine(int argc, char** argv){
	using namespace base;
	if(bakeConfigure){
	glutInit(&argc,argv);
	initCallBackFunctions;
	gameCallBackFunctions;
	genCallBackFunctions;
	}
};
=======
#include "engine_collect.h"
void eDisplay = [](){};

void (*initCallBackFunctions)(const std::map<std::string,SI> &genBakeVar) = [](const std::map<std::string,SI> &genBakeVar){
		glutInitWindowSize(genBakeVar.find(WINDOWWIDTH)->second,genBakeVar.find(WINDOWHEIGHT)->second);
		glutInitWindowPosition(genBakeVar.find(CENTERWIDTH)->second,genBakeVar.find(CENTERHEIGHT)->second);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
		glutCreateWindow(GAME_NAME);
		glutDisplayFunc(display);
};
void eGameCallBackFunctions = [](const std::map<std::string,SI> &genBakeVar){
};
eGenCallBackFunctions = [](const std::map<std::string,SI> &genBakeVar){
		glutInitErrorFunc(base::errorReturn);
		glutTimerFunc(CLOCKUP, base::timerUpdate, 0);
		glutTimerFunc(LOGTIME, base::timerUpdate, 0); 
		
		glutMainLoop();
};
void engine(int argc, char** argv){
	using namespace base;
	if(bakeConfigure){
	glutInit(&argc,argv);
	initCallBackFunctions;
	gameCallBackFunctions;
	genCallBackFunctions;
	}
};
>>>>>>> ddcba64 (MakeUpdate)
