#include "../include/Screen.hpp"
#include "../include/object.h"
#include "../include/image.h"
#include <iostream> 
#include <vector>
#include <windows.h>
#include <cmath>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <gl/gl.h>
#include <gl/glu.h>
#include <glm.hpp>

static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60; 

	Screen::Screen(int width, int height){
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
			std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
			
		}

		win = SDL_CreateWindow("Flapp'IMAC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL );
		SDL_GL_CreateContext(win);
		if (win == nullptr){
			std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
			
		}

		
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-10, 10, -10, 10);

		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(win);

		glPointSize(5.f);
		glLineWidth(2.);

	}



	void drawAxis(){
		glBegin(GL_LINES);
		glColor3ub(255, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);

		glColor3ub(0, 255, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);

		glColor3ub(0, 0, 255);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);
		glEnd();
	}



void drawBox(GLuint tex) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();
	
	glPushMatrix();
	
	glBegin(GL_QUADS);
	//piou piou
	glColor3ub(255, 255, 255);
	glTexCoord2f(0, 1.f);
	glVertex2f(-1, -1);

	glTexCoord2f(0, 0);
	glVertex2f(-1, 1);

	glTexCoord2f(1, 0);
	glVertex2f(1, 1);
	

	glTexCoord2f(1, 1.f);
	glVertex2f(1, -1);
	
	
	glEnd();
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPopMatrix();

	}


void drawobstacle(GLuint tex) {

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPushMatrix();

	glPushMatrix();

	glBegin(GL_QUADS);             // Each set of 4 vertices form a quad


	glTexCoord2f(0, 1.f);
	glVertex2f(-1.f, 3.f);

	glTexCoord2f(0, 0);
	glVertex2f(-1.f, 20.f);

	glTexCoord2f(1, 0.f);
	glVertex2f(1.f, 20.f);

	glTexCoord2f(1, 1.f);
	glVertex2f(1.f, 3.f);


	glTexCoord2f(0, 1.f);
	glVertex2f(-1.f, -20.f);

	glTexCoord2f(0, 0.f);
	glVertex2f(-1.f, -3.f);

	glTexCoord2f(1, 0.f);
	glVertex2f(1.f, -3.f);

	glTexCoord2f(1.f, 1.f);
	glVertex2f(1.f, -20.f);


	glEnd();
	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glPopMatrix();

}


const float g = 9.81f;  // Gravity of Earth in m/s²
float gravity(glm::vec2 p1_pos, glm::vec2 p2_pos, float p1_mass, float p2_mass)
{
	float distance = (p2_pos - p1_pos).length();
	return g * p1_mass * p2_mass / (distance*distance);
}


