#pragma once
#include <vector>


class Robot
{
private:
    double position[2]; // [1]: x coordinate, [2]: y coordinate
    double velocity[2]; // [1]: x velocity, [2]: y velocity
    double rotation;    // theta rotation

public:
    Robot(double x, double y, double theta, double vx, double vy);

    double getXPosition();
    double getYPosition();
    double getRotation();

    void setPosition(double x, double y);
    void setRotation(double theta);

    void constVel(double dt);
    void updatePos(double dt);
};

extern std::vector<Robot*>* robots_pnt;