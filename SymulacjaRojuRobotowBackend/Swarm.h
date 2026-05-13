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
		Swarm(float x_map,float y_map, unsigned int num_r)
			{	
			static std::mt19937 rng(std::random_device{}());
			std::uniform_real_distribution <float> width(0, x_map);
			std::uniform_real_distribution <float> height(0, y_map);

				for (int i = 0; i < num_r; i++)
					{
						Robot* Bot = new Robot(width(rng), height(rng));
						swarm.push_back(Bot);
					}
			}

	};