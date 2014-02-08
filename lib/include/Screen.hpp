#pragma once
#include <iostream> 
#include <SDL/SDL.h>




class Screen {
	private:
		SDL_Window* win;
	public:
		Screen(int width, int height) ;
		void render();
				
};
