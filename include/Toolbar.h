#pragma once
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "define.h"

class Toolbar {
public:
	Toolbar(void (*i)(int));
	~Toolbar(void);
	void Open(unsigned int,int);
	void Close();
	void CallReshape(int);
	void DrawButtons(bool);
	int getCurrentButton();
	void stop();

};
