#include "pch.h"
#include "Swarm.h"
#include <random>
#include <cmath>
#include <algorithm>

using namespace std;

static std::mt19937 rng(std::random_device{}());

// ─────────────────────────────────────────────────────────────────────────────
// Constructor / Destructor
// ─────────────────────────────────────────────────────────────────────────────

Swarm::Swarm(float x_map, float y_map, unsigned int num_r)
{
    this->x_map = x_map;
    this->y_map = y_map;

    std::uniform_real_distribution<float> width(0, x_map);
    std::uniform_real_distribution<float> height(0, y_map);

    for (unsigned int i = 0; i < num_r; i++)
    {
        Robot* bot = new Robot(width(rng), height(rng));
        swarm.push_back(bot);
    }
}

Swarm::~Swarm()
{
    for (Robot* robot : swarm)
        delete robot;
    swarm.clear();
}

// ─────────────────────────────────────────────────────────────────────────────
// Swarm management
// ─────────────────────────────────────────────────────────────────────────────

const vector<Robot*>& Swarm::getSwarm()
{
    return swarm;
}

void Swarm::addRobot()
{
    std::uniform_real_distribution<float> width(0, x_map);
    std::uniform_real_distribution<float> height(0, y_map);
    swarm.push_back(new Robot(width(rng), height(rng)));
}

void Swarm::removeRobot()
{
    if (swarm.empty()) return;
    std::uniform_int_distribution<unsigned int> l(0, (unsigned int)swarm.size() - 1);
    unsigned int idx = l(rng);
    delete swarm[idx];
    swarm.erase(swarm.begin() + idx);
}

// ─────────────────────────────────────────────────────────────────────────────
// Obstacle API
// ─────────────────────────────────────────────────────────────────────────────

void Swarm::addObstacle(float x, float y, float radius)
{
    obstacles.emplace_back(x, y, radius);
}

void Swarm::removeObstacle(int index)
{
    if (index < 0 || index >= (int)obstacles.size()) return;
    obstacles.erase(obstacles.begin() + index);
}

void Swarm::clearObstacles()
{
    obstacles.clear();
}

// ─────────────────────────────────────────────────────────────────────────────
// Target API
// ─────────────────────────────────────────────────────────────────────────────

void Swarm::setTarget(float x, float y)
{
    targetPoint = Vector2D(x, y);
    hasTarget = true;
}

void Swarm::clearTarget()
{
    hasTarget = false;
}

// ─────────────────────────────────────────────────────────────────────────────
// Flocking behaviours
// ─────────────────────────────────────────────────────────────────────────────

Vector2D Swarm::cohesion(Robot* robot, const vector<Robot*>& swarm)
{
    if (swarm.empty()) return Vector2D();

    Vector2D sumPos;
    unsigned int n = 0;

    for (Robot* bot : swarm)
    {
        if (bot != robot && robot->position.distanceTo(bot->position) < robot->percepR)
        {
            sumPos = sumPos + bot->position;
            n++;
        }
    }
    if (n == 0) return Vector2D();

    Vector2D avgPos = sumPos * (1.0f / n);
    Vector2D desired = (avgPos - robot->position).normalized() * robot->maxSpeed;
    Vector2D steerF = (desired - robot->velocity) * robot->mass;
    steerF.limit(robot->maxF);
    return steerF;
}

Vector2D Swarm::seperation(Robot* robot, const vector<Robot*>& swarm)
{
    if (swarm.empty()) return Vector2D();

    Vector2D sumForce;
    unsigned int n = 0;

    for (Robot* bot : swarm)
    {
        float d = robot->position.distanceTo(bot->position);
        if (bot != robot && d < robot->percepR)
        {
            Vector2D away = (robot->position - bot->position).normalized() * (1.0f / std::max(d, 0.1f));
            sumForce = sumForce + away;
            n++;
        }
    }
    if (n == 0) return Vector2D();

    Vector2D desired = (sumForce * (1.0f / n)).normalized() * robot->maxSpeed;
    Vector2D steerF = (desired - robot->velocity) * robot->mass;
    steerF.limit(robot->maxF);
    return steerF;
}

Vector2D Swarm::alignment(Robot* robot, const vector<Robot*>& swarm)
{
    if (swarm.empty()) return Vector2D();

    Vector2D sumVel;
    unsigned int n = 0;

    for (Robot* bot : swarm)
    {
        if (bot != robot && robot->position.distanceTo(bot->position) < robot->percepR)
        {
            sumVel = sumVel + bot->velocity;
            n++;
        }
    }
    if (n == 0) return Vector2D();

    Vector2D desired = (sumVel * (1.0f / n)).normalized() * robot->maxSpeed;
    Vector2D steerF = (desired - robot->velocity) * robot->mass;
    steerF.limit(robot->maxF);
    return steerF;
}

// ─────────────────────────────────────────────────────────────────────────────
// Obstacle avoidance
// ─────────────────────────────────────────────────────────────────────────────

