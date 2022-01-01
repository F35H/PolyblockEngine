#include "gameio.h"

using namespace base;

//Function Pointers
void  (*errorReturn)(const char *fmt, va_list ap) =[](const char* fmt,va_list ap)
{
	printLog(throwString,(std::string)fmt, clocker);
};
void (*failedOutLog)() = []()
{
	glutLeaveMainLoop ();
	glutInitWindowSize(genBakeVar.find(WINDOWWIDTH)->second * FAILEDLOGFRAC,genBakeVar.find(WINDOWHEIGHT)->second * FAILEDLOGFRAC);
	glutInitWindowPosition(genBakeVar.find(CENTERWIDTH)->second,genBakeVar.find(CENTERHEIGHT)->second);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutCreateWindow(FAILEDLOGNAME);
	glutMainLoop();
};
void (*outLog)(std::string throwString, std::ofstream &out) = [](std::string throwString, std::ofstream &out)
{
	
	out.open ("ErrLog.txt");
	if(out.is_open())
	{
		out << throwString;
	}
//	else 
//	(
	//failedOutLog 
//	);
	out.close();
};

//Functions
void printLog(std::string &throwString, time_t &time)
{
	time = std::time(0);
	throwString = std::ctime(&time);
	throwString += "\n";
	std::cout << throwString;
};
void printLog(std::string &throwString,const std::string st, time_t &time)
{
	time = std::time(nullptr);
	throwString = std::ctime(&time);
	throwString += ":";
	throwString += st;
	throwString += "\n";
	std::cout << throwString;
};
