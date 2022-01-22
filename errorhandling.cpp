#include "errorhandling.h"


bool (*errorHan::initCheck)()	=	{	errorHan::InitCheck	};
void (*errorHan::display)()	=	{	errorHan::Display	};
void (*errorHan::errorReturn)(const char *fmt,  va_list ap)
	=	{	errorHan::ErrorReturn	};

bool errorHan::InitCheck()	{	
	if (	FT_Init_FreeType( &game_values::library ) 	)	{
		errorHan::ErrorReturn("FreeType Library Failed");
			return false;	}
	else if (	Check == game_values::check::bados	)	{
		errorHan::ErrorReturn("Unidentified Operating System!");
			return false; }
	else if (	true	)	{
			return true;	}	};	
void errorHan::Display()	{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	};
void errorHan::ErrorReturn(const char *fmt,  va_list ap)	{	
	printLog((std::string)fmt);	};

void errorHan::ErrorReturn(const char *fmt)	{	
	printLog((std::string)fmt);	};
void errorHan::errorSwitch(short int i){
	switch(i)	{
		default:	{
				return;	}	
		case 0:	{	char b[FILENAME_MAX];
			CompileDirectory(b,FILENAME_MAX);
			std::string c(b);	c += "\\txt";	mkdir(c.c_str());
				return;	}
		case 1:	{	glutDestroyWindow(game_values::window);
			glutCreateWindow("GEN_GAME_ERROR");
			glutReshapeWindow(400,200);
			glutPositionWindow(glutGet(GLUT_SCREEN_WIDTH) * 
				(0.4),glutGet(GLUT_SCREEN_HEIGHT)*(0.4));
			glutDisplayFunc(errorHan::display);	glutMainLoop();
				return;	}	}	};
void errorHan::printLog(const std::string st)	{
	time_t time;	std::string throwString;
	time = std::time(nullptr);	throwString = std::ctime(&time);
	throwString += ":";		throwString += st;	throwString += "\n";
	std::cout << throwString;	outLog(throwString);	};
void errorHan::outLog(const std::string &st)	{
	std::ofstream out;	out.open("txt/ErrLog.txt", std::ios::out);
	for (	int i = -1; i != 2; i++	)	{
		if (	out.tellp() == 0	)	{
			out << st << std::endl;
				return;	}
		else if (	true	)	{
			switch(i)	{	
				default:	def:	{	out.close();
					out.open("txt/ErrLog.txt",	std::ios::out |	std::ios::trunc | std::ios::ate);
						break;	}
				case 0: {	errorSwitch(i);	
						goto def;	}
				case 1: {	errorSwitch(i);	
						break;	}	}	}	}	out.close();	};

