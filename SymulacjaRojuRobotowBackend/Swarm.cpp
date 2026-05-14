#include "pch.h"
#include "Swarm.h"
#include <random>
#include <cmath>

using namespace std;

static std::mt19937 rng(std::random_device{}());

Swarm::Swarm(float x_map, float y_map, unsigned int num_r)
{
	std::uniform_real_distribution <float> width(0, x_map);
	std::uniform_real_distribution <float> height(0, y_map);

	for (int i = 0; i < num_r; i++)
	{
		Robot* Bot = new Robot(width(rng), height(rng));
		swarm.push_back(Bot);
	}
}

Swarm::~Swarm()
{
	for (Robot* robot : swarm)
	{
		delete robot;
	}
	swarm.clear();
}

void Swarm::addRobot()
{
	std::uniform_real_distribution <float> width(0, x_map);
	std::uniform_real_distribution <float> height(0, y_map);

	Robot* Bot = new Robot(width(rng), height(rng));
	swarm.push_back(Bot);
}

void Swarm::removeRobot()
{
	std::uniform_real_distribution <unsigned int> l(0, swarm.size());
	swarm.erase(l);
}