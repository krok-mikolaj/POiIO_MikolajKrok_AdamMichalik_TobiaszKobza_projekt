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
	targetsDirty = true;
}

void Swarm::removeRobot()
{
	if (swarm.empty()) return;
	std::uniform_int_distribution <unsigned int> l(0, swarm.size() - 1);
	unsigned int idx = l(rng);
	delete swarm[idx];
	swarm.erase(swarm.begin() + idx);
	targetsDirty = true;
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
	// Przelicz cele tylko gdy coś się zmieniło
	if (currentBehavior != SwarmBehavior::Flocking)
	{
		if (targetsDirty || (int)swarm.size() != cachedRobotCount)
		{
			if (currentBehavior == SwarmBehavior::CircleFormation)
				cachedTargets = assignTargets(getCircleTargets());
			else if (currentBehavior == SwarmBehavior::HexGridFormation)
				cachedTargets = assignTargets(getHexGridTargets());
			else if (currentBehavior == SwarmBehavior::CustomFormation)
				cachedTargets = assignTargets(customFormation);

			cachedRobotCount = (int)swarm.size();
			targetsDirty = false;
		}
	}

	for (size_t i = 0; i < swarm.size(); i++)
	{
		if (currentBehavior == SwarmBehavior::Flocking)
		{
			flock(swarm[i], swarm);
		}
		else if (i < cachedTargets.size())
		{
			float sepWeight = sep_w * 0.3f;
			Vector2D sep = seperation(swarm[i], swarm) * sepWeight;
			Vector2D form = formationForce(swarm[i], cachedTargets[i]);
			swarm[i]->applyForce(form + sep);
		}
	}

	for (Robot* robot : swarm)
	{
		robot->velocity = robot->velocity + robot->acceleration * (float)dt;
		robot->velocity.limit(robot->maxSpeed);
		robot->position = robot->position + robot->velocity * (float)dt;
		robot->acceleration = Vector2D();
		robot->updateRotation();

		if (currentBehavior == SwarmBehavior::Flocking)
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

void Swarm::addRobotAt(float x, float y)
{
	Robot* Bot = new Robot(x, y);
	swarm.push_back(Bot);
	targetsDirty = true;
}

void Swarm::removeNearestRobot(float x, float y)
{
	if (swarm.empty()) return;

	int nearestIdx = 0;
	float minDist = FLT_MAX;

	for (size_t i = 0; i < swarm.size(); i++)
	{
		float dx = swarm[i]->position.x - x;
		float dy = swarm[i]->position.y - y;
		float dist = sqrt(dx * dx + dy * dy);
		if (dist < minDist)
		{
			minDist = dist;
			nearestIdx = i;
		}
	}

	delete swarm[nearestIdx];
	swarm.erase(swarm.begin() + nearestIdx);
	targetsDirty = true;
}

// Siła przyciągająca robota do jego miejsca w formacji
Vector2D Swarm::formationForce(Robot* robot, Vector2D targetPos)
{
	Vector2D t_Vel = targetPos - robot->position;
	float dist = t_Vel.mag();
	if (dist < 1.0f) return Vector2D();

	t_Vel = t_Vel.normalized() * robot->maxSpeed;
	Vector2D steerF = (t_Vel - robot->velocity) * robot->mass;
	steerF.limit(robot->maxF);
	return steerF;
}

std::vector<Vector2D> Swarm::getCircleTargets()
{
	std::vector<Vector2D> targets;
	int n = swarm.size();
	if (n == 0) return targets;

	float cx = x_map / 2.0f;
	float cy = y_map / 2.0f;
	float radius = std::min(x_map, y_map) * 0.35f;
	float angleStep = 2.0f * 3.14159265f / n;

	for (int i = 0; i < n; i++)
	{
		float angle = i * angleStep;
		targets.push_back(Vector2D(
			cx + radius * std::cos(angle),
			cy + radius * std::sin(angle)
		));
	}
	return targets;
}

std::vector<Vector2D> Swarm::getHexGridTargets()
{
	std::vector<Vector2D> targets;
	int n = swarm.size();
	if (n == 0) return targets;

	float spacing = 50.0f;
	float cx = x_map / 2.0f;
	float cy = y_map / 2.0f;

	// Hexgrid — co drugi rząd przesunięty o połowę
	int cols = (int)std::ceil(std::sqrt((float)n));
	int rows = (int)std::ceil((float)n / cols);

	float startX = cx - (cols * spacing) / 2.0f;
	float startY = cy - (rows * spacing * 0.866f) / 2.0f; // 0.866 = sqrt(3)/2

	for (int i = 0; i < n; i++)
	{
		int row = i / cols;
		int col = i % cols;
		float offsetX = (row % 2 == 0) ? 0.0f : spacing * 0.5f; // przesunięcie co drugi rząd

		targets.push_back(Vector2D(
			startX + col * spacing + offsetX,
			startY + row * spacing * 0.866f
		));
	}
	return targets;
}

// Swarm.cpp - nowa metoda przypisująca cele
std::vector<Vector2D> Swarm::assignTargets(const std::vector<Vector2D>& targets)
{
	std::vector<Vector2D> assigned(swarm.size());
	std::vector<bool> taken(targets.size(), false);

	for (int i = 0; i < swarm.size(); i++)
	{
		float minDist = FLT_MAX;
		int bestIdx = 0;

		for (int j = 0; j < targets.size(); j++)
		{
			if (taken[j]) continue;
			float d = swarm[i]->position.distanceTo(targets[j]);
			if (d < minDist)
			{
				minDist = d;
				bestIdx = j;
			}
		}
		taken[bestIdx] = true;
		assigned[i] = targets[bestIdx];
	}
	return assigned;
}
