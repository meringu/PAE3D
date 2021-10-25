#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h>

#if defined(__APPLE__)
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <assert.h>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "define.h"
#include "Color.h"
#include "ImageLoader.h"
#include "Model.h"
#include "RenderHelper.h"
#include "Toolbar.h"

using namespace std;

GLuint g_mainWnd;
int g_nWinWidth = 1280;
int g_nWinHeight = 720;
int g_dpiScaling = 1;
Model* g_model = NULL;
Toolbar* g_toolbar = NULL;

int displayMode = PAE3D_EDIT_MODE;
int handleMode = PAE3D_HANDLE_MOVE;
int leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
int mode = PAE3D_RENDER;
bool q_middleClickDown = false;
bool q_leftClickDown = false;
int g_lastX = 0, g_lastY = 0;
float rotation = 45, tilt = 25, zoom = 10;
bool m_ctrlDownNow = false, m_shiftDownNow = false;
bool m_ctrlDownLastMiddleClick = false, m_shiftDownLastMiddleClick = false;
PAE3D_Point g_center;
Color* g_color;

GLuint skyBox;
bool subToggle = false;
int prev = -1;

GLuint skyBoxXp;
GLuint skyBoxXn;
GLuint skyBoxYp;
GLuint skyBoxYn;
GLuint skyBoxZp;
GLuint skyBoxZn;

void PAE3D_DisplayMode(int);
void PAE3D_Display();
void PAE3D_Reshape(int w, int h);
void PAE3D_SetLights();
void PAE3D_SetCamera();
void PAE3D_MouseClick(int button, int state, int x, int y);
void PAE3D_MouseMove(int x, int y);
void PAE3D_KeyboardDown(SDL_Keycode);
void PAE3D_KeyboardUp(SDL_Keycode);
void PAE3D_RepostMain();
void PAE3D_ButtonPushed(int);
int getParami(int);

int main(int argc, char** argv) {
	const char* usage = "Usage: PAE3D [option]\n\nOption\t\tMeaning\n --help\t\tShow this message\n -o <file>\tLoad .obj file\n\t\tLoad default cube\n";
	char* file = NULL;
	if (argc > 1) {
		if (argc != 3) {
			printf("%s", usage);
			exit(0);
		}
		if (strcmp(argv[1], "-o") == 0) {
			file = argv[2];
		}
		else {
			printf("%s", usage);
			exit(0);
		}
	}

	uint32_t windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
	SDL_Window* Window = SDL_CreateWindow("PAE3D", 0, 0, g_nWinWidth, g_nWinHeight, windowFlags);
	if (Window == NULL) {
		printf("Failed to create winwow: %s\n", SDL_GetError());
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	SDL_GLContext Context = SDL_GL_CreateContext(Window);
	SDL_GL_GetDrawableSize(Window, &g_nWinWidth, &g_nWinHeight);
	int wx;
	SDL_GetWindowSize(Window, &wx, NULL);
	g_dpiScaling = g_nWinWidth / wx;
    
	skyBoxXp = openTexture("textures/cubemapxp.jpg");
	skyBoxXn = openTexture("textures/cubemapxn.jpg");
	skyBoxYp = openTexture("textures/cubemapyp.jpg");
	skyBoxYn = openTexture("textures/cubemapyn.jpg");
	skyBoxZp = openTexture("textures/cubemapzp.jpg");
	skyBoxZn = openTexture("textures/cubemapzn.jpg");
    g_model = new Model(file);

	g_color = new Color(PAE3D_KeyboardDown, PAE3D_KeyboardUp);
	g_toolbar = new Toolbar(PAE3D_ButtonPushed, getParami);

	int running = 1;
	int fullScreen = 0;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_RESIZED)
						SDL_GL_GetDrawableSize(Window, &g_nWinWidth, &g_nWinHeight);
						SDL_GetWindowSize(Window, &wx, NULL);
						g_dpiScaling = g_nWinWidth / wx;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = 0;
							break;
						case SDLK_k:
							fullScreen = !fullScreen;
							if (fullScreen) {
								SDL_SetWindowFullscreen(Window, windowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP);
							}
							else {
								SDL_SetWindowFullscreen(Window, windowFlags);
							}
							break;
						default:
							PAE3D_KeyboardDown(event.key.keysym.sym);
							break;
					}
					break;
				case SDL_KEYUP:
					PAE3D_KeyboardUp(event.key.keysym.sym);
					break;
				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP:
					PAE3D_MouseClick(event.button.button, event.button.state, event.button.x, event.button.y);
					break;
				case SDL_MOUSEMOTION:
					PAE3D_MouseMove(event.motion.x, event.motion.y);
					break;
				case SDL_QUIT:
					running = 0;
					break;
				default:
					break;
			}
		}


		glViewport(0, 0, g_nWinWidth, g_nWinHeight);
		glClearColor(0.5, 0.5, 0.5, 1);

		PAE3D_SetLights();
		PAE3D_SetCamera();

		PAE3D_Display();

		SDL_GL_SwapWindow(Window);
	}
}

