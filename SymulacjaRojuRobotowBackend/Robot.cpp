#include "pch.h"
#include "Robot.h"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>

std::vector<Robot*>* robots_pnt = nullptr;

Robot::Robot(double x=50, double y=50, double theta=0, double vx=0, double vy=0)
{
    position[0] = x;
    position[1] = y;
    rotation = theta;
    velocity[0] = vx;
    velocity[1] = vy;
}

double Robot::getXPosition()
{
    return position[0];
}

double Robot::getYPosition()
{
    return position[1];
}

double Robot::getRotation()
{
    return rotation;
}

void Robot::setPosition(double x, double y)
{
    position[0] = x;
    position[1] = y;
}

void Robot::setRotation(double theta)
{
    rotation = theta;
}

void Robot::constVel(double dt)
{
    while (true) {
        position[0] += velocity[0]*dt;
        position[1] += velocity[1]*dt;
        std::this_thread::sleep_for(std::chrono::duration<double>(dt));
        std::cout << "x = " << getXPosition();
        std::cout << " y = " << getYPosition();
        std::cout << " theta = " << getRotation() << "\n";
    }
}

void Robot::updatePos(double dt)
{
    position[0] += velocity[0] * dt;
    position[1] += velocity[1] * dt;
}