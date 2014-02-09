#include <iostream>
#include "../lib/include/Screen.hpp"

int main(int argc, char *argv[]){
	int game = 0; 
	int menu; 
	int loop = 1; 
	Screen screen(800,800);


	while (loop) {
		menu = screen.menu();
		if (menu == 0) loop = 0;
		if (menu == 1)  { menu = 2;  screen.render(); }

	}
	

	return 0;
}