int getParami(int value) {
	if (value == PAE3D_RIGHT_CLICK) {
		return g_model->m_SelectMode;
	}
	if (value == PAE3D_LEFT_CLICK) {
		return leftCLickOperation;
	}
	if (value == PAE3D_RENDER_MODE) {
		return displayMode;
	}
	if (value == PAE3D_HANDLE_MODE) {
		return handleMode;
	}
	return -1;
}

void PAE3D_ButtonPushed (int button) {
	switch(button) {
		case 1:
			g_model->SetSelectType(PAE3D_SELECT_FACES);
			leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
			break;
		case 2:
			g_model->SetSelectType(PAE3D_SELECT_EDGES);
			leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
			break;
		case 3:
			g_model->SetSelectType(PAE3D_SELECT_VERTICES);
			leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
			break;
		case 4:
			g_model->SelectAll();
			break;
		case 5:
			handleMode = PAE3D_HANDLE_MOVE;
			break;
		case 6:
			handleMode = PAE3D_HANDLE_SCALE;
			break;
		case 7:
			g_model->Extrude();
			break;
		case 8:
			g_model->Merge();
			break;
		case 9:
			g_model->Subdivide(false);
			break;
		case 10:
			g_model->Subdivide(true);
			break;
		case 11:
			switch(leftCLickOperation){
			case PAE3D_LEFTCLICK_NOTHING:
				leftCLickOperation = PAE3D_LEFTCLICK_COLOR;
				break;
			case PAE3D_LEFTCLICK_COLOR:
				leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
				break;
			}
			break;
		case 12:
			displayMode++;
			displayMode%=2;
			break;
	}
}

void PAE3D_DisplayMode(int mode) {
	displayMode = mode;
}

void PAE3D_RepostMain() {
	// glutSetWindow(g_mainWnd);
	// glutPostRedisplay();
}

