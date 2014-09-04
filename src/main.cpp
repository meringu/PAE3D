#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "define.h"
#include "Model.h"
#include <iostream>

using namespace std;

// Global Variables
GLuint g_mainWnd;
GLuint g_nWinWidth  = 640;
GLuint g_nWinHeight = 480;
Model* g_pGeometry = NULL;

bool middleClickDown = false;
int lastX, lastY = 0;
float rotation = 0, tilt = 0, zoom = 10;

void PAE3D_Display() ;
void PAE3D_Reshape(int w, int h) ;
void PAE3D_SetCamera();
void PAE3D_SetLight();
void PAE3D_MouseClick(int button, int state, int x, int y);
void PAE3D_MouseMove(int x, int y);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(g_nWinWidth, g_nWinHeight);
    g_mainWnd = glutCreateWindow("COMP308 Assignment1");

    g_pGeometry = new Model;
    g_pGeometry->CreateGeometry();

    glutDisplayFunc(PAE3D_Display);
    glutReshapeFunc(PAE3D_Reshape);
    glutMouseFunc(PAE3D_MouseClick);
    glutMotionFunc(PAE3D_MouseMove);

	PAE3D_SetLight();
	PAE3D_SetCamera();
	glutMainLoop();

    return 0;
}

// Display function
void PAE3D_Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);

	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor3f(1.0f,0.0f,0.0f); /* set object color as red */

	g_pGeometry->RenderGeometry();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	glutSwapBuffers();
}

// Reshape function
void PAE3D_Reshape(int w, int h)
{
    if (h == 0) h = 1;

	g_nWinWidth = w;
	g_nWinHeight = h;

    glViewport(0, 0, g_nWinWidth, g_nWinHeight);
}

// Set Light
void PAE3D_SetLight()
{
	float direction[]	  = {0.0f, 0.0f, 1.0f, 0.0f};
	float diffintensity[] = {0.7f, 0.7f, 0.7f, 1.0f};
	float ambient[]       = {0.2f, 0.2f, 0.2f, 1.0f};

	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);


	glEnable(GL_LIGHT0);
}

void PAE3D_MouseClick(int button, int state, int x, int y){
	lastX = x;
	lastY = y;

	switch (button) {
	case GLUT_MIDDLE_BUTTON:
		middleClickDown = state == GLUT_DOWN;
		break;
	}
	glutPostRedisplay();
}

void PAE3D_MouseMove(int x, int y) {
	if (middleClickDown) {
		rotation += x - lastX;
		tilt += y - lastY;
	}
	if (tilt > 85) {
		tilt = 85;
	}
	if (tilt < -85) {
		tilt = -85;
	}
	lastX = x;
	lastY = y;
	PAE3D_SetCamera();
	glutPostRedisplay();
}

// Set Camera Position
void PAE3D_SetCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(PAE3D_FOVY, (double) g_nWinWidth / (double) g_nWinHeight, PAE3D_ZNEAR_3D, PAE3D_ZFAR_3D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float xpos = -zoom*cos(tilt*M_PI/180)*sin(rotation*M_PI/180);
	float ypos = zoom*sin(tilt*M_PI/180);
	float zpos = zoom*cos(tilt*M_PI/180)*cos(rotation*M_PI/180);

	gluLookAt(xpos, ypos, zpos, 0, 0, 0, 0, 1, 0);
}













