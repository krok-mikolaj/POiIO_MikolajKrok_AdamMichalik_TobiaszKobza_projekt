#pragma once
#include "Vector2D.h"

// Represents a circular obstacle on the simulation map.
// Robots will steer away from obstacles using the obstacleAvoidance() behavior in Swarm.
struct Obstacle
{
    Vector2D position;
    float radius; // collision/avoidance radius in pixels

    Obstacle(float x, float y, float r)
        : position(x, y), radius(r) {}
};
