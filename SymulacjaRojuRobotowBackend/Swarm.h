#pragma once
#include "Robot.h"
#include "Vector2D.h"
#include <vector>

class Swarm
	{
	private:
		// Swarm
		std::vector<Robot*> swarm;

		// Map size (widht and height)
		float x_map, y_map; 

		// Behavior weights
		float sep_w = 1.5f;
		float alig_w = 1.0f;
		float coh_w = 1.0f;

	public:
		Swarm(float x_map, float y_map, unsigned int num_r);
		~Swarm(); // destructor which deletes all members of swarm object
		void addRobot(); // adds single Robot to the swarm
		void removeRobot(); // removes single random Robot from the swarm

		// Flocking methods
		Vector2D cohesion(Swarm swarm);
	};