#include "errorhandling.h"




void errorHan::genGLUTswitch(short int i){ switch(i){ 
	default:	return;	//Case 0 is error routing for outLog
	case 0:
	
	return;
	case 1:
	glutDestroyWindow(window);	glutCreateWindow("GEN_GAME_ERROR");	glutReshapeWindow(400,200);
	glutPositionWindow(glutGet(GLUT_SCREEN_WIDTH)*(0.4),glutGet(GLUT_SCREEN_HEIGHT)*(0.4));
	glutDisplayFunc(errorHan::display);
	glutMainLoop();
	return;	}	};
void errorHan::Display(){	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	};
void (*errorHan::display)() = errorHan::Display;

void (*errorHan::errorReturn)(const char *fmt,  va_list ap) = {	errorHan::ErrorReturn	};
void errorHan::ErrorReturn(const char *fmt,  va_list ap){	printLog((std::string)fmt);	};
void errorHan::printLog(const std::string st){	time_t time;  std::string throwString;
	time = std::time(nullptr);	throwString = std::ctime(&time);
	throwString += ":";	throwString += st;	throwString += "\n";
	std::cout << throwString;	outLog(throwString);	};
void errorHan::outLog(const std::string &st){	std::ofstream out;	
	out.open("txt/ErrLog.txt", std::ios::out);	int i;	openTest:
	if (	out.eof()	)	{	out << st << std::endl;	}
	else	{	switch(i)	{	i =0;	default:	out.close(); 
	out.open("txt/ErrLog.txt", std::ios::trunc);	goto openTest;
	case 0: {	genGLUTswitch(i);	i++;	}	break;
	case 1: {	genGLUTswitch(i);	}	break;	}	}	out.close();	};
		