void PAE3D_Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glShadeModel(GL_SMOOTH);

	switch (mode) {
	case PAE3D_RENDER:
		switch (displayMode) {
		case PAE3D_EDIT_MODE:
			glDisable(GL_LIGHTING);
			PAE3D_RenderGrid(zoom);
			PAE3D_RenderAxes();
			g_model->RenderVertices(zoom, g_nWinWidth, g_nWinHeight);
			g_model->RenderEdges(zoom, g_nWinWidth, g_nWinHeight);
			glEnable(GL_LIGHTING);
			g_model->RenderFaces(g_color, false);
			break;
		case PAE3D_PHONG_MODE:
			float oldZoom = zoom;
			zoom = 1;
			PAE3D_Point oldCenter = g_center;
			g_center.x = 0;
			g_center.y = 0;
			g_center.z = 0;
			PAE3D_SetCamera();
			PAE3D_RenderSkyBox(skyBoxXp, skyBoxXn, skyBoxYp, skyBoxYn, skyBoxZp, skyBoxZn);
			zoom = oldZoom;
			g_center = oldCenter;
			PAE3D_SetCamera();
			g_model->RenderFaces(g_color, true);
			break;
		}
		break;
	case PAE3D_SELECT_RIGHT:
		glDisable(GL_LIGHTING);
		g_model->RenderPicker(zoom, g_nWinWidth, g_nWinHeight, handleMode);
		g_model->ProcessSelection(g_lastX, g_lastY, m_shiftDownNow, false, -1);
		glEnable(GL_LIGHTING);
		break;
	case PAE3D_SELECT_LEFT:
		glDisable(GL_LIGHTING);
		g_model->RenderPicker(zoom, g_nWinWidth, g_nWinHeight, handleMode);
		int mat = -1;
		if (leftCLickOperation == PAE3D_LEFTCLICK_COLOR) {
			mat = g_color->GetCurrentMaterial();
		}
		g_model->ProcessSelection(g_lastX, g_lastY, m_shiftDownNow, true, mat);
		glEnable(GL_LIGHTING);
		break;
	}
	if (mode != PAE3D_RENDER) {
		mode = PAE3D_RENDER;
		// glutPostRedisplay();
	} else {
		glClear(GL_DEPTH_BUFFER_BIT);
		g_model->RenderSelectedFacesHandle(zoom, handleMode, g_nWinWidth, g_nWinHeight);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_LIGHTING);
		glDisable(GL_COLOR_MATERIAL);
		// glutSwapBuffers();
	}
	// g_toolbar->Display(g_nWinWidth / g_dpiScaling, g_nWinHeight / g_dpiScaling);
	// g_color->Display(g_nWinWidth / g_dpiScaling, g_nWinHeight / g_dpiScaling);
}

void PAE3D_KeyboardDown(SDL_Keycode key) {
	switch (key) {
	case SDLK_LSHIFT:
		m_shiftDownNow = true;
		break;
	case SDLK_LCTRL:
		m_ctrlDownNow = true;
		break;
	case SDLK_g:
		g_model->Subdivide(true);
		break;
	case SDLK_3:
		g_model->Duplicate();
		break;
	case 127:
		g_model->DeleteSelection();
		break;
	case SDLK_f:
		g_model->SetSelectType(PAE3D_SELECT_FACES);
		leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
		break;
	case SDLK_v:
		g_model->SetSelectType(PAE3D_SELECT_VERTICES);
		leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
		break;
	case SDLK_e:
		g_model->SetSelectType(PAE3D_SELECT_EDGES);
		leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
		break;
	case SDLK_m:
		handleMode = PAE3D_HANDLE_MOVE;
		break;
	case SDLK_s:
		handleMode = PAE3D_HANDLE_SCALE;
		break;
	case SDLK_x:
		g_model->Extrude();
		break;
	case SDLK_l:
		g_model->Merge();
		break;
	case SDLK_p:
		displayMode++;
		displayMode%=2;
		break;
	case SDLK_d:
		g_model->Subdivide(false);
		break;
	case SDLK_c:
		switch(leftCLickOperation){
		case PAE3D_LEFTCLICK_NOTHING:
			leftCLickOperation = PAE3D_LEFTCLICK_COLOR;
			break;
		case PAE3D_LEFTCLICK_COLOR:
			leftCLickOperation = PAE3D_LEFTCLICK_NOTHING;
			break;
		}
		break;
	case SDLK_a:
		g_model->SelectAll();
		break;
	}
}

void PAE3D_KeyboardUp(SDL_Keycode key) {
	switch(key) {
		case SDLK_LSHIFT:
			m_shiftDownNow = false;
			break;
		case SDLK_LCTRL:
			m_ctrlDownNow = false;
			break;
	}
}

