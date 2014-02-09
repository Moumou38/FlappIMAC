#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL/SDL.h> 
#include "SDL_image.h" 

#include <gl/gl.h>
#include <gl/glu.h>



GLuint loadTexture(SDL_Surface* tex);

SDL_Surface* loadImage(const char* image);



