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
    glDisable(GL_LIGHTING);
    glColor3d(0.1,0.1,0.4);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	};
void errorHan::ErrorReturn(const char *fmt,  va_list ap)	{	
	printLog((std::string)fmt);	};

void errorHan::ErrorReturn(const char *fmt)	{	
	printLog((std::string)fmt);	};
void errorHan::errorSwitch(short int i, std::string st){ 
	switch(i)	{
		default:	{
				return;	}	
		case 0:	{	char b[FILENAME_MAX];
			CompileDirectory(b,FILENAME_MAX);
			std::string c(b);	c += "\\txt";	mkdir(c.c_str());
				return;	}
		case 1:	{	glutDestroyWindow(game_values::window);
			glutCreateWindow("GAME_ERROR");
			glutReshapeWindow(glutGet(GLUT_SCREEN_WIDTH) * 
				(0.4),glutGet(GLUT_SCREEN_HEIGHT)*(0.4));
			glutPositionWindow(glutGet(GLUT_SCREEN_WIDTH) * 
				(0.4),glutGet(GLUT_SCREEN_HEIGHT)*(0.4));
			glutDisplayFunc(errorHan::display);
			st += "\n\n A Severe Error Has Occured. \n\n";
			st += "A good bug report needs to convey three things:";
			st += "\n How to reproduce the bug, as precisely as possible, and how often this will make the bug appear";
			st +=  "\n What should have happened, at least in your opinion";
			st += "\n What actually happened, or at least as much information as you have recorded";
			st += "\n \n Please Submit an Error Report to the Developer using the Error Log provided under the txt folder in the program directory, \n a Brief Explanation of What Happened, \n How to Reproduce the Bug, \n and What Should Have Happened. Thank You.";
			const unsigned char* throwString = (const unsigned char*)st.c_str();
			glPushMatrix();
			glLoadIdentity();
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0.0, GLUT_WINDOW_WIDTH, 0.0, GLUT_WINDOW_HEIGHT, -1, 1);
			glColor3f(1.0f,1.0f,1.0f);
			glRasterPos2i(41,60);
			glutBitmapString(GLUT_BITMAP_HELVETICA_10, throwString);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
			glutSwapBuffers();
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
				case 0: {	errorSwitch(i, st);	
						goto def;	}
				case 1: {	errorSwitch(i, st);	
						break;	}	}	}	}	out.close();	};

