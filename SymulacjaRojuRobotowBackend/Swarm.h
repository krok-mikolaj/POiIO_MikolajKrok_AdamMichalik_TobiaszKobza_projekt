#pragma once
#include "Robot.h"
#include "Vector2D.h"
#include "Obstacle.h"
#include <vector>

// =============================================================================
// NOTE FOR PATTERN FORMATION MERGE:
//   When merging with the pattern_forming branch, add the PatternFormation
//   include and fields here (activePattern, slots, assignment, seekSlot).
//   In flock(), the seekTarget() call should be skipped when a pattern is
//   active (activePattern != PatternType::NONE), because per-robot slot
//   targets will take over directional guidance.
// =============================================================================

class Swarm
{
private:
    // Swarm
    std::vector<Robot*> swarm;

    // Map size (width and height)
    float x_map, y_map;

    // --- Flocking behavior weights ---
    float sep_w   = 2.0f;
    float align_w = 2.0f;
    float coh_w   = 2.0f;

    // --- Obstacle avoidance ---
    std::vector<Obstacle> obstacles;
    float obs_w = 2.0f; // obstacle avoidance weight (high = hard boundary)

    // --- Target point (global seek for the whole swarm) ---
    Vector2D targetPoint;
    bool     hasTarget  = false;
    float    target_w   = 1.5f;   // target seek weight
    float    slowRadius = 120.0f; // arrival slowing radius (pixels)

    // --- Internal steering helpers ---

    // Steer away from all obstacles within detection range.
    Vector2D obstacleAvoidance(Robot* robot, const std::vector<Obstacle>& obs);

    // Seek / arrive at a single world-space target position.
    // Uses arrival behaviour: slows down inside slowRadius.
    Vector2D seekTarget(Robot* robot, const Vector2D& target);

public:
    Swarm(float x_map, float y_map, unsigned int num_r);
    ~Swarm();

    const std::vector<Robot*>& getSwarm();
    void addRobot();
    void removeRobot();

    int    getRobotsCount() const { return (int)swarm.size(); }
    Robot* getRobot(int i)        { return swarm[i]; }

    // --- Obstacle API ---
    void addObstacle(float x, float y, float radius);
    void removeObstacle(int index);
    void clearObstacles();
    const std::vector<Obstacle>& getObstacles() const { return obstacles; }

    // --- Target API ---
    void     setTarget   (float x, float y);
    void     clearTarget ();
    bool     getHasTarget()  const { return hasTarget; }
    Vector2D getTarget   ()  const { return targetPoint; }

    // --- Behavior weight tuning ---
    void setObstacleWeight(float w) { obs_w      = w; }
    void setTargetWeight  (float w) { target_w   = w; }
    void setSlowRadius    (float r) { slowRadius = r; }

    // --- Flocking methods ---
    Vector2D cohesion  (Robot* robot, const std::vector<Robot*>& swarm);
    Vector2D seperation(Robot* robot, const std::vector<Robot*>& swarm);
    Vector2D alignment (Robot* robot, const std::vector<Robot*>& swarm);

    // Applies all active steering forces to robot->acceleration.
    // Order: flocking (sep/align/coh) -> obstacle avoidance -> target seek
    void flock(Robot* robot, const std::vector<Robot*>& swarm);

    void update();
    void update_dt(double dt);
    void wrapEdges(Robot* robot);
};