void PAE3D_MouseClick(int button, int state, int x, int y) {
	printf("%d x %d\n", x, y);
	g_lastX = x;
	g_lastY = y;
	if (g_toolbar->Click(button, state, x, y, g_nWinWidth, g_nWinHeight)) {
		return;
	}
	switch (button) {
	case SDL_BUTTON_MIDDLE:
		q_middleClickDown = state == SDL_PRESSED;
		m_shiftDownLastMiddleClick = m_shiftDownNow;
		m_ctrlDownLastMiddleClick = m_ctrlDownNow;
		break;
	case SDL_BUTTON_LEFT:
		q_leftClickDown = state == SDL_PRESSED;
		if (state == SDL_PRESSED) {
			mode = PAE3D_SELECT_LEFT;
		}
		break;
	case SDL_BUTTON_RIGHT:
		if (state == SDL_PRESSED) {
			mode = PAE3D_SELECT_RIGHT;
		}
		break;
	}
}

void PAE3D_MouseMove(int x, int y) {
	int height = g_nWinHeight;
	float worldDX = ((g_lastX-x)*cos(rotation*M_PI/180)+(y-g_lastY)*sin(tilt*M_PI/180)*sin(rotation*M_PI/180))*zoom/height/4;
	float worldDY = (y-g_lastY)*cos(tilt*M_PI/180)*zoom/height/4;
	float worldDZ = ((g_lastX-x)*sin(rotation*M_PI/180)+(g_lastY-y)*sin(tilt*M_PI/180)*cos(rotation*M_PI/180))*zoom/height/4;
	if (q_middleClickDown) {
		if (m_ctrlDownLastMiddleClick) {
			zoom *= 1 + (y - g_lastY)*0.005;
		} else if (m_shiftDownLastMiddleClick) {
			g_center.x += worldDX;
			g_center.y += worldDY;
			g_center.z += worldDZ;
		} else {
			rotation += x - g_lastX;
			tilt += y - g_lastY;
			if (tilt > 85) {
				tilt = 85;
			}
			if (tilt < -85) {
				tilt = -85;
			}
		}
	}
	if (q_leftClickDown) {
		switch(handleMode) {
		case PAE3D_HANDLE_MOVE:
			switch (g_model->SelectedHandle) {
			case PAE3D_SELECT_X_HANDLE:
				g_model->MoveSelected(-worldDX * 2, 0, 0);
				break;
			case PAE3D_SELECT_Y_HANDLE:
				g_model->MoveSelected(0, -worldDY * 2, 0);
				break;
			case PAE3D_SELECT_Z_HANDLE:
				g_model->MoveSelected(0, 0, -worldDZ * 2);
				break;
			}
			break;
		case PAE3D_HANDLE_SCALE:
			switch (g_model->SelectedHandle) {
			case PAE3D_SELECT_X_HANDLE:
				g_model->ScaleSelected(-worldDX * 2 / zoom * 10, 0, 0);
				break;
			case PAE3D_SELECT_Y_HANDLE:
				g_model->ScaleSelected(0, -worldDY * 2 / zoom * 10, 0);
				break;
			case PAE3D_SELECT_Z_HANDLE:
				g_model->ScaleSelected(0, 0, -worldDZ * 2 / zoom * 10);
				break;
			}
			break;
		}
	}
	g_lastX = x;
	g_lastY = y;
}

void PAE3D_SetLights() {
	float direction[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	float diffintensity[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffintensity);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glEnable(GL_LIGHT0);
}

void PAE3D_SetCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(PAE3D_FOVY, (double) g_nWinWidth / (double) g_nWinHeight, PAE3D_ZNEAR_3D, PAE3D_ZFAR_3D);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float xpos = -zoom*cos(tilt*M_PI/180)*sin(rotation*M_PI/180) + g_center.x;
	float ypos = zoom*sin(tilt*M_PI/180) + g_center.y;
	float zpos = zoom*cos(tilt*M_PI/180)*cos(rotation*M_PI/180) + g_center.z;
	gluLookAt(xpos, ypos, zpos, g_center.x, g_center.y, g_center.z, 0, 1, 0);
}
