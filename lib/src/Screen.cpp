#include "../include/Screen.hpp"
#include "../include/object.h"
#include <iostream> 
#include <vector>
#include <windows.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <gl/gl.h>
#include <gl/glu.h>
#include <glm.hpp>

static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;
GLfloat ballRadius = 0.5f;   // Radius of the bouncing ball
GLfloat ballX = 0.0f;         // Ball's center (x, y) position
GLfloat ballY = 0.0f;
GLfloat ballXMax = 5; 
GLfloat ballXMin = -5;
GLfloat ballYMax = 5; 
GLfloat ballYMin = -5 ; // Ball's center (x, y) bounds
GLfloat xSpeed = 0.1f;      // Ball's speed in x and y directions
GLfloat ySpeed = 0.12f; 
bool down = false;

	Screen::Screen(int width, int height){
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
			std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
			
		}

		win = SDL_CreateWindow("Flapp'IMAC", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_OPENGL );
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

	/* 
	void bouncingball() {
		
		glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
		glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
		glLoadIdentity();              // Reset model-view matrix

		glTranslatef(ballX, ballY, 0.0f);  // Translate to (xPos, yPos)
		// Use triangular segments to form a circle
		glBegin(GL_TRIANGLE_FAN);
		glColor3f(0.0f, 0.0f, 1.0f);  // Blue
		glVertex2f(0.0f, 0.0f);       // Center of circle
		int numSegments = 100;
		GLfloat angle;
		for (int i = 0; i <= numSegments; i++) { // Last vertex same as first vertex
			angle = (float)(i * 2.0f * M_PI / numSegments);  // 360 deg for all segments
			glColor3f(0.0f, 0.0f, 1.0f);
			glVertex2f(cos(angle) * ballRadius, sin(angle) * ballRadius);
		}
		glEnd();

		// Animation Control - compute the location for the next refresh
		ballX += xSpeed;
		ballY += ySpeed;
		// Check if the ball exceeds the edges
		if (ballX > ballXMax) {
			ballX = ballXMax;
			xSpeed = -xSpeed;
		}
		else if (ballX < ballXMin) {
			ballX = ballXMin;
			xSpeed = -xSpeed;
		}
		if (ballY > ballYMax) {
			ballY = ballYMax;
			ySpeed = -ySpeed;
		}
		else if (ballY < ballYMin) {
			ballY = ballYMin;
			ySpeed = -ySpeed;
		}
		
	} */



void drawBox() {

		glBegin(GL_QUADS);              // Each set of 4 vertices form a quad
	

		glColor3f(1.f, 0.0f, 0.0f); 
		glVertex2f(-0.5f, -0.5f);
		
		glVertex2f(-0.5f, 0.5f);

		glVertex2f(0.5f, 0.5f);
		
		glVertex2f(0.5f, -0.5f);
		glEnd(); 

	}


void drawobstacle() {

	glBegin(GL_QUADS);              // Each set of 4 vertices form a quad


	glColor3f(0.f, 0.8f, 0.0f);
	glVertex2f(-1.f, 5.f);

	glColor3f(0.8f, 1.0f, 0.1f);
	glVertex2f(-1.f, 20.f);

	glColor3f(0.f, 0.8f, 0.0f);
	glVertex2f(1.f, 20.f);

	glColor3f(0.8f, 1.0f, 0.1f);
	glVertex2f(1.f, 5.f);


	glColor3f(0.f, 0.8f, 0.0f);
	glVertex2f(-1.f, -20.f);

	glColor3f(0.8f, 1.0f, 0.1f);
	glVertex2f(-1.f, -1.f);

	glColor3f(0.f, 0.8f, 0.0f);
	glVertex2f(1.f, -1.f);

	glColor3f(0.8f, 1.0f, 0.1f);
	glVertex2f(1.f, -20.f);


	glEnd();

}


const float g = 9.81f;  // Gravity of Earth in m/s�
float gravity(glm::vec2 p1_pos, glm::vec2 p2_pos, float p1_mass, float p2_mass)
{
	float distance = (p2_pos - p1_pos).length();
	return g * p1_mass * p2_mass / (distance*distance);
}


void Screen::render(){
	
		int loop = 1;
		// Timing variables
		Uint32 old_time, current_time;
		float ftime;
		//flying process
		bool up = false; 
		bool plane = false;
		float cpt = 0.f;
		
		object ball; // piou piou 
		glm::vec2 earth(0., -15); // earth position to set gravity

		//obstacles 
		float pos = 12;
		float pos2 = 28;

		srand(time(NULL));

		/* generate secret number between 1 and 8 for the different kinds of obstacles: */
		int iSecret = rand() % 8 + 1;
		int iSecret2 = rand() % 8 + 1;

		
		current_time = SDL_GetTicks(); // Need to initialize this here for event loop to work
		while (loop){
			
			/*setting time*/
			old_time = current_time;
			current_time = SDL_GetTicks();
			ftime = (current_time - old_time) / 1000.0f;

			/*placing obstacles*/
			if (pos < -17) { pos = 15; iSecret = rand() % 8 + 1;}
			pos -= 10 * ftime;

			if (pos2 < -17) { pos2 = 15; iSecret2 = rand() % 8 + 1; }
			pos2 -= 10 * ftime;



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
				ball.add_force(gravity(ball.pos, earth, ball.mass, 1.5)*(earth - ball.pos), ftime);
			}
			
			ball.pos = ball.vel * ftime; 
			
			glClearColor(0, 0, 0, 0);

			glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer
			glMatrixMode(GL_MODELVIEW);    // To operate on the model-view matrix
			glLoadIdentity();              // Reset model-view matrix
			

			glPushMatrix();
			if (ball.pos.y*10 < -10) glTranslatef(0.f,-10.f, 0.0f);
			else glTranslatef(ball.pos.x*10, ball.pos.y*10, 0.0f);
				drawBox();
			glPopMatrix();


			glPushMatrix();
			glTranslatef(pos, iSecret-4, 0.0f);
				drawobstacle();
			glPopMatrix();
			glTranslatef(pos2, iSecret2-4, 0.0f);
			drawobstacle();
			glPopMatrix();

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
		}
		SDL_Quit();

			

} 

