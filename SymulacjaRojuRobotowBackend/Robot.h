#pragma once
#include "Vector2D.h"
#include <vector>

class Robot
{
    friend class Swarm;

private:
    Vector2D position;
    Vector2D velocity;
    Vector2D acceleration;
    float rotation;    // theta rotation
    float mass = 1.0f; // Robot weight 1kg

    float percepR; // Perception radius - circle of vision of robot
    float maxF; // Maximum steering force
    float maxSpeed; // Maximum velocity

public:
    Robot(float x, float y);

    Vector2D getPosition();
    Vector2D getVelocity();
    float getRotation();

    void setPosition(float x, float y);
    void setVelocity(float x, float y);
    void setAcceleration(float x, float y);
    void setRotation(float theta);
    void applyForce(Vector2D& F);
};