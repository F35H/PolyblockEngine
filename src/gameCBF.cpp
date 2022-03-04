#include "hubCBF.h"

void (*GameCBF::display)() = {	GameCBF::Display	};

void GameCBF::mainMenCBF(){
	using namespace MainMenCBF;
	
	try{
		for (float i_ = 0.5;i <= 1.5; i_ += 0.25) {
			win.push_back(new menBut(i_));
			glutDisplayFunc(display);
			glutReshapeFunc(resize) };
		
		glutReshapeFunc(NULL);
		glutMouseFunc(mouse); }
		catch(std::exception &e) 
		{ std::string st = e.what(); st += "Found in Main Menu Scene";
			errorHan::printLog(st); };	};

void GameCBF::Display() {
	switch( scene ) {
		case sceneCheck::mainMen: { mainMenCallBackFunctions(); 
		break; } }; };	
