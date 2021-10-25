#pragma once

#include <SDL2/SDL.h>

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "define.h"

class Color {
public:
	Color(void (*d) (SDL_Keycode), void (*u) (SDL_Keycode));
	~Color(void);
	PAE3D_Material* GetMaterial(int);
	int GetCurrentMaterial();
	void Display(int, int);
private:
	GLuint redTexture_;
	GLuint greenTexture_;
	GLuint blueTexture_;
	GLuint specularTexture_;
	GLuint shininessTexture_;
	GLuint addTexture_;
	GLuint leftArrowTexture_;
	GLuint rightArrowTexture_;
};
