#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include "define.h"
#include "Model.h"
#include <iostream>

using namespace std;

GLuint g_mainWnd;
GLuint g_nWinWidth = PAE3D_WIN_WIDTH;
GLuint g_nWinHeight = PAE3D_WIN_HEIGHT;
Model* g_model = NULL;
#define PAE3D_RENDER 1
#define PAE3D_SELECT_RIGHT 2
#define PAE3D_SELECT_LEFT 3

int mode = PAE3D_RENDER;
bool q_middleClickDown = false;
bool q_leftClickDown = false;
int g_lastX, g_lastY = 0;
float rotation = 45, tilt = 30, zoom = 10;
bool m_ctrlDownNow = false, m_shiftDownNow = false;
bool m_ctrlDownLastMiddleClick = false, m_shiftDownLastMiddleClick = false;
PAE3D_Point g_center;

void PAE3D_Display() ;
void PAE3D_Reshape(int w, int h);
void PAE3D_SetLights();
void PAE3D_SetCamera();
void PAE3D_RenderAxes();
void PAE3D_RenderGrid();
void PAE3D_MouseClick(int button, int state, int x, int y);
void PAE3D_MouseMove(int x, int y);
void PAE3D_KeyboardDown(unsigned char, int, int);
void PAE3D_KeyboardUp(unsigned char, int, int);

int main(int argc, char** argv) {
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
    PAE3D_SetLights();
	PAE3D_SetCamera();

	glutMainLoop();
    return 0;
}

void PAE3D_Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);
	switch (mode) {
	case PAE3D_RENDER:
		glDisable(GL_LIGHTING);
		PAE3D_RenderGrid();
		PAE3D_RenderAxes();
		g_model->RenderVertices(zoom);
		g_model->RenderEdges(zoom);
		glEnable(GL_LIGHTING);
		g_model->RenderFaces();
		break;
	case PAE3D_SELECT_RIGHT:
		glDisable(GL_LIGHTING);
		g_model->RenderPicker(zoom);
		g_model->ProcessSelection(g_lastX, g_lastY, m_shiftDownNow, false);
		glEnable(GL_LIGHTING);
		break;
	case PAE3D_SELECT_LEFT:
		glDisable(GL_LIGHTING);
		g_model->RenderPicker(zoom);
		g_model->ProcessSelection(g_lastX, g_lastY, m_shiftDownNow, true);
		glEnable(GL_LIGHTING);
		break;
	}
	if (mode != PAE3D_RENDER) {
		mode = PAE3D_RENDER;
		glutPostRedisplay();
	} else {
		glClear(GL_DEPTH_BUFFER_BIT);
		g_model->RenderSelectedFacesHandle(zoom);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
		glutSwapBuffers();
	}
}

void PAE3D_Reshape(int w, int h) {
    if (h == 0) h = 1;

	g_nWinWidth = w;
	g_nWinHeight = h;

    glViewport(0, 0, g_nWinWidth, g_nWinHeight);
    PAE3D_SetCamera();
    glutPostRedisplay();
}

void PAE3D_KeyboardDown(unsigned char key, int x, int y) {
	(void)x;
	(void)y;
	int sp = glutGetModifiers();
	m_shiftDownNow = GLUT_ACTIVE_SHIFT & sp;
	m_ctrlDownNow = GLUT_ACTIVE_CTRL & sp;
	switch (key) {
	case ' ':
		g_model->Smooth();
		break;
	case 'f':
		g_model->SetSelectType(PAE3D_SELECT_FACES);
		break;
	case 'v':
		g_model->SetSelectType(PAE3D_SELECT_VERTICES);
		break;
	case 'e':
		g_model->SetSelectType(PAE3D_SELECT_EDGES);
		break;
	}
	glutPostRedisplay();
}

