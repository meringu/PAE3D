#include "Toolbar.h"

#include <SDL2/SDL.h>

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <stdio.h>
#include <math.h>

#include "define.h"
#include "ImageLoader.h"

using namespace std;

#define BUTTON_COUNT 12 // 12 buttons in the toolbar
#define BUTTON_PIXELS 32 // height in pixels of the toolbar
#define TOOLBAR_TEXTURE_FILE "textures/toolbar.jpg"

Toolbar::Toolbar(void (*buttonCallback)(int), int (*getParamCallback)(int)) {
	buttonCallback_ = buttonCallback;	
	toolbarTexture_ = openTexture(TOOLBAR_TEXTURE_FILE);
	getParamCallback_ = getParamCallback;
}

Toolbar::~Toolbar() {
	glDeleteTextures(1, &toolbarTexture_);
}

void Toolbar::Display(int x, int y) {
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_COLOR);
	glLoadIdentity();

	float buttonSize = BUTTON_PIXELS * 2.0f; // Multiply by 2 as our view port is from -1 to +1
	float bottomRightx = BUTTON_COUNT * buttonSize / x;
	float bottomRighty = 1 - buttonSize / y;

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, toolbarTexture_);
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2f(0, 0);
	glVertex3f(-1, 1, 0);
	glTexCoord2f(1, 0);
	glVertex3f(-1 + bottomRightx, 1, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-1, bottomRighty, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-1 + bottomRightx, bottomRighty, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	bool highlighted[12] = {
			getParamCallback_(PAE3D_RIGHT_CLICK) == PAE3D_SELECT_FACES,
			getParamCallback_(PAE3D_RIGHT_CLICK) == PAE3D_SELECT_EDGES,
			getParamCallback_(PAE3D_RIGHT_CLICK) == PAE3D_SELECT_VERTICES,
			false,
			getParamCallback_(PAE3D_HANDLE_MODE) == PAE3D_HANDLE_MOVE,
			getParamCallback_(PAE3D_HANDLE_MODE) == PAE3D_HANDLE_SCALE,
			false, false, false, false,
			getParamCallback_(PAE3D_LEFT_CLICK) == PAE3D_LEFTCLICK_COLOR,
			getParamCallback_(PAE3D_RENDER_MODE) == PAE3D_PHONG_MODE};

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0, 1.0, 1.0, 0.3);

	for (int i = 0; i < 12; i++) {
		if (highlighted[i]) {
			glBegin(GL_TRIANGLE_STRIP);
			glVertex3f(-1 + i*buttonSize/x    , 1-buttonSize/y, 0);
			glVertex3f(-1 + (i+1)*buttonSize/x, 1-buttonSize/y, 0);
			glVertex3f(-1 + i*buttonSize/x    , 1, 0);
			glVertex3f(-1 + (i+1)*buttonSize/x, 1, 0);
			glEnd();
		}
	}

	glDisable(GL_BLEND);
}

bool Toolbar::Click(int button, int state, int x, int y, int width, int height) {
	if (button != SDL_BUTTON_LEFT ||
		state != SDL_PRESSED ||
		y > BUTTON_PIXELS ||
		x / BUTTON_PIXELS >= BUTTON_COUNT)
		return false; // Only process left click down in the toolbar

	buttonCallback_(x / BUTTON_PIXELS + 1);
	return true;
}
