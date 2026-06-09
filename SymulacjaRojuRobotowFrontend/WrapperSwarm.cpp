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

void WrapperSwarm::setBehavior(int behavior)
{
    swarm->setBehavior((SwarmBehavior)behavior);
}

void WrapperSwarm::setCustomFormation(List<Tuple<float, float>^>^ positions)
{
    std::vector<Vector2D> pts;
    for each (Tuple<float, float> ^ p in positions)
        pts.push_back(Vector2D(p->Item1, p->Item2));
    swarm->setCustomFormation(pts);
}