void PAE3D_KeyboardUp(unsigned char key, int x, int y) {
	(void)x;
	(void)y;
	int sp = glutGetModifiers();
	m_shiftDownNow = GLUT_ACTIVE_SHIFT & sp;
	m_ctrlDownNow = GLUT_ACTIVE_CTRL & sp;
	switch(key) {
	}
	glutPostRedisplay();
}

void PAE3D_MouseClick(int button, int state, int x, int y){
	g_lastX = x;
	g_lastY = y;
	int sp = glutGetModifiers();
	m_shiftDownNow = GLUT_ACTIVE_SHIFT & sp;
	m_ctrlDownNow = GLUT_ACTIVE_CTRL & sp;
	switch (button) {
	case GLUT_MIDDLE_BUTTON:
		q_middleClickDown = state == GLUT_DOWN;
		m_shiftDownLastMiddleClick = GLUT_ACTIVE_SHIFT & sp;
		m_ctrlDownLastMiddleClick = GLUT_ACTIVE_CTRL & sp;
		break;
	case GLUT_LEFT_BUTTON:
		q_leftClickDown = state == GLUT_DOWN;
		if (state == GLUT_DOWN) {
			mode = PAE3D_SELECT_LEFT;
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			mode = PAE3D_SELECT_RIGHT;
		}
		break;
	}
	glutPostRedisplay();
}

void PAE3D_MouseMove(int x, int y) {
	int sp = glutGetModifiers();
	m_shiftDownNow = GLUT_ACTIVE_SHIFT & sp;
	m_ctrlDownNow = GLUT_ACTIVE_CTRL & sp;
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float worldDX = ((g_lastX-x)*cos(rotation*M_PI/180)+(y-g_lastY)*sin(tilt*M_PI/180)*sin(rotation*M_PI/180))*zoom/height/4;
	float worldDY = (y-g_lastY)*cos(tilt*M_PI/180)*zoom/height/4;
	float worldDZ = ((g_lastX-x)*sin(rotation*M_PI/180)+(g_lastY-y)*sin(tilt*M_PI/180)*cos(rotation*M_PI/180))*zoom/height/4;
	if (q_middleClickDown) {
		if (m_ctrlDownLastMiddleClick) {
			zoom *= 1 + (y - g_lastY)*0.005;
		} else if (m_shiftDownLastMiddleClick) {
			g_center.x += worldDX;
			g_center.y += worldDY;
			g_center.z += worldDZ;
		} else {
			rotation += x - g_lastX;
			tilt += y - g_lastY;
			if (tilt > 85) {
				tilt = 85;
			}
			if (tilt < -85) {
				tilt = -85;
			}
		}
	}
	if (q_leftClickDown) {
		switch (g_model->SelectedHandle) {
		case PAE3D_SELECT_X_HANDLE:
			g_model->MoveSelected(-worldDX*2, 0, 0);
			break;
		case PAE3D_SELECT_Y_HANDLE:
			g_model->MoveSelected(0, -worldDY*2, 0);
			break;
		case PAE3D_SELECT_Z_HANDLE:
			g_model->MoveSelected(0, 0, -worldDZ*2);
			break;
		}
	}
	g_lastX = x;
	g_lastY = y;
	PAE3D_SetCamera();
	glutPostRedisplay();
}

void PAE3D_SetLights() {
	float direction[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	float diffintensity[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glEnable(GL_LIGHT0);
}

void PAE3D_SetCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(PAE3D_FOVY, (double) g_nWinWidth / (double) g_nWinHeight, PAE3D_ZNEAR_3D, PAE3D_ZFAR_3D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float xpos = -zoom*cos(tilt*M_PI/180)*sin(rotation*M_PI/180) + g_center.x;
	float ypos = zoom*sin(tilt*M_PI/180) + g_center.y;
	float zpos = zoom*cos(tilt*M_PI/180)*cos(rotation*M_PI/180) + g_center.z;

	gluLookAt(xpos, ypos, zpos, g_center.x, g_center.y, g_center.z, 0, 1, 0);
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











