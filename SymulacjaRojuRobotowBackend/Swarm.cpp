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
	if (swarm.empty()) return;
	std::uniform_int_distribution <unsigned int> l(0, swarm.size() - 1);
	unsigned int idx = l(rng);
	delete swarm[idx];
	swarm.erase(swarm.begin() + idx);
}

Vector2D cohesion(Robot* robot,const vector<Robot*>& swarm)
{
	Vector2D S_pos(); // Sum of position of the group of robots
	unsigned int n = 0; // Number of neighboars in perception radius

	if (swarm.empty()) return;

	for (Robot* bot : swarm)
	{
		if ((robot = !bot) && (robot->distanceTo(bot) < robot->percepR))
		{
			S_pos += bot->position;
			n++;
		}
	}

	Vector2D avgPos = S_pos * (1 / n);
	Vector2D t_Vel = avgPos - robot->position;
	t_Vel = t_Vel.normalized() * robot->maxSpeed;
	Vector2D steerF = robot->mass * (robot->velocity - t_Vel);
	return steerF;
}