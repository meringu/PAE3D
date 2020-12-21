#pragma once

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

void PAE3D_RenderSkyBox(GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
void PAE3D_RenderAxes();
void PAE3D_RenderGrid(float);
