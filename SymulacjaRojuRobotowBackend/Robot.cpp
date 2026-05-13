#include "pch.h"
#include "Robot.h"
#include <vector>
#include <iostream>
#include <random>

Robot::Robot(float x, float y)
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(-2.0f, 2.0f);

    setPosition(x, y);
    setVelocity(dist(rng), dist(rng));
    setAcceleration(0.0f, 0.0f);
    setRotation(40.0f);

    maxSpeed = 3.0f;
    maxF = 0.1f;
    percepR = 50.0f;
}

Vector2D Robot::getPosition()
{
    return position;
}

float Robot::getRotation()
{
    return rotation;
}

void Robot::setPosition(float x, float y)
{
    position.x = x;
    position.y = y;
}

void Robot::setVelocity(float x, float y)
{
    velocity.x = x;
    velocity.y = y;
}

void Robot::setAcceleration(float x, float y)
{
    acceleration.x = x;
    acceleration.y = y;
}

void Robot::setRotation(float theta)
{
    rotation = theta;
}


