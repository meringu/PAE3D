#pragma once

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL
#define png_voidp_NULL NULL
#define TEXTURE_NONE 0
#define TEXTURE_MATERIAL 1
#define TEXTURE_UV 2

typedef struct texInfo {
	char *filename;
	unsigned char* textureData;
	int format;
	int width;
	int height;
	bool hasAlpha;
} TextureInfo;

int loadTextureFromJPEG(char*,TextureInfo*);
int loadTextureFromPNG(char*, TextureInfo*);
GLuint openTexture(const char* filename, GLuint);
GLuint openTexture(const char* filename);
TextureInfo openTextureInfo(const char* filename);
GLuint openCubeMap(const char*, const char*, const char*, const char*, const char*, const char*);

