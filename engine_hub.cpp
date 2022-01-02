
gameVar::gameVar()
{
	
};


void engine(int argc,char **argv, gameVar vars)
{
	initCallBackFunctions(argc,argv,vars);
	gameCallBackFunctions();
	genCallBackFunctions();
};
void initCallBackFunctions(argc,argv,vars){
		glutInitWindowSize(vars.find(WINDOWWIDTH)->second,vars.find(WINDOWHEIGHT)->second);
		glutInitWindowPosition(vars.find(CENTERWIDTH)->second,vars.find(CENTERHEIGHT)->second);
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
		glutCreateWindow(GAME_NAME);
//		glutDisplayFunc(display);
};
void gameCallBackFunctions(){};
void genCallBackFunctions(){	
		glutInitErrorFunc(errorReturn);
		glutTimerFunc(CLOCKUP, timerUpdate, 0);
		glutTimerFunc(LOGTIME, timerUpdate, 0); 
		
		glutMainLoop();
};
