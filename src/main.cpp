#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>
#include <iostream>
#include "define.h"
#include "Model.h"
#include "ImageLoader.h"
#include "Color.h"
#include "Toolbar.h"
#include "RenderHelper.h"

using namespace std;

GLuint g_mainWnd;
GLuint g_nWinWidth = PAE3D_WIN_WIDTH;
GLuint g_nWinHeight = PAE3D_WIN_HEIGHT;
Model* g_model = NULL;
Toolbar* g_toolbar = NULL;

int displayMode = PAE3D_EDIT_MODE;
int handleMode = PAE3D_HANDLE_MOVE;
int leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
int mode = PAE3D_RENDER;
bool q_middleClickDown = false;
bool q_leftClickDown = false;
int g_lastX = 0, g_lastY = 0;
float rotation = 45, tilt = 25, zoom = 10;
bool m_ctrlDownNow = false, m_shiftDownNow = false;
bool m_ctrlDownLastMiddleClick = false, m_shiftDownLastMiddleClick = false;
PAE3D_Point g_center;
Color* g_color;

GLuint skyBox;
bool subToggle = false;
int prev = -1;

GLuint skyBoxXp;
GLuint skyBoxXn;
GLuint skyBoxYp;
GLuint skyBoxYn;
GLuint skyBoxZp;
GLuint skyBoxZn;

void PAE3D_DisplayMode(int);
void PAE3D_Display();
void PAE3D_Reshape(int w, int h);
void PAE3D_SetLights();
void PAE3D_SetCamera();
void PAE3D_MouseClick(int button, int state, int x, int y);
void PAE3D_MouseMove(int x, int y);
void PAE3D_KeyboardDown(unsigned char, int, int);
void PAE3D_KeyboardUp(unsigned char, int, int);
void PAE3D_RepostMain();
void PAE3D_ButtonPushed(int);
int getParami(int);

int main(int argc, char** argv) {
	const char* usage = "Usage: PAE3D [option]\n\nOption\t\tMeaning\n --help\t\tShow this message\n -o <file>\tLoad .obj file\n\t\tLoad default cube\n";
	char* file = NULL;
	if (argc > 1) {
		if (argc != 3) {
			printf("%s", usage);
			exit(0);
		}
		if (strcmp(argv[1], "-o") == 0) {
			file = argv[2];
		}
		else {
			printf("%s", usage);
			exit(0);
		}
	}
	leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(g_nWinWidth, g_nWinHeight);
    g_mainWnd = glutCreateWindow("PAE3D");
    skyBoxXp = openTexture("textures/cubemapxp.jpg");
    skyBoxXn = openTexture("textures/cubemapxn.jpg");
    skyBoxYp = openTexture("textures/cubemapyp.jpg");
    skyBoxYn = openTexture("textures/cubemapyn.jpg");
    skyBoxZp = openTexture("textures/cubemapzp.jpg");
    skyBoxZn = openTexture("textures/cubemapzn.jpg");
    openCubeMap("textures/cubemapxp.jpg", "textures/cubemapxn.jpg", "textures/cubemapyp.jpg", "textures/cubemapyn.jpg", "textures/cubemapzp.jpg", "textures/cubemapzn.jpg");
    g_model = new Model(file);
    glClearColor(0.5, 0.5, 0.5, 1);
    glutDisplayFunc(PAE3D_Display);
    glutReshapeFunc(PAE3D_Reshape);
    glutMouseFunc(PAE3D_MouseClick);
    glutMotionFunc(PAE3D_MouseMove);
    glutKeyboardFunc(PAE3D_KeyboardDown);
    glutKeyboardUpFunc(PAE3D_KeyboardUp);
    PAE3D_SetLights();
	PAE3D_SetCamera();
	g_color = new Color(PAE3D_RepostMain, g_mainWnd, g_nWinHeight, PAE3D_KeyboardDown, PAE3D_KeyboardUp);
    g_toolbar = new Toolbar(PAE3D_ButtonPushed, g_mainWnd, PAE3D_KeyboardDown, PAE3D_KeyboardUp, getParami);
	glutMainLoop();
    return 0;
}

