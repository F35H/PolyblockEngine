#include "GL/freeglut.h"
#include <iostream>

void idle() { glutPostRedisplay(); };

GLfloat angle = 0;
GLfloat x = 0;
GLfloat y = 1;
GLfloat z = 0;

GLfloat sx = 0.1;
GLfloat sy = 0.1;
GLfloat sz = 0.1;

GLdouble left = -1;
GLdouble right = 1;
GLdouble  botton  = -1;
GLdouble top = 1;
GLdouble zNear = 1;
GLdouble zFar = 1;

short stage;

void zPsvRender(){
  glNormal3f(0, 0, 1);
  glColor3f(1, 0, 0);
  glVertex3f(-1, -1, 1);
  glVertex3f(1, -1, 1);
  glVertex3f(1, 1, 1);
  glVertex3f(-1, 1, 1); };
  
void xPsvRender(){
  glNormal3f(1, 0, 0);
  glColor3f(0, 1, 0);
  glVertex3f(1, -1, 1);
  glVertex3f(1, -1, -1);
  glVertex3f(1, 1, -1);
  glVertex3f(1, 1, 1); };
  
void yPsvRender(){
  glNormal3f(0, 1, 0);
  glColor3f(0, 1, 1);
  glVertex3f(-1, 1, 1);
  glVertex3f(1, 1, 1);
  glVertex3f(1, 1, -1);
  glVertex3f(-1, 1, -1); };

void zNegRender(){
  glNormal3f(0, 0, -1);
  glColor3f(0, 0, 1);
  glVertex3f(1, -1, -1);
  glVertex3f(-1, -1, -1);
  glVertex3f(-1, 1, -1);
  glVertex3f(1, 1, -1); };

void xNegRender(){
  glNormal3f(-1, 0, 0);
  glColor3f(1, 1, 0);
  glVertex3f(-1, -1, -1);
  glVertex3f(-1, -1, 1);
  glVertex3f(-1, 1, 1);
  glVertex3f(-1, 1, -1); };
    
void yNegRender(){
  glNormal3f(0, -1, 0);
  glColor3f(1, 0, 1);
  glVertex3f(-1, -1, -1);
  glVertex3f(1, -1, -1);
  glVertex3f(1, -1, 1);
  glVertex3f(-1, -1, 1); };
  
void tempIso(){  
  glViewPort(left, right, top, botton, zNear, zFar);
  
  glBegin(GL_QUADS);

  switch(stage){
    default:{
      xNegRender();
      zPsvRender(); }
    break;
    case 1:{
      xNegRender();
      zNegRender(); }
    break;
    case 2:{
      xPsvRender();
      zNegRender(); }
    break;
    case 3:{
      xPsvRender();
      zPsvRender(); }
    break; };
      
  glEnd();  
  
  angle += 1;
  
  if (angle >= 359) { angle = 0; stage = 0; }
  if (angle >= 90) {  stage = 1; }
  if (angle >= 180) { stage = 2; }
  if (angle >= 270) {  stage = 3; }
};


void  display(){
  glClear(GL_COLOR_BUFFER_BIT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  glutSwapBuffers();
};

int main(int argc, char** argv){
 
  glutInit(&argc, argv);
  
  //Settings
  glutInitWindowSize(
    glutGet(GLUT_SCREEN_WIDTH),
    glutGet(GLUT_SCREEN_WIDTH));
  glutInitWindowPosition(0,0);
  glutInitDisplayMode(
    GLUT_RGB | GLUT_DOUBLE |
    GLUT_DEPTH | GLUT_MULTISAMPLE);
  
  //Vars
  short window = glutCreateWindow("Engine Test");
  
  //Display	
  
  glutDisplayFunc(display);
  glutIdleFunc(display);

  
  glutMainLoop();
  
  return 1;  }; 