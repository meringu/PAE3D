#pragma once

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "define.h"

class Handle
{
private:
	static void RenderCube(float);

public:
	static void RenderXHandleMove(GLUquadric*, float, float);
	static void RenderYHandleMove(GLUquadric*, float, float);
	static void RenderZHandleMove(GLUquadric*, float, float);

	static void RenderXHandleScale(GLUquadric*, float, float);
	static void RenderYHandleScale(GLUquadric*, float, float);
	static void RenderZHandleScale(GLUquadric*, float, float);
};
