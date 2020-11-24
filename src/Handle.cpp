#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif
#include "Handle.h"

using namespace std;

void Handle::RenderXHandleMove(GLUquadric* quadric, float width, float length) {
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	gluCylinder(quadric, width, width, length, 20, 1);
	glTranslatef(0, 0, length);
	gluCylinder(quadric, width * 4, 0, width * 8, 20, 1);
	glPopMatrix();
}

void Handle::RenderYHandleMove(GLUquadric* quadric, float width, float length) {
	glPushMatrix();
	glRotatef(90, -1, 0, 0);
	gluCylinder(quadric, width, width, length, 20, 1);
	glTranslatef(0, 0, length);
	gluCylinder(quadric, width * 4, 0, width * 8, 20, 1);
	glPopMatrix();
}

void Handle::RenderZHandleMove(GLUquadric* quadric, float width, float length) {
	glPushMatrix();
	gluCylinder(quadric, width, width, length, 20, 1);
	glTranslatef(0, 0, length);
	gluCylinder(quadric, width * 4, 0, width * 8, 20, 1);
	glPopMatrix();
}

void Handle::RenderXHandleScale(GLUquadric* quadric, float width, float length) {
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	gluCylinder(quadric, width, width, length, 20, 1);
	glTranslatef(0, 0, length);
	RenderCube(width);
	glPopMatrix();
}

void Handle::RenderYHandleScale(GLUquadric* quadric, float width, float length) {
	glPushMatrix();
	glRotatef(90, -1, 0, 0);
	gluCylinder(quadric, width, width, length, 20, 1);
	glTranslatef(0, 0, length);
	RenderCube(width);
	glPopMatrix();
}

void Handle::RenderZHandleScale(GLUquadric* quadric, float width, float length) {
	glPushMatrix();
	gluCylinder(quadric, width, width, length, 20, 1);
	glTranslatef(0, 0, length);
	RenderCube(width);
	glPopMatrix();
}

void Handle::RenderCube(float width) {
	glBegin(GL_QUADS);
	glVertex3f( width*4,  width*4, 0);
	glVertex3f(-width*4,  width*4, 0);
	glVertex3f(-width*4, -width*4, 0);
	glVertex3f( width*4, -width*4, 0);
	glVertex3f( width*4,  width*4, width*8);
	glVertex3f(-width*4,  width*4, width*8);
	glVertex3f(-width*4, -width*4, width*8);
	glVertex3f( width*4, -width*4, width*8);
	glVertex3f( width*4,  width*4, 0);
	glVertex3f(-width*4,  width*4, 0);
	glVertex3f(-width*4,  width*4, width*8);
	glVertex3f( width*4,  width*4, width*8);
	glVertex3f( width*4, -width*4, 0);
	glVertex3f(-width*4, -width*4, 0);
	glVertex3f(-width*4, -width*4, width*8);
	glVertex3f( width*4, -width*4, width*8);
	glVertex3f(-width*4,  width*4, 0);
	glVertex3f(-width*4, -width*4, 0);
	glVertex3f(-width*4, -width*4, width*8);
	glVertex3f(-width*4,  width*4, width*8);
	glVertex3f( width*4,  width*4, 0);
	glVertex3f( width*4, -width*4, 0);
	glVertex3f( width*4, -width*4, width*8);
	glVertex3f( width*4,  width*4, width*8);
	glEnd();
}
