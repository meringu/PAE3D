#pragma once

#include "define.h"
#include <GL/glut.h>

class Handle
{
public:
	static void RenderXHandle(GLUquadric*, float, float);
	static void RenderYHandle(GLUquadric*, float, float);
	static void RenderZHandle(GLUquadric*, float, float);
};
