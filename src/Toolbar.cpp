#include "Toolbar.h"
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

using namespace std;

void BarDisplay(void);
void BarClick(int, int, int, int);
void BarMove(int, int);
void BarReshape(int,int);
void BarSetCamera(void);
void DrawButtons(bool);
void ProcessButton(int x, int y);
void (*startId)(int);

GLint barWind;
GLint barToMainWind;
int barMode = 0;
bool barLeftDown = false;
int mouseX =0;
int mouseY =0;
int currentButton = 0;

Toolbar::Toolbar(void (*i)(int)) {
	startId = i;
	currentButton = 0;
}

Toolbar::~Toolbar() {

}

void Toolbar::Open(unsigned int mainWin, int width) {
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	barToMainWind = mainWin;
	barWind = glutCreateSubWindow(mainWin,0,0,640,35);
	glClearColor(0,0,0,0);
	glutDisplayFunc(BarDisplay);
	BarSetCamera();
	glutReshapeFunc(BarReshape);
	glutMouseFunc(BarClick);
	glutMotionFunc(BarMove);
	glClearColor(1, 1, 1, 1);
	glutMainLoop();
}

void Toolbar::Close(){
	glutDestroyWindow(barWind);
}


void BarClick(int button, int state, int x, int y) {
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			barLeftDown = true;
			barMode = 1;
		}
		else {
			barLeftDown = false;
			barMode = 0;
		}
	}
	mouseX = x;
	mouseY = y;
	glutPostRedisplay();
}

void BarMove(int x, int y) {
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	if (barLeftDown) {

	}
	glutPostRedisplay();
}
void Toolbar::CallReshape(int w){
	//glutSetWindow(barWind);
	//BarReshape(w,50);
	//glutSetWindow(barToMainWind);
}
int Toolbar::getCurrentButton(){
	int btn = currentButton;
 return btn;
}
void Toolbar::stop(){
	 currentButton = 0;
	 startId(0);
}

void BarReshape(int w, int h){
	glutReshapeWindow(w,35);
    glViewport(0, 0, w, 35);

    glutPostRedisplay();
}

void BarDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(barMode == 0){
	DrawButtons(false);}
	else if(barMode == 1){
		DrawButtons(true);
		ProcessButton(mouseX,mouseY);
	}
	if (barMode != 0) {
		barMode = 0;
		currentButton = 0;
		glutPostRedisplay();
	} else {
		glutSwapBuffers();
	}
}

void ProcessButton(int x, int y){
	GLint viewport[4];
	GLubyte pixel[3];
	glGetIntegerv(GL_VIEWPORT,viewport);
	glReadPixels(x,viewport[3]-y,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);
	int id = (pixel[0] << 0) + (pixel[1] << 8) + (pixel[2] << 16);

	currentButton = id;

	startId(id);
}



void DrawButtons(bool picking){
	float indent = -3.0;
	for(int i=0; i<12; i++){
	glPushMatrix();

	glTranslatef(indent+(0.35*i),0,0);
	glBegin(GL_QUADS);
	if(!picking){
	/* put textures here */
	glColor3f(1, 0, 0);
	}else{
		int id = i+1;
		int r = (id & 0x000000FF) >> 0;
		int g = (id & 0x0000FF00) >> 8;
		int b = (id & 0x00FF0000) >> 16;
		glColor3f(r/255.0, g/255.0, b/255.0);
	}
	glTexCoord2f(0, 0);
	glVertex3f(0.15, 0.15, 0);
	glTexCoord2f(1, 0);
	glVertex3f(-0.15, 0.15, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-0.15, -0.15, 0);
	glTexCoord2f(0, 15);
	glVertex3f(0.15, -0.15, 0);

	glEnd();
	glPopMatrix();
	}
}


void BarSetCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(PAE3D_FOVY, (double) 640 / (double) (35), PAE3D_ZNEAR_3D, PAE3D_ZFAR_3D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	PAE3D_Point newCenter;
	newCenter.x =0;newCenter.y =0;newCenter.z =0;
	gluLookAt(0.0, 0.0, 1.0, newCenter.x, newCenter.y, newCenter.z, 0, 1, 0);
}
