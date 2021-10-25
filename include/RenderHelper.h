#pragma once

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

void PAE3D_RenderSkyBox(GLuint, GLuint, GLuint, GLuint, GLuint, GLuint);
void PAE3D_RenderAxes();
void PAE3D_RenderGrid(float);
