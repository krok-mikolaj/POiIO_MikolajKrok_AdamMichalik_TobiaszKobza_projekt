#pragma once
#include "Swarm.h"

using namespace System;
using namespace System::Collections::Generic;

public ref class WrapperSwarm
{
    Swarm* swarm;

public:
    WrapperSwarm(float x_map, float y_map, unsigned int num_r);
    ~WrapperSwarm();

    void addRobot();
    void removeRobot();
    void update(double dt);

    List<Tuple<float, float>^>^ getSwarmPositions();
	List<float>^ getSwarmRotations();

    void setMapSize(float x, float y);

    void addRobotAt(float x, float y);
    void removeNearestRobot(float x, float y);
};

