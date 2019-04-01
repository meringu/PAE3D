#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <math.h>

void PAE3D_RenderSkyBox(GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
void PAE3D_RenderAxes();
void PAE3D_RenderGrid(float);
