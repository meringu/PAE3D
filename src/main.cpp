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
Model* g_model = NULL;

bool middleClickDown = false;
int lastX, lastY = 0;
float rotation = 45, tilt = 30, zoom = 10;
bool ctrlKey = false, shiftKey;
PAE3D_Point center;

void PAE3D_Display() ;
void PAE3D_Reshape(int w, int h) ;
void PAE3D_SetCamera();
void PAE3D_SetLight();
void PAE3D_SetUnlit();
void PAE3D_SetLit();
void PAE3D_RenderAxes();
void PAE3D_RenderGrid();
void PAE3D_MouseClick(int button, int state, int x, int y);
void PAE3D_MouseMove(int x, int y);
void PAE3D_KeyboardDown(unsigned char, int, int);
void PAE3D_KeyboardUp(unsigned char, int, int);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(g_nWinWidth, g_nWinHeight);
    g_mainWnd = glutCreateWindow("PAE3D");

    g_model = new Model();

    glClearColor(0.5, 0.5, 0.5, 1);
    glutDisplayFunc(PAE3D_Display);
    glutReshapeFunc(PAE3D_Reshape);
    glutMouseFunc(PAE3D_MouseClick);
    glutMotionFunc(PAE3D_MouseMove);
    glutKeyboardFunc(PAE3D_KeyboardDown);
    glutKeyboardUpFunc(PAE3D_KeyboardUp);

	PAE3D_SetLight();
	PAE3D_SetCamera();
	glutMainLoop();

    return 0;
}

void PAE3D_Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);

	PAE3D_SetUnlit();

	PAE3D_RenderGrid();
    PAE3D_RenderAxes();

    g_model->RenderVertices(zoom);
    g_model->RenderEdges(zoom);

    PAE3D_SetLit();

	g_model->RenderFaces();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);

	glutSwapBuffers();
}

void PAE3D_Reshape(int w, int h) {
    if (h == 0) h = 1;

	g_nWinWidth = w;
	g_nWinHeight = h;

    glViewport(0, 0, g_nWinWidth, g_nWinHeight);
    PAE3D_SetCamera();
    glutPostRedisplay();
}

void PAE3D_SetLight() {
	float direction[]	  = {0.0f, 0.0f, 1.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	PAE3D_SetLit();
	glEnable(GL_LIGHT0);
}

void PAE3D_KeyboardDown(unsigned char key, int x, int y) {
	(void)x;
	(void)y;
	switch (key) {
	}
}

void PAE3D_KeyboardUp(unsigned char key, int x, int y) {
	(void)x;
	(void)y;
	switch(key) {
	}
}

void PAE3D_MouseClick(int button, int state, int x, int y){
	lastX = x;
	lastY = y;

	switch (button) {
	case GLUT_MIDDLE_BUTTON:
		middleClickDown = state == GLUT_DOWN;
		int sp = glutGetModifiers();
		shiftKey = GLUT_ACTIVE_SHIFT & sp;
		ctrlKey = GLUT_ACTIVE_CTRL & sp;
		break;
	}
	glutPostRedisplay();
}

void PAE3D_MouseMove(int x, int y) {
	if (middleClickDown) {
		if (ctrlKey) {
			zoom *= 1 + (y - lastY)*0.005;
		} else if (shiftKey) {
			int height = glutGet(GLUT_WINDOW_HEIGHT);
			center.x += ((lastX-x)*cos(rotation*M_PI/180)+(y-lastY)*sin(tilt*M_PI/180)*sin(rotation*M_PI/180))*zoom/height/4;
			center.y += (y-lastY)*cos(tilt*M_PI/180)*zoom/height/4;
			center.z += ((lastX-x)*sin(rotation*M_PI/180)+(lastY-y)*sin(tilt*M_PI/180)*cos(rotation*M_PI/180))*zoom/height/4;
		} else {
			rotation += x - lastX;
			tilt += y - lastY;
			if (tilt > 85) {
				tilt = 85;
			}
			if (tilt < -85) {
				tilt = -85;
			}
		}
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

	float xpos = -zoom*cos(tilt*M_PI/180)*sin(rotation*M_PI/180) + center.x;
	float ypos = zoom*sin(tilt*M_PI/180) + center.y;
	float zpos = zoom*cos(tilt*M_PI/180)*cos(rotation*M_PI/180) + center.z;

	gluLookAt(xpos, ypos, zpos, center.x, center.y, center.z, 0, 1, 0);
}

void PAE3D_RenderAxes() {
	// X-AXIS
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1000, 0, 0);
	// Y-AXIS
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1000, 0);
	// Z-AXIS
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1000);
	glEnd();
}

void PAE3D_RenderGrid() {
	float level = pow(10, (int) log10(zoom) - 1);
	glBegin(GL_LINES);
	glColor3f(0.3, 0.3, 0.3);
	for (int x = -10; x <= 10; x++) {
		if (x == 0) {
			glVertex3f(x * level, 0, 0);
			glVertex3f(x * level, 0, -10 * level);
		} else {
			glVertex3f(x * level, 0, 10 * level);
			glVertex3f(x * level, 0, -10 * level);
		}
	}
	for (int z = -10; z <= 10; z++) {
		if (z == 0) {
			glVertex3f(0, 0, z * level);
			glVertex3f(-10 * level, 0, z * level);
		} else {
			glVertex3f(10 * level, 0, z * level);
			glVertex3f(-10 * level, 0, z * level);
		}
	}
}

void PAE3D_SetUnlit() {
	float diffintensity[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float ambient[]       = {1.0f, 1.0f, 1.0f, 1.0f};

	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
}

void PAE3D_SetLit() {
	float diffintensity[] = {0.7f, 0.7f, 0.7f, 1.0f};
	float ambient[]       = {0.2f, 0.2f, 0.2f, 1.0f};

	glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient);
}











