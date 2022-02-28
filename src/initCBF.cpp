#include "CBF.h"


void (*InitCBF::display)() = {	InitCBF::Display	};

void InitCBF::settings()	{
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH),
	glutGet(GLUT_SCREEN_WIDTH));
	glutInitWindowPosition(0,0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE |
		GLUT_DEPTH | GLUT_MULTISAMPLE);
	glDisable(GL_LIGHTING);	};

void InitCBF::Display()	{
	glColor3d(0.1,0.1,0.4);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, GLUT_WINDOW_WIDTH, 0.0, GLUT_WINDOW_HEIGHT, -1, 1);
	glColor3f(1.0f,1.0f,1.0f);
	glRasterPos2i(0,2);
	glutBitmapString(GLUT_BITMAP_HELVETICA_12, (unsigned const char*)"Test");
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glutSwapBuffers();	};
