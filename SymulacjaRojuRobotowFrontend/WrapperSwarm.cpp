#include "pch.h"
#include "WrapperSwarm.h"

WrapperSwarm::WrapperSwarm(float x_map, float y_map, unsigned int num_r)
{
    swarm = new Swarm(x_map, y_map, num_r);
}

WrapperSwarm::~WrapperSwarm()
{
    delete swarm;
    swarm = nullptr;
}

void WrapperSwarm::addRobot()
{
    swarm->addRobot();
}

void WrapperSwarm::removeRobot()
{
    swarm->removeRobot();
}

void WrapperSwarm::update(double dt)
{
    swarm->update_dt(dt);
}

List<Tuple<float, float>^>^ WrapperSwarm::getSwarmPositions()
{
    List<Tuple<float, float>^>^ list = gcnew List<Tuple<float, float>^>();

    const std::vector<Robot*>& robots = swarm->getSwarm();
    for (Robot* robot : robots)
    {
        Vector2D pos = robot->getPosition();
        list->Add(Tuple::Create(pos.x, pos.y));
    }

    return list;
}

List<float>^ WrapperSwarm::getSwarmRotations()
{
    List<float>^ result = gcnew List<float>();
    for (int i = 0; i < swarm->getRobotsCount(); i++)
        result->Add(swarm->getRobot(i)->getRotation());
    return result;
}

void WrapperSwarm::setMapSize(float x, float y)
{
    swarm->setMapSize(x, y);
}

void WrapperSwarm::addRobotAt(float x, float y)
{
    swarm->addRobotAt(x, y);
}

void WrapperSwarm::removeNearestRobot(float x, float y)
{
    swarm->removeNearestRobot(x, y);
}

// ------------------------------------------------------------------ Pattern API

void WrapperSwarm::setPattern(int patternType, float spacing)
{
    swarm->setPattern(static_cast<PatternType>(patternType), spacing);
}

void WrapperSwarm::setPattern(int patternType)
{
    swarm->setPattern(static_cast<PatternType>(patternType));
}

void WrapperSwarm::clearPattern()
{
    swarm->clearPattern();
}

int WrapperSwarm::getActivePattern()
{
    return static_cast<int>(swarm->getActivePattern());
}

