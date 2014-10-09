#pragma once
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "define.h"

class Toolbar {
public:
	Toolbar(void (*i)(int), unsigned int, void (*d) (unsigned char, int,int), void (*u) (unsigned char, int,int));
	~Toolbar(void);
	void DrawButtons(bool);
	int getCurrentButton();
	void stop();
};
