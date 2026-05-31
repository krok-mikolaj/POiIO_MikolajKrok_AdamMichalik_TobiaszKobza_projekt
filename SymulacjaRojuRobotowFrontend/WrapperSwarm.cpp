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