int getParami(int value) {
	if (value == PAE3D_RIGHT_CLICK) {
		return g_model->m_SelectMode;
	}
	if (value == PAE3D_LEFT_CLICK) {
		return leftCLickOperation;
	}
	if (value == PAE3D_RENDER_MODE) {
		return displayMode;
	}
	if (value == PAE3D_HANDLE_MODE) {
		return handleMode;
	}
	return -1;
}

void PAE3D_ButtonPushed (int button) {
	switch(button) {
		case 1:
			g_model->SetSelectType(PAE3D_SELECT_FACES);
			leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
			break;
		case 2:
			g_model->SetSelectType(PAE3D_SELECT_EDGES);
			leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
			break;
		case 3:
			g_model->SetSelectType(PAE3D_SELECT_VERTICES);
			leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
			break;
		case 4:
			g_model->SelectAll();
			break;
		case 5:
			handleMode = PAE3D_HANDLE_MOVE;
			break;
		case 6:
			handleMode = PAE3D_HANDLE_SCALE;
			break;
		case 7:
			g_model->Extrude();
			break;
		case 8:
			g_model->Merge();
			break;
		case 9:
			g_model->Subdivide(false);
			break;
		case 10:
			g_model->Subdivide(true);
			break;
		case 11:
			switch(leftCLickOperation){
			case PAE3D_LEFTCLICK_NOTHING:
				leftCLickOperation = PAE3D_LEFTCLICK_COLOR;
				break;
			case PAE3D_LEFTCLICK_COLOR:
				leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
				break;
			}
			break;
		case 12:
			displayMode++;
			displayMode%=2;
			break;
	}
}

void PAE3D_DisplayMode(int mode) {
	displayMode = mode;
}

void PAE3D_RepostMain() {
	glutSetWindow(g_mainWnd);
	glutPostRedisplay();
}

void PAE3D_Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);

	switch (mode) {
	case PAE3D_RENDER:
		switch (displayMode) {
		case PAE3D_EDIT_MODE:
			glDisable(GL_LIGHTING);
			PAE3D_RenderGrid(zoom);
			PAE3D_RenderAxes();
			g_model->RenderVertices(zoom);
			g_model->RenderEdges(zoom);
			glEnable(GL_LIGHTING);
			g_model->RenderFaces(g_color, false);
			break;
		case PAE3D_PHONG_MODE:
			float oldZoom = zoom;
			zoom = 1;
			PAE3D_Point oldCenter = g_center;
			g_center.x = 0;
			g_center.y = 0;
			g_center.z = 0;
			PAE3D_SetCamera();
			PAE3D_RenderSkyBox(skyBoxXp, skyBoxXn, skyBoxYp, skyBoxYn, skyBoxZp, skyBoxZn);
			zoom = oldZoom;
			g_center = oldCenter;
			PAE3D_SetCamera();
			g_model->RenderFaces(g_color, true);
			break;
		}
		break;
	case PAE3D_SELECT_RIGHT:
		glDisable(GL_LIGHTING);
		g_model->RenderPicker(zoom, handleMode);
		g_model->ProcessSelection(g_lastX, g_lastY, m_shiftDownNow, false, -1);
		glEnable(GL_LIGHTING);
		break;
	case PAE3D_SELECT_LEFT:
		glDisable(GL_LIGHTING);
		g_model->RenderPicker(zoom, handleMode);
		int mat = -1;
		if (leftCLickOperation == PAE3D_LEFTCLICK_COLOR) {
			mat = g_color->GetCurrentMaterial();
		}
		g_model->ProcessSelection(g_lastX, g_lastY, m_shiftDownNow, true, mat);
		glEnable(GL_LIGHTING);
		break;
	}
	if (mode != PAE3D_RENDER) {
		mode = PAE3D_RENDER;
		glutPostRedisplay();
	} else {
		glClear(GL_DEPTH_BUFFER_BIT);
		g_model->RenderSelectedFacesHandle(zoom, handleMode);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
		glutSwapBuffers();
	}
	g_toolbar->repaint();
}

