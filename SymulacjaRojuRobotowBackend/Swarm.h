#pragma once
#include "Robot.h"
#include "Vector2D.h"
#include "Obstacle.h"
#include <vector>

enum class SwarmBehavior
{
	Flocking,
	CircleFormation,
	HexGridFormation,
	CustomFormation
};

class Swarm
{
private:
	// Swarm
	std::vector<Robot*> swarm;

	// Map size (widht and height)
	float x_map, y_map;

	// Behavior weights
	float sep_w = 2.0f;
	float align_w = 1.5f;
	float coh_w = 2.0f;

	// ── Przeszkody ──
	std::vector<Obstacle> obstacles;
	float obs_w = 2.0f;

	// ── Cel ──
	Vector2D targetPoint;
	bool     hasTarget = false;
	float    target_w = 1.5f;
	float    slowRadius = 120.0f;

	SwarmBehavior currentBehavior = SwarmBehavior::Flocking;
	std::vector<Vector2D> customFormation; // względne pozycje dla Custom

	// Metody formacji
	Vector2D formationForce(Robot* robot, Vector2D targetPos);
	std::vector<Vector2D> getCircleTargets();
	std::vector<Vector2D> getHexGridTargets();
	std::vector<Vector2D> assignTargets(const std::vector<Vector2D>& targets);

	Vector2D obstacleAvoidance(Robot* robot, const std::vector<Obstacle>& obs);
	Vector2D seekTarget(Robot* robot, const Vector2D& target);

	std::vector<Vector2D> cachedTargets;
	int cachedRobotCount = 0;
	bool targetsDirty = true;

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

	void setMapSize(float x, float y) { x_map = x; y_map = y; }

	void addRobotAt(float x, float y);
	void removeNearestRobot(float x, float y);

	void setBehavior(SwarmBehavior behavior)
	{
		currentBehavior = behavior;
		targetsDirty = true;  // wymuś przeliczenie
	}
	SwarmBehavior getBehavior() { return currentBehavior; }
	void setCustomFormation(const std::vector<Vector2D>& positions)
	{
		customFormation = positions;
		targetsDirty = true;
	}

	// ── Przeszkody ──
	void addObstacle(float x, float y, float radius);
	void removeNearestObstacle(float x, float y);
	void clearObstacles();
	const std::vector<Obstacle>& getObstacles() const { return obstacles; }

	// ── Cel ──
	void     setTarget(float x, float y);
	void     clearTarget();
	bool     getHasTarget()  const { return hasTarget; }
	Vector2D getTarget()     const { return targetPoint; }
};