#include "ImageLoader.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

GLuint openTexture(const char* filename) {
	GLuint TextureID = 0;
	SDL_Surface* Surface = IMG_Load(filename);
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	int Mode = GL_RGB;
	if(Surface->format->BytesPerPixel == 4) {
		Mode = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, Mode, Surface->w, Surface->h, 0, Mode, GL_UNSIGNED_BYTE, Surface->pixels);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return TextureID;
}
