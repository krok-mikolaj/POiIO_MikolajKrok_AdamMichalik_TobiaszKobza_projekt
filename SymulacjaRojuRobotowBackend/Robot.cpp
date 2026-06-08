#include "pch.h"
#include "Robot.h"
#include <random>

using namespace std;

Robot::Robot(float x, float y)
{
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(-2.0f, 2.0f);

    setPosition(x, y);
    setVelocity(dist(rng), dist(rng));
    setAcceleration(0.0f, 0.0f);
    setRotation(40.0f);

    maxSpeed = 10.0f; //zmieniono z 3.0f
    maxF = 3.0f; //zmieniono z 0.1f
    percepR = 100.0f; //zmieniono z 150.f
}

Vector2D Robot::getPosition()
{
    return position;
}

Vector2D Robot::getVelocity()
{
    return velocity;
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

void Robot::applyForce(Vector2D& F)
{
    F.limit(maxF);
    acceleration.x += F.x / mass;
    acceleration.y += F.y / mass;
}

Vector2D Robot::getAcceleration()
{
    return acceleration;
}

void Robot::update(double dt)
{
    velocity =
        velocity +
        acceleration * dt;

    velocity.limit(maxSpeed);

    position =
        position +
        velocity * dt;

    acceleration = Vector2D();
}