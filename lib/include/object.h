#pragma once
#include <glm.hpp>

class object {

	/* dt = time_now - time_prev_frame
		acc = -9.8
		vel += acc * dt
		pos += vel * dt
		draw object at pos */
public : 
	glm::vec2 pos; 
	glm::vec2 vel; 
	float mass;
	object(glm::vec2 pos, glm::vec2 vel, float mass) : pos(pos), vel(vel), mass(mass) {};
	object() : pos((0.f,0.f)), vel((0.f,0.f)), mass(0.25f) {} ;
	object(const object &);

	void object::add_force(glm::vec2 force, float dt)
	{
		vel += (force / mass) * dt;
	}
};