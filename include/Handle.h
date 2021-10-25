#pragma once

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
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