void Screen::render(){
	
		int loop = 1;
		bool dead = false; 
		// Timing variables
		Uint32 old_time, current_time;
		float ftime;
		//flying process
		bool up = false; 
		bool plane = false;

		float cpt = 0.f;
		//background 
		SDL_Surface* bgTex = loadImage("../image/bg.png");
		GLuint bgTexid = loadTexture(bgTex);

		SDL_Surface* obstacleTex = loadImage("../image/obstacle.png");
		GLuint obstacleTexid = loadTexture(obstacleTex);

		object ball; // piou piou 
		SDL_Surface* ballTex = loadImage("../image/boule.png");
		ball.tex = loadTexture(ballTex);

		glm::vec2 earth(0., -15); // earth position to set gravity

		//obstacles 
		float pos = 13;
		float pos2 = 24;
		float pos3 = 35;

		srand(time(NULL));

		/* generate secret number between 1 and 8 for the different kinds of obstacles: */
		int iSecret = rand() % 9 + 1;
		int iSecret2 = rand() % 9 + 1;
		int iSecret3 = rand() % 9 + 1;

		
		current_time = SDL_GetTicks(); // Need to initialize this here for event loop to work
		while (loop){
			
			/*setting time*/
			old_time = current_time;
			current_time = SDL_GetTicks();
			ftime = (current_time - old_time) / 1000.0f;

			/*placing obstacles*/
			if (pos < -17) { pos = 15; iSecret = rand() % 9 + 1;}
			pos -= 10 * ftime;

			if (pos2 < -17) { pos2 = 15; iSecret2 = rand() % 9 + 1; }
			pos2 -= 10 * ftime;

			if (pos3 < -17) { pos3 = 15; iSecret3 = rand() % 9 + 1; }
			pos3 -= 10 * ftime;


			/*flapping bird */
			if (up == true && plane == false) { //fly
				ball.add_force(glm::vec2(0.f, 15.81f), ftime);
				cpt += ftime;
				if (cpt > 0.3) {
					up = false;
					cpt = 0.f;
					plane = true;
				}
			}			
			if (plane == true) { //plane
				ball.add_force(glm::vec2(0.f, 0.f), ftime);
				cpt += ftime;
				if (cpt > 0.1) {				
					cpt = 0.f;
					plane = false;
				}

			}
			if (plane == false && up == false && ball.pos.y * 10 > -10) { //fall
				ball.add_force(gravity(ball.pos, earth, ball.mass, 1.5 + cpt*2)*(earth - ball.pos), ftime);
				cpt += ftime;
					if (cpt > 3) {
						cpt = 0.f;
					}
			}
			
			ball.pos = ball.vel * ftime; 
			
			/* hit obstacle */
			if (ball.pos.x * 10+ 0.4 >= pos-0.5 && //collision left
				ball.pos.x * 10 - 0.4 <= pos + 0.5 &&  //collision right
				(ball.pos.y * 10 + 0.3 >= iSecret - 2 || //top collision  (path height = 6)
				ball.pos.y * 10 - 0.3 <= iSecret - 7)) dead = true;

			if (ball.pos.x * 10 + 0.4 >= pos2 - 0.5 && //collision left
				ball.pos.x * 10 - 0.4 <= pos2 + 0.5 &&  //collision right
				(ball.pos.y * 10 + 0.3 >= iSecret2 - 2 || //top collision  (path height = 6)
				ball.pos.y * 10 - 0.3 <= iSecret2 - 7)) dead = true;

			if (ball.pos.x * 10 + 0.4 >= pos3 - 0.5 && //collision left
				ball.pos.x * 10 - 0.4 <= pos3 + 0.5 &&  //collision right
				(ball.pos.y * 10 + 0.3 >= iSecret3 - 2 || //top collision  (path height = 6)
				ball.pos.y * 10 - 0.3 <= iSecret3 - 7)) dead = true;



			/*this is where we draw */
			glClearColor(0, 0, 0, 0);

			glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
			glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
			glLoadIdentity();              // Reset model-view matrix
			
			glPushMatrix();
				glScalef(20, 10, 1);
				drawBox(bgTexid);
			glPopMatrix();


			glPushMatrix();
			if (ball.pos.y*10 < -10) glTranslatef(0.f,-10.f, 0.0f); //collision sol
			else glTranslatef(ball.pos.x*10, ball.pos.y*10, 0.0f);

				if(up) glRotatef(-30, 0.,0., 1.);

				drawBox(ball.tex);
			glPopMatrix();


			glPushMatrix();
			glTranslatef(pos, iSecret-5 , 0.0f);
				drawobstacle(obstacleTexid);
			glPopMatrix();
			glPushMatrix();
			
			glTranslatef(pos2, iSecret2 - 5, 0.0f);
			drawobstacle(obstacleTexid);
			glPopMatrix();
			
			glPushMatrix();
			glTranslatef(pos3, iSecret3 - 5, 0.0f);
			drawobstacle(obstacleTexid);
			glPopMatrix();
			//drawAxis();

			SDL_GL_SwapWindow(win);


			SDL_Event e;
			while (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_QUIT:
					loop = 0;
					break;

				case SDL_MOUSEBUTTONDOWN :
					
						if (e.button.button == SDL_BUTTON_LEFT)
						{
							up = true;
							plane = false;
							cpt = 0.f;
						}
						break;
				case SDL_KEYDOWN:
					switch (e.key.keysym.sym){
					case SDLK_q:
						loop = 0;
						break;

					default:
						break;
					}
					break;

				default:
					break;
				}
			}

			
			if (ftime < FRAMERATE_MILLISECONDS) {
				SDL_Delay(FRAMERATE_MILLISECONDS - ftime);
			} 
			if (dead) loop = 0;
		}
		SDL_Quit();

			

} 


