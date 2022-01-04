#include "genCBFvar.h"


void (*genCBF::errorReturn)(const char *fmt,  va_list ap) = {	genCBF::ErrorReturn	};

void genCBF::ErrorReturn(const char *fmt,  va_list ap){	printLog((const std::string)fmt);	};
void genCBF::printLog(const std::string st){	time_t time;  std::string throwString;
	time = std::time(nullptr);
	throwString = std::ctime(&time);
	throwString += ":";
	throwString += st;
	throwString += "\n";
	std::cout << throwString;
	outLog(throwString);	};
void genCBF::outLog(const std::string &st){	std::ofstream out;	out.open("txt/ErrLog.txt", std::ios::trunc);
	int i = 0;
	if(out.is_open()){	out << st << std::endl;	}
	else if(i = 0){	mkdir("txt");	i++; }
	else if(i = 1){	genGLUTswitch(0);	};
	out.close();	};
		

