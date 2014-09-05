#pragma once

#include "define.h"
#include <GL/glut.h>

class Lights
{
public:
	static void AddLight(int);
	static void SetUnlit(int);
	static void SetLit(int);
};
