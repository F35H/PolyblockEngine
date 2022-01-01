<<<<<<< HEAD
#include "gameio.h"

using namespace base;
//Function Pointers
void eErrorReturn =[](const char* fmt,va_list ap)
{
	printLog(throwString,(std::string)fmt, clocker);
};
void eFailedOutLog = []()
{
	glutLeaveMainLoop ();
	glutInitWindowSize(genBakeVar.find(WINDOWWIDTH)->second * FAILEDLOGFRAC,genBakeVar.find(WINDOWHEIGHT)->second * FAILEDLOGFRAC);
	glutInitWindowPosition(genBakeVar.find(CENTERWIDTH)->second,genBakeVar.find(CENTERHEIGHT)->second);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutCreateWindow(FAILEDLOGNAME);
	glutMainLoop();
};
void eOutLog = [](std::string throwString, std::ofstream &out)
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
ePrintLog2
{
	time = std::time(0);
	throwString = std::ctime(&time);
	throwString += "\n";
	std::cout << throwString;
	outLog;
};
ePrintLog3
{
	time = std::time(nullptr);
	throwString = std::ctime(&time);
	throwString += ":";
	throwString += st;
	throwString += "\n";
	std::cout << throwString;
	outLog;
};
=======
#include "gameio.h"

using namespace base;
//Function Pointers
void eErrorReturn =[](const char* fmt,va_list ap)
{
	printLog(throwString,(std::string)fmt, clocker);
};
void eFailedOutLog = []()
{
	glutLeaveMainLoop ();
	glutInitWindowSize(genBakeVar.find(WINDOWWIDTH)->second * FAILEDLOGFRAC,genBakeVar.find(WINDOWHEIGHT)->second * FAILEDLOGFRAC);
	glutInitWindowPosition(genBakeVar.find(CENTERWIDTH)->second,genBakeVar.find(CENTERHEIGHT)->second);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutCreateWindow(FAILEDLOGNAME);
	glutMainLoop();
};
void eOutLog = [](std::string throwString, std::ofstream &out)
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
ePrintLog2
{
	time = std::time(0);
	throwString = std::ctime(&time);
	throwString += "\n";
	std::cout << throwString;
	outLog;
};
ePrintLog3
{
	time = std::time(nullptr);
	throwString = std::ctime(&time);
	throwString += ":";
	throwString += st;
	throwString += "\n";
	std::cout << throwString;
	outLog;
};
>>>>>>> ddcba64 (MakeUpdate)
