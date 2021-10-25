#pragma once

#include <SDL2/SDL.h>

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

class Toolbar {
public:
	Toolbar(void (*i)(int), int (*pi) (int));
	~Toolbar(void);
	void Display(int, int);
	bool Click(int, int, int, int, int, int);
private:
	void (*buttonCallback_)(int);
	int (*getParamCallback_)(int);
	GLuint toolbarTexture_;
};
