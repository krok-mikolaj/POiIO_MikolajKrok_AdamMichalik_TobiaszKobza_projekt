#include "pch.h"
#include "Swarm.h"
#include <random>
#include <cmath>

using namespace std;

static std::mt19937 rng(std::random_device{}());

Swarm::Swarm(float x_map, float y_map, unsigned int num_r)
{
	this->x_map = x_map;
	this->y_map = y_map;
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

const vector<Robot*>& Swarm::getSwarm()
{
	return swarm;
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

Vector2D Swarm::cohesion(Robot* robot, const vector<Robot*>& swarm)
{
	if (swarm.empty()) return Vector2D();

	Vector2D S_pos; // Sum of position of the group of robots
	unsigned int n = 0; // Number of neighboars in perception radius

	for (Robot* bot : swarm)
	{
		if ((robot != bot) && (robot->position.distanceTo(bot->position) < robot->percepR)) // Checks if neighbor is in robot's visual field and if it isn't the same robot
		{
			S_pos = S_pos + bot->position;
			n++;
		}
	}
	if (n == 0) return Vector2D();
	Vector2D avgPos = S_pos * (1.0f / n);
	Vector2D t_Vel = avgPos - robot->position; // Steers robot towards center of mass of local group of neighbors
	t_Vel = t_Vel.normalized() * robot->maxSpeed;
	Vector2D steerF = (t_Vel - robot->velocity) * robot->mass;
	steerF.limit(robot->maxF);
	return steerF;
}

Vector2D Swarm::seperation(Robot* robot, const vector<Robot*>& swarm)
{

	if (swarm.empty()) return Vector2D();

	Vector2D S_pos; // Sum of steering force (respresented as target velocity) from all of the neighbors, dependend from how close they are to robot
	Vector2D Dir; // Direction vector from robot to neighbor
	unsigned int n = 0; // Number of neighbors in perception radius

	for (Robot* bot : swarm)
	{
		float d = robot->position.distanceTo(bot->position); // distance to neighbor

		if ((robot != bot) && (d < robot->percepR))
		{
			Dir = robot->position - bot->position;
			S_pos = S_pos + Dir.normalized() * (1.0f / d); // Steering force is inverse-proportional to the distance to a neighbor
			n++;
		}
	}

	if (n == 0) return Vector2D();

	Vector2D t_Vel = S_pos * (1.0f / n);
	t_Vel = t_Vel.normalized() * robot->maxSpeed;
	Vector2D steerF = (t_Vel - robot->velocity) * robot->mass;
	steerF.limit(robot->maxF);
	return steerF;
}

Vector2D Swarm::alignment(Robot* robot, const vector<Robot*>& swarm)
{
	if (swarm.empty()) return Vector2D();

	Vector2D S_vel; // Sum of velocities of all neighbors
	unsigned int n = 0; // Number of neighbors in perception radius

	for (Robot* bot : swarm)
	{
		if ((robot != bot) && (robot->position.distanceTo(bot->position) < robot->percepR))
		{
			S_vel = S_vel + bot->velocity;
			n++;
		}
	}

	if (n == 0) return Vector2D();

	Vector2D t_Vel = S_vel * (1.0f / n); // Computing avarage direction (velocity) of each robot which local claster of robots will follow
	t_Vel = t_Vel.normalized() * robot->maxSpeed;
	Vector2D steerF = (t_Vel - robot->velocity) * robot->mass;
	steerF.limit(robot->maxF);
	return steerF;
}

void Swarm::flock(Robot* robot, const vector<Robot*>& swarm)
{
	Vector2D steerF;
	steerF = steerF + cohesion(robot, swarm) * coh_w;
	steerF = steerF + seperation(robot, swarm) * sep_w;
	steerF = steerF + alignment(robot, swarm) * align_w;

	robot->applyForce(steerF);
}

void Swarm::update()
{
	for (Robot* robot : swarm) // Calculates steering force and new acceleration for each robot in exacts frame of time
	{
		flock(robot, swarm);
	}

	for (Robot* robot : swarm) // Corrects velocity and position regarding to steering force from flocking algorithm
	{
		robot->velocity = robot->velocity + robot->acceleration;
		robot->velocity.limit(robot->maxSpeed);
		robot->position = robot->position + robot->velocity;
		robot->acceleration = Vector2D();
		wrapEdges(robot);
	}
}

void Swarm::update_dt(double dt)
{
	for (Robot* robot : swarm) // Calculates steering force and new acceleration for each robot in exacts frame of time
	{
		flock(robot, swarm);
	}

	for (Robot* robot : swarm) // Corrects velocity and position regarding to steering force from flocking algorithm
	{
		robot->velocity = robot->velocity + robot->acceleration * dt;
		robot->velocity.limit(robot->maxSpeed);
		robot->position = robot->position + robot->velocity * dt;
		robot->acceleration = Vector2D();
		robot->updateRotation();
		wrapEdges(robot);
	}
}

void Swarm::wrapEdges(Robot* robot)
{
	if (robot->position.x > x_map) robot->position.x = 0;
	if (robot->position.x < 0) robot->position.x = x_map;
	if (robot->position.y > y_map) robot->position.y = 0;
	if (robot->position.y < 0) robot->position.y = y_map;
}