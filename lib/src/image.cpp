#include "../include/image.h"

GLuint loadTexture(SDL_Surface* tex){

	GLuint textures;
	glGenTextures(1, &textures);

	glBindTexture(GL_TEXTURE_2D, textures);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	GLenum format;
	switch (tex->format->BytesPerPixel) {
	case 1:
		format = GL_RED;
		break;
	case 3:
		format = GL_RGB;
		break;
	case 4:
		if (tex->format->Rshift == 16)
			format = GL_RGBA;
		else
			format = GL_RGBA;
		break;
	default:
		fprintf(stderr, "Format des pixels de l’image non pris en charge\n");
		return 0;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->w, tex->h, 0, format, GL_UNSIGNED_BYTE, tex->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	return textures;
}

SDL_Surface* loadImage(const char* image){
	SDL_Surface* tex = IMG_Load(image);
	if (tex == NULL){
		fprintf(stderr, "Impossible d'afficher l'image %s.\n", image);
	}
	return tex;
}