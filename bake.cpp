#include "bake.h"

//Variables
//External Function Pointers
bool (*bakeConfigure)(std::map<const char*,SI> &genBakeVar, PSI &psi) = [](std::map<const char*,SI> &genBakeVar, PSI &psi){
	for(psi = 0;;psi++)
	{
		switch(psi)
		{
			default:
			genBakeVar.insert(std::pair<const char*,SI>(WINDOWWIDTH,si));
			break;
			case 0:
			genBakeVar.insert(std::pair<const char*,SI>(WINDOWHEIGHT,si));
			break;
			case 1:
			genBakeVar.insert(std::pair<const char*,SI>(CENTERWIDTH,si));
			break;
			case 2:
			genBakeVar.insert(std::pair<const char*,SI>(CENTERHEIGHT,si));
			break;
			case 3:
			goto escape;
			break;
		}
	}
		escape:
		psi = 0;
		return true;
};
 void (*getScreenSize)(PSI &psi,SI &si) = [](PSI &psi,SI &si){
	switch(psi)
	{
		case 0:
		si = glutGet(GLUT_SCREEN_WIDTH);
		return;
		case 1:
		si = glutGet(GLUT_SCREEN_HEIGHT);
		return;
		case 3:
		si = glutGet(GLUT_SCREEN_WIDTH)/2;
		return;
		case 4:
		si = glutGet(GLUT_SCREEN_HEIGHT)/2;
		return;
	}
};
//Functions
