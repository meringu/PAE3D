#include "Color.h"
#include <GL/glut.h>
#include <stdio.h>

using namespace std;

void (*startColor)(void);
void (*repostMain)(void);
void Display(void);
void Click(int, int, int, int);
void Move(int, int);
void ColKeyboard(unsigned char, int, int);
void RenderColorPalate(void);

PAE3D_Material* mats;
int matCount;
GLint wind;
GLint mainWind;
int cur = 0;
bool visable = false;
bool leftDown = false;


Color::Color(void (*p)(void), void (*r)(void)) {
	startColor = p;
	repostMain = r;
	matCount = 1;
	mats = new PAE3D_Material[matCount];

	PAE3D_Material mat;
	mat.col.r = mat.col.g = 0.77;
	mat.col.b = 0.5;
	mat.col.a = 1;
	mats[0] = mat;
}

Color::~Color() {

}

void Color::Open(unsigned int mainWin) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
//	glutInitWindowSize(259, 500);
	//wind = glutCreateWindow("Color Picker");
	visable = true;
	mainWind = mainWin;
	wind = glutCreateSubWindow(mainWin,0,45,259,430);
	glutDisplayFunc(Display);
	glutMouseFunc(Click);
	glutMotionFunc(Move);
	glutKeyboardFunc(ColKeyboard);
	glClearColor(1, 1, 1, 1);
	glutMainLoop();
}

void Color::Close(){
	visable = false;
	glutDestroyWindow(wind);
}

PAE3D_Material* Color::GetMaterial(int i) {
	if (i < 0 || i > matCount) return &mats[0];
	return &mats[i];
}

int Color::GetCurrentMaterial() {
	return cur;
}

void Click(int button, int state, int x, int y) {
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float percY = -(y / (height / 2.0) - 1);
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			leftDown = true;
			if (percY > 0.8) {
				mats[0].col.r = (x - 1) / 256.0;
			}
			else if (percY > 0.6) {
				mats[0].col.g = (x - 1) / 256.0;
			}
			else if (percY > 0.4) {
				mats[0].col.b = (x - 1) / 256.0;
			}
		}
		else {
			leftDown = false;
		}
	}
	glutPostRedisplay();
	repostMain();
	startColor();
}

void Move(int x, int y) {
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	float percY = -(y / (height / 2.0) - 1.1);
	if (leftDown) {
		if (percY > 0.8) {
			mats[0].col.r = (x - 1) / 256.0;
		}
		else if (percY > 0.6) {
			mats[0].col.g = (x - 1) / 256.0;
		}
		else if (percY > 0.4) {
			mats[0].col.b = (x - 1) / 256.0;
		}
	}
	glutPostRedisplay();
	repostMain();
	startColor();
}

void ColKeyboard(unsigned char key, int x, int y){
	(void)x;
	(void)y;
	switch (key) {
	case 'c':
		visable = false;
		glutSetWindow(mainWind);
		glutDestroyWindow(wind);
		break;
	}
	glutPostRedisplay();
}

bool Color::IsVisable(){
	return visable;
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderColorPalate();
	glutSwapBuffers();
}

void RenderColorPalate(){
	glPushMatrix();
	glTranslatef(0,-0.1,0);
	glBegin(GL_LINES);
	for (int i = 0; i < 256; i++) {
		glColor3f(i/256.0, mats[cur].col.g, mats[cur].col.b);
		glVertex2f((i+1)/129.0-1, 1);
		glVertex2f((i+1)/129.0-1, 0.8);
	}
	glColor3f(1 - mats[cur].col.r, 1 - mats[cur].col.g, 1 - mats[cur].col.b);
	glVertex2f((mats[cur].col.r * 256 + 1) / 129.0 - 1, 1);
	glVertex2f((mats[cur].col.r * 256 + 1) / 129.0 - 1, 0.8);
	for (int i = 0; i < 256; i++) {
		glColor3f(mats[0].col.r, i / 256.0, mats[cur].col.b);
		glVertex2f((i + 1) / 129.0 - 1, 0.8);
		glVertex2f((i + 1) / 129.0 - 1, 0.6);
	}
	glColor3f(1 - mats[cur].col.r, 1 - mats[cur].col.g, 1 - mats[cur].col.b);
	glVertex2f((mats[cur].col.g * 256 + 1) / 129.0 - 1, 0.8);
	glVertex2f((mats[cur].col.g * 256 + 1) / 129.0 - 1, 0.6);
	for (int i = 0; i < 256; i++) {
		glColor3f(mats[cur].col.r, mats[cur].col.b, i / 256.0);
		glVertex2f((i + 1) / 129.0 - 1, 0.6);
		glVertex2f((i + 1) / 129.0 - 1, 0.4);
	}
	glColor3f(1 - mats[cur].col.r, 1 - mats[cur].col.g, 1 - mats[cur].col.b);
	glVertex2f((mats[cur].col.b * 256 + 1) / 129.0 - 1, 0.6);
	glVertex2f((mats[cur].col.b * 256 + 1) / 129.0 - 1, 0.4);
	glEnd();
	glPopMatrix();
}
