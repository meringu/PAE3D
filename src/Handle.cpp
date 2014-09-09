#include "Handle.h"

using namespace std;

void Handle::RenderXHandle(GLUquadric* quadric, float width, float length) {
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	gluCylinder(quadric, width, width, length, 20, 1);
	glTranslatef(0, 0, length);
	gluCylinder(quadric, width * 4, 0, width * 8, 20, 1);
	glPopMatrix();
}

void Handle::RenderYHandle(GLUquadric* quadric, float width, float length) {
	glPushMatrix();
	glRotatef(90, -1, 0, 0);
	gluCylinder(quadric, width, width, length, 20, 1);
	glTranslatef(0, 0, length);
	gluCylinder(quadric, width * 4, 0, width * 8, 20, 1);
	glPopMatrix();
}

void Handle::RenderZHandle(GLUquadric* quadric, float width, float length) {
	glPushMatrix();
	gluCylinder(quadric, width, width, length, 20, 1);
	glTranslatef(0, 0, length);
	gluCylinder(quadric, width * 4, 0, width * 8, 20, 1);
	glPopMatrix();
}
