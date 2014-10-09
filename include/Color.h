#pragma once
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "define.h"

class Color {
public:
	Color(void (*p)(void), void (*r)(void));
	~Color(void);
	void Open(unsigned int);
	void Close();
	PAE3D_Material* GetMaterial(int);
	int GetCurrentMaterial();
	bool IsVisable();
};
