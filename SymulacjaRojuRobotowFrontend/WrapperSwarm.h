#pragma once
#include "Swarm.h"

using namespace System;
using namespace System::Collections::Generic;

public ref class WrapperSwarm
{
private:
    Swarm* swarm;

public:
    WrapperSwarm(float x_map, float y_map, unsigned int num_r);
    ~WrapperSwarm();

    // --- Swarm management ---
    void addRobot();
    void removeRobot();
    void update(double dt);

    // --- Robot data for rendering ---
    List<Tuple<float, float>^>^ getSwarmPositions();
    List<float>^                getSwarmRotations();

    // --- Obstacle API ---
    // Add a circular obstacle at (x, y) with given radius.
    void addObstacle(float x, float y, float radius);
    // Remove obstacle by index (0-based). No-op if index is out of range.
    void removeObstacle(int index);
    // Remove all obstacles.
    void clearObstacles();
    // Returns list of (x, y, radius) tuples — one per obstacle.
    List<Tuple<float, float, float>^>^ getObstacles();

    // --- Target API ---
    // Set a single global seek target for the whole swarm.
    void setTarget(float x, float y);
    // Remove the target (robots return to free flocking).
    void clearTarget();
    // Returns true if a target is currently set.
    bool hasTarget();
    // Returns the current target as (x, y). Check hasTarget() first.
    Tuple<float, float>^ getTarget();

    // --- Optional weight tuning ---
    void setObstacleWeight(float w);
    void setTargetWeight  (float w);
};
