#include "WrapperSwarm.h"

// ─────────────────────────────────────────────────────────────────────────────
// Constructor / Destructor
// ─────────────────────────────────────────────────────────────────────────────

WrapperSwarm::WrapperSwarm(float x_map, float y_map, unsigned int num_r)
{
    swarm = new Swarm(x_map, y_map, num_r);
}

WrapperSwarm::~WrapperSwarm()
{
    delete swarm;
    swarm = nullptr;
}

// ─────────────────────────────────────────────────────────────────────────────
// Swarm management
// ─────────────────────────────────────────────────────────────────────────────

void WrapperSwarm::addRobot()    { swarm->addRobot(); }
void WrapperSwarm::removeRobot() { swarm->removeRobot(); }
void WrapperSwarm::update(double dt) { swarm->update_dt(dt); }

// ─────────────────────────────────────────────────────────────────────────────
// Robot data for rendering
// ─────────────────────────────────────────────────────────────────────────────

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

// ─────────────────────────────────────────────────────────────────────────────
// Obstacle API
// ─────────────────────────────────────────────────────────────────────────────

void WrapperSwarm::addObstacle(float x, float y, float radius)
{
    swarm->addObstacle(x, y, radius);
}

void WrapperSwarm::removeObstacle(int index)
{
    swarm->removeObstacle(index);
}

void WrapperSwarm::clearObstacles()
{
    swarm->clearObstacles();
}

List<Tuple<float, float, float>^>^ WrapperSwarm::getObstacles()
{
    List<Tuple<float, float, float>^>^ list = gcnew List<Tuple<float, float, float>^>();
    const std::vector<Obstacle>& obs = swarm->getObstacles();
    for (const Obstacle& o : obs)
        list->Add(Tuple::Create(o.position.x, o.position.y, o.radius));
    return list;
}

// ─────────────────────────────────────────────────────────────────────────────
// Target API
// ─────────────────────────────────────────────────────────────────────────────

void WrapperSwarm::setTarget(float x, float y)
{
    swarm->setTarget(x, y);
}

void WrapperSwarm::clearTarget()
{
    swarm->clearTarget();
}

bool WrapperSwarm::hasTarget()
{
    return swarm->getHasTarget();
}

Tuple<float, float>^ WrapperSwarm::getTarget()
{
    Vector2D t = swarm->getTarget();
    return Tuple::Create(t.x, t.y);
}

// ─────────────────────────────────────────────────────────────────────────────
// Optional weight tuning
// ─────────────────────────────────────────────────────────────────────────────

void WrapperSwarm::setObstacleWeight(float w) { swarm->setObstacleWeight(w); }
void WrapperSwarm::setTargetWeight  (float w) { swarm->setTargetWeight(w);   }
