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
	float sep_w = 2.0f;
	float align_w = 2.0f;
	float coh_w = 2.0f;

public:
	Swarm(float x_map, float y_map, unsigned int num_r);
	~Swarm(); // destructor which deletes all members of swarm object
	const std::vector<Robot*>& getSwarm();
	void addRobot(); // adds single Robot to the swarm
	void removeRobot(); // removes single random Robot from the swarm

	int getRobotsCount() const { return (int)swarm.size(); }
	Robot* getRobot(int i) { return swarm[i]; }

	// Flocking methods
	Vector2D cohesion(Robot* robot, const std::vector<Robot*>& swarm);
	Vector2D seperation(Robot* robot, const std::vector<Robot*>& swarm);
	Vector2D alignment(Robot* robot, const std::vector<Robot*>& swarm);
	void flock(Robot* robot, const std::vector<Robot*>& swarm);

	void update();
	void update_dt(double dt);
	void wrapEdges(Robot* robot);
};