void PAE3D_Reshape(int w, int h) {
    if (h == 0) h = 1;

	if(w>= PAE3D_WIN_WIDTH){
		g_nWinWidth = w;
	}else{	g_nWinWidth = PAE3D_WIN_WIDTH;}

	if(h>= PAE3D_WIN_HEIGHT){
		g_nWinHeight = h;
	}else{	g_nWinHeight = PAE3D_WIN_HEIGHT;}

	/*if(!subToggle){
		g_color->Close();
	}*/
	glutReshapeWindow(g_nWinWidth,g_nWinHeight);
	glViewport(0, 0, g_nWinWidth, g_nWinHeight);
    PAE3D_SetCamera();
    g_color->Resize(w, h);
    glutPostRedisplay();
}

void PAE3D_KeyboardDown(unsigned char key, int x, int y) {
	(void)x;
	(void)y;
	int sp = glutGetModifiers();
	m_shiftDownNow = GLUT_ACTIVE_SHIFT & sp;
	m_ctrlDownNow = GLUT_ACTIVE_CTRL & sp;
	switch (key) {
	case 'g':
		g_model->Subdivide(true);
		break;
	case 3:
		g_model->Duplicate();
		break;
	case 127:
		g_model->DeleteSelection();
		break;
	case 'f':
		g_model->SetSelectType(PAE3D_SELECT_FACES);
		leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
		break;
	case 'v':
		g_model->SetSelectType(PAE3D_SELECT_VERTICES);
		leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
		break;
	case 'e':
		g_model->SetSelectType(PAE3D_SELECT_EDGES);
		leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
		break;
	case 'm':
		handleMode = PAE3D_HANDLE_MOVE;
		break;
	case 's':
		handleMode = PAE3D_HANDLE_SCALE;
		break;
	case 'x':
		g_model->Extrude();
		break;
	case 'l':
		g_model->Merge();
		break;
	case 'p':
		displayMode++;
		displayMode%=2;
		break;
	case 'd':
		g_model->Subdivide(false);
		break;
	case 'c':
		switch(leftCLickOperation){
		case PAE3D_LEFTCLICK_NOTHING:
			leftCLickOperation = PAE3D_LEFTCLICK_COLOR;
			break;
		case PAE3D_LEFTCLICK_COLOR:
			leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
			break;
		}
		break;
	case 'a':
		g_model->SelectAll();
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
		switch(handleMode) {
		case PAE3D_HANDLE_MOVE:
			switch (g_model->SelectedHandle) {
			case PAE3D_SELECT_X_HANDLE:
				g_model->MoveSelected(-worldDX * 2, 0, 0);
				break;
			case PAE3D_SELECT_Y_HANDLE:
				g_model->MoveSelected(0, -worldDY * 2, 0);
				break;
			case PAE3D_SELECT_Z_HANDLE:
				g_model->MoveSelected(0, 0, -worldDZ * 2);
				break;
			}
			break;
		case PAE3D_HANDLE_SCALE:
			switch (g_model->SelectedHandle) {
			case PAE3D_SELECT_X_HANDLE:
				g_model->ScaleSelected(-worldDX * 2 / zoom * 10, 0, 0);
				break;
			case PAE3D_SELECT_Y_HANDLE:
				g_model->ScaleSelected(0, -worldDY * 2 / zoom * 10, 0);
				break;
			case PAE3D_SELECT_Z_HANDLE:
				g_model->ScaleSelected(0, 0, -worldDZ * 2 / zoom * 10);
				break;
			}
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
