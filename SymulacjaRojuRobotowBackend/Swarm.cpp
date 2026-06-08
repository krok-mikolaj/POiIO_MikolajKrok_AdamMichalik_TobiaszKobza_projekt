#include "pch.h"
#include "Swarm.h"
#include <random>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

static std::mt19937 rng(std::random_device{}());

// ============================================================
//  Construction / destruction
// ============================================================

Swarm::Swarm(float x_map, float y_map, unsigned int num_r)
    : x_map(x_map), y_map(y_map)
{
    uniform_real_distribution<float> width(0, x_map);
    uniform_real_distribution<float> height(0, y_map);

    for (unsigned int i = 0; i < num_r; ++i) {
        swarm.push_back(new Robot(width(rng), height(rng)));
    }
}

Swarm::~Swarm()
{
    for (Robot* robot : swarm) delete robot;
    swarm.clear();
}

const vector<Robot*>& Swarm::getSwarm() { return swarm; }

// ============================================================
//  Add / remove
// ============================================================

void Swarm::addRobot()
{
    uniform_real_distribution<float> width(0, x_map);
    uniform_real_distribution<float> height(0, y_map);
    swarm.push_back(new Robot(width(rng), height(rng)));

    // Re-assign slots so the new robot gets a target too.
    if (activePattern != PatternType::NONE) {
        setPattern(activePattern); // regenerates slots for new count
    }
}

void Swarm::removeRobot()
{
    if (swarm.empty()) return;
    uniform_int_distribution<unsigned int> idx(0, (unsigned int)swarm.size() - 1);
    unsigned int i = idx(rng);
    delete swarm[i];
    swarm.erase(swarm.begin() + i);

    if (activePattern != PatternType::NONE) {
        setPattern(activePattern);
    }
}

// ============================================================
//  Pattern API
// ============================================================

void Swarm::setPattern(PatternType type, float spacing)
{
    activePattern = type;
    Vector2D center(x_map * 0.5f, y_map * 0.5f);
    slots = PatternFormation::generateSlots(type, center, (int)swarm.size(), spacing);
    assignSlots();
}

void Swarm::clearPattern()
{
    activePattern = PatternType::NONE;
    slots.clear();
    assignment.clear();
}

// Greedy closest-slot assignment.
// For each slot we pick the nearest robot that hasn't been assigned yet.
// O(n^2) — fine for typical swarm sizes (< 1000 robots).
void Swarm::assignSlots()
{
    int n = (int)swarm.size();
    assignment.assign(n, -1);

    vector<bool> slotTaken(slots.size(), false);

    for (int si = 0; si < (int)slots.size() && si < n; ++si) {
        float bestDist = std::numeric_limits<float>::max();
        int   bestRobot = -1;

        for (int ri = 0; ri < n; ++ri) {
            if (assignment[ri] != -1) continue; // already assigned
            float d = swarm[ri]->position.distanceTo(slots[si]);
            if (d < bestDist) {
                bestDist = d;
                bestRobot = ri;
            }
        }

        if (bestRobot != -1) {
            assignment[bestRobot] = si;
            slotTaken[si] = true;
        }
    }
}

// Seek steering force: steer smoothly towards target position.
Vector2D Swarm::seekSlot(Robot* robot, const Vector2D& target)
{
    Vector2D desired = target - robot->position;
    float dist = desired.mag();
    if (dist < 1.0f) return Vector2D(); // already there

    // Scale max speed down when very close (arrival behaviour)
    float speed = robot->maxSpeed;
    const float arrivalRadius = 40.0f;
    if (dist < arrivalRadius) {
        speed *= dist / arrivalRadius;
    }

    Vector2D t_vel = desired.normalized() * speed;
    Vector2D steerF = (t_vel - robot->velocity) * robot->mass;
    steerF.limit(robot->maxF);
    return steerF;
}

// ============================================================
//  Flocking
// ============================================================

Vector2D Swarm::cohesion(Robot* robot, vector<Robot*>& swarm)
{
    if (swarm.empty()) return Vector2D();

    Vector2D S_pos;
    unsigned int n = 0;

    for (Robot* bot : swarm) {
        if ((robot != bot) && (robot->position.distanceTo(bot->position) < robot->percepR)) {
            S_pos = S_pos + bot->position;
            n++;
        }
    }
    if (n == 0) return Vector2D();

    Vector2D avgPos = S_pos * (1.0f / n);
    Vector2D t_vel = (avgPos - robot->position).normalized() * robot->maxSpeed;
    Vector2D steerF = (t_vel - robot->velocity) * robot->mass;
    steerF.limit(robot->maxF);
    return steerF;
}

