#pragma once
#include "define.h"

class Color {
public:
	Color(void (*p)(void), void (*r)(void));
	~Color(void);
	void Open();
	PAE3D_Material GetMaterial(int);
};
