#pragma once
#include "Robot.h"
#include "Vector2D.h"
#include "PatternFormation.h"
#include <vector>

class Swarm
{
private:
    // Swarm
    std::vector<Robot*> swarm;

    // Map size (width and height)
    float x_map, y_map;

    // Flocking behavior weights
    float sep_w = 1.5f;
    float align_w = 1.0f;
    float coh_w = 2.0f;

    // Pattern formation state
    PatternType             activePattern = PatternType::NONE;
    std::vector<Vector2D>   slots;          // target position per robot
    std::vector<int>        assignment;     // assignment[robotIdx] = slotIdx
    float                   formationForceWeight = 3.0f; // strength of seek force

    // Assign each robot to its nearest unoccupied slot (greedy closest-slot).
    // Called once when a pattern is activated / when robots are added/removed.
    void assignSlots();

    // Steering force towards robot's assigned slot
    Vector2D seekSlot(Robot* robot, const Vector2D& target);

 
public:
    Swarm(float x_map, float y_map, unsigned int num_r);
    ~Swarm();

    const std::vector<Robot*>& getSwarm();
    void addRobot();
    void removeRobot();
  
  
    int getRobotsCount() const { return (int)swarm.size(); }
	  Robot* getRobot(int i) { return swarm[i]; }
    // --- Pattern API ---
    // Activate a pattern centred on the map; spacing controls slot density.
    void setPattern(PatternType type, float spacing = 60.0f);
    // Return to free flocking mode.
    void clearPattern();
    PatternType getActivePattern() { return activePattern; }

    // --- Flocking methods ---
    Vector2D cohesion(Robot* robot, std::vector<Robot*>& swarm);
    Vector2D seperation(Robot* robot, std::vector<Robot*>& swarm);
    Vector2D alignment(Robot* robot, std::vector<Robot*>& swarm);
    void     flock(Robot* robot, std::vector<Robot*>& swarm);

    void update();
    void update_dt(double dt);
    void wrapEdges(Robot* robot);


	void setMapSize(float x, float y) { x_map = x; y_map = y; }

	void addRobotAt(float x, float y);
	void removeNearestRobot(float x, float y);
};

