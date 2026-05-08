#pragma once
#include <vector>

class Robot
{
private:
    double position[2]; // [1]: x coordinate, [2]: y coordinate
    double velocity[2]; // [1]: x velocity, [2]: y velocity
    double rotation;    // theta rotation

public:
    Robot(double x = 0.0, double y = 0.0, double theta = 0.0, double vx = 0.0, double vy = 0.0);

    double getXPosition();
    double getYPosition();
    double getRotation();

    void setPosition(double x, double y);
    void setRotation(double theta);

    void constVel(double dt);
};

extern std::vector<Robot*> robots_pnt;