Vector2D Swarm::obstacleAvoidance(Robot* robot, const vector<Obstacle>& obs)
{
    if (obs.empty()) return Vector2D();

    Vector2D sumForce;
    bool hit = false;

    for (const Obstacle& o : obs)
    {
        float d = robot->position.distanceTo(o.position);

        // Detection range: obstacle surface + half perception radius
        float detectionRange = o.radius + robot->percepR * 0.5f;

        if (d < detectionRange)
        {
            // Distance from obstacle surface (clamped to avoid div/0)
            float surfaceDist = std::max(d - o.radius, 0.1f);

            // Force grows strongly as robot approaches surface
            Vector2D away = (robot->position - o.position).normalized();
            float    strength = (detectionRange / surfaceDist) * robot->maxSpeed;

            sumForce = sumForce + away * strength;
            hit = true;
        }
    }

    if (!hit) return Vector2D();

    Vector2D desired = sumForce.normalized() * robot->maxSpeed;
    Vector2D steerF = (desired - robot->velocity) * robot->mass;
    steerF.limit(robot->maxF);
    return steerF;
}

// ─────────────────────────────────────────────────────────────────────────────
// Target seek (arrival behaviour — slows down near target)
// ─────────────────────────────────────────────────────────────────────────────

Vector2D Swarm::seekTarget(Robot* robot, const Vector2D& target)
{
    Vector2D toTarget = target - robot->position;
    float d = toTarget.mag();

    if (d < 1.0f) return Vector2D(); // already at target

    // Slow down inside slowRadius (arrival)
    float speed = (d < slowRadius) ? robot->maxSpeed * (d / slowRadius) : robot->maxSpeed;
    Vector2D desired = toTarget.normalized() * speed;
    Vector2D steerF = (desired - robot->velocity) * robot->mass;
    steerF.limit(robot->maxF);
    return steerF;
}

// ─────────────────────────────────────────────────────────────────────────────
// flock() — combines all active steering forces
// ─────────────────────────────────────────────────────────────────────────────

void Swarm::flock(Robot* robot, const vector<Robot*>& swarm)
{
    // --- Flocking + target (capped together by applyForce / maxF) ---
    Vector2D flockTotal;
    flockTotal = flockTotal + cohesion(robot, swarm) * coh_w;
    flockTotal = flockTotal + seperation(robot, swarm) * sep_w;
    flockTotal = flockTotal + alignment(robot, swarm) * align_w;

    // Global target seek — skip when pattern formation is active.
    // NOTE FOR MERGE: wrap this block with:
    //   if (hasTarget && activePattern == PatternType::NONE)
    if (hasTarget)
        flockTotal = flockTotal + seekTarget(robot, targetPoint) * target_w;

    robot->applyForce(flockTotal); // internally limited to robot->maxF

    // --- Obstacle avoidance applied SEPARATELY ---
    // Must NOT be mixed into the same applyForce call: when combined with
    // flocking forces, the shared maxF cap dilutes the avoidance force and
    // robots pass through obstacles.
    if (!obstacles.empty())
    {
        Vector2D obsF = obstacleAvoidance(robot, obstacles) * obs_w;
        // Cap at obs_w * maxF so it can always overpower flocking
        obsF.limit(robot->maxF * obs_w);
        // Write directly to acceleration (Swarm is friend of Robot)
        robot->acceleration.x += obsF.x / robot->mass;
        robot->acceleration.y += obsF.y / robot->mass;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Update loops
// ─────────────────────────────────────────────────────────────────────────────

void Swarm::update()
{
    for (Robot* robot : swarm)
        flock(robot, swarm);

    for (Robot* robot : swarm)
    {
        robot->velocity = robot->velocity + robot->acceleration;
        robot->velocity.limit(robot->maxSpeed);
        robot->position = robot->position + robot->velocity;
        robot->acceleration = Vector2D();
        wrapEdges(robot);
    }
}

void Swarm::update_dt(double dt)
{
    for (Robot* robot : swarm)
        flock(robot, swarm);

    for (Robot* robot : swarm)
    {
        robot->velocity = robot->velocity + robot->acceleration * (float)dt;
        robot->velocity.limit(robot->maxSpeed);
        robot->position = robot->position + robot->velocity * (float)dt;
        robot->acceleration = Vector2D();
        robot->updateRotation();

        // Hard boundary: if a robot is inside an obstacle (can happen at high
        // speed), push it back to the surface and cancel inward velocity.
        for (const Obstacle& o : obstacles)
        {
            float d = robot->position.distanceTo(o.position);
            float minDist = o.radius + 4.0f; // 4px robot margin

            if (d < minDist && d > 0.01f)
            {
                // Displace robot to obstacle surface
                Vector2D away = (robot->position - o.position).normalized();
                robot->position.x = o.position.x + away.x * minDist;
                robot->position.y = o.position.y + away.y * minDist;

                // Cancel the velocity component pointing into the obstacle
                float dot = robot->velocity.x * away.x + robot->velocity.y * away.y;
                if (dot < 0)
                {
                    robot->velocity.x -= dot * away.x;
                    robot->velocity.y -= dot * away.y;
                }
            }
        }

        wrapEdges(robot);
    }
}

void Swarm::wrapEdges(Robot* robot)
{
    if (robot->position.x > x_map) robot->position.x = 0;
    if (robot->position.x < 0)     robot->position.x = x_map;
    if (robot->position.y > y_map) robot->position.y = 0;
    if (robot->position.y < 0)     robot->position.y = y_map;
}