Vector2D Swarm::seperation(Robot* robot, vector<Robot*>& swarm)
{
    if (swarm.empty()) return Vector2D();

    Vector2D S_pos;
    unsigned int n = 0;

    for (Robot* bot : swarm) {
        float d = robot->position.distanceTo(bot->position);
        if ((robot != bot) && (d < robot->percepR)) {
            Vector2D dir = robot->position - bot->position;
            S_pos = S_pos + dir.normalized() * (1.0f / d);
            n++;
        }
    }
    if (n == 0) return Vector2D();

    Vector2D t_vel = (S_pos * (1.0f / n)).normalized() * robot->maxSpeed;
    Vector2D steerF = (t_vel - robot->velocity) * robot->mass;
    steerF.limit(robot->maxF);
    return steerF;
}

Vector2D Swarm::alignment(Robot* robot, vector<Robot*>& swarm)
{
    if (swarm.empty()) return Vector2D();

    Vector2D S_vel;
    unsigned int n = 0;

    for (Robot* bot : swarm) {
        if ((robot != bot) && (robot->position.distanceTo(bot->position) < robot->percepR)) {
            S_vel = S_vel + bot->velocity;
            n++;
        }
    }
    if (n == 0) return Vector2D();

    Vector2D t_vel = (S_vel * (1.0f / n)).normalized() * robot->maxSpeed;
    Vector2D steerF = (t_vel - robot->velocity) * robot->mass;
    steerF.limit(robot->maxF);
    return steerF;
}

void Swarm::flock(Robot* robot, vector<Robot*>& swarm)
{
    Vector2D steerF;
    steerF = steerF + cohesion(robot, swarm) * coh_w;
    steerF = steerF + seperation(robot, swarm) * sep_w;
    steerF = steerF + alignment(robot, swarm) * align_w;
    robot->applyForce(steerF);
}

// ============================================================
//  Update
// ============================================================

void Swarm::update()
{
    if (activePattern == PatternType::NONE) {
        // Normal flocking
        for (Robot* robot : swarm) flock(robot, swarm);
    }
    else {
        // Pattern formation: each robot seeks its assigned slot
        for (int i = 0; i < (int)swarm.size(); ++i) {
            if (assignment[i] < 0 || assignment[i] >= (int)slots.size()) continue;
            Vector2D f = seekSlot(swarm[i], slots[assignment[i]]) * formationForceWeight;
            swarm[i]->applyForce(f);
        }
    }

    for (Robot* robot : swarm) {
        robot->velocity = robot->velocity + robot->acceleration;
        robot->velocity.limit(robot->maxSpeed);
        robot->position = robot->position + robot->velocity;
        robot->acceleration = Vector2D();
        wrapEdges(robot);
    }
}

void Swarm::update_dt(double dt)
{
    if (activePattern == PatternType::NONE) {
        // Normal flocking
        for (Robot* robot : swarm) flock(robot, swarm);
    }
    else {
        // Pattern formation: each robot seeks its assigned slot, flocking disabled
        for (int i = 0; i < (int)swarm.size(); ++i) {
            if (assignment[i] < 0 || assignment[i] >= (int)slots.size()) continue;
            Vector2D f = seekSlot(swarm[i], slots[assignment[i]]) * formationForceWeight;
            swarm[i]->applyForce(f);
        }
    }

    for (Robot* robot : swarm) {
        robot->velocity = robot->velocity + robot->acceleration * (float)dt;
        robot->velocity.limit(robot->maxSpeed);
        robot->position = robot->position + robot->velocity * (float)dt;
        robot->acceleration = Vector2D();
        wrapEdges(robot);
    }
}

void Swarm::wrapEdges(Robot* robot)
{
	if (robot->position.x > x_map) robot->position.x = 0;
	if (robot->position.x < 0) robot->position.x = x_map;
	if (robot->position.y > y_map) robot->position.y = 0;
	if (robot->position.y < 0) robot->position.y = y_map;
}

void Swarm::addRobotAt(float x, float y)
{
	Robot* Bot = new Robot(x, y);
	swarm.push_back(Bot);
}

void Swarm::removeNearestRobot(float x, float y)
{
	if (swarm.empty()) return;

	int nearestIdx = 0;
	float minDist = FLT_MAX;

	for (int i = 0; i < swarm.size(); i++)
	{
		float dx = swarm[i]->position.x - x;
		float dy = swarm[i]->position.y - y;
		float dist = sqrt(dx * dx + dy * dy);
		if (dist < minDist)
		{
			minDist = dist;
			nearestIdx = i;
		}
	}

	delete swarm[nearestIdx];
	swarm.erase(swarm.begin() + nearestIdx);
}
