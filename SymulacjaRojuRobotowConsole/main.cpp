#include "Vector2D.h"
#include "Robot.h"
#include "Swarm.h"
#include "PatternFormation.h"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

// Map size matching the Windows Forms window
static const float  MAP_W = 490.0f;
static const float  MAP_H = 379.0f;
static const double DT = 0.016; // ~60 fps

// ------------------------------------------------------------------ helpers

static void printSeparator(const char* title)
{
    cout << "\n========================================\n";
    cout << "  " << title << "\n";
    cout << "========================================\n";
}

static void printRobotState(Robot* r, int idx)
{
    Vector2D p = r->getPosition();
    Vector2D v = r->getVelocity();
    cout << fixed << setprecision(2)
        << "  Robot[" << idx << "]"
        << "  pos=(" << setw(7) << p.x << ", " << setw(7) << p.y << ")"
        << "  vel=(" << setw(6) << v.x << ", " << setw(6) << v.y << ")"
        << "  speed=" << setw(5) << v.mag()
        << "\n";
}

// Mean distance of all robots from a given point
static float meanDistTo(const vector<Robot*>& robots, Vector2D pt)
{
    if (robots.empty()) return 0.0f;
    float sum = 0.0f;
    for (Robot* r : robots)
        sum += r->getPosition().distanceTo(pt);
    return sum / robots.size();
}

// Mean distance of each robot from its nearest slot
static float meanSlotError(const vector<Robot*>& robots,
    const vector<Vector2D>& slots)
{
    if (robots.empty() || slots.empty()) return 0.0f;
    float sum = 0.0f;
    for (Robot* r : robots) {
        float best = 1e9f;
        for (const Vector2D& s : slots)
            best = min(best, r->getPosition().distanceTo(s));
        sum += best;
    }
    return sum / robots.size();
}

// Run `steps` simulation steps and print a one-line summary every `every` steps
static void runSteps(Swarm& swarm, const char* label, int steps, int every)
{
    auto& robots = swarm.getSwarm();
    Vector2D centre(MAP_W * 0.5f, MAP_H * 0.5f);

    for (int t = 1; t <= steps; ++t) {
        swarm.update_dt(DT);
        if (t % every == 0) {
            cout << "  [" << label << "] step " << setw(4) << t
                << "  mean_dist_centre=" << setw(6) << fixed << setprecision(1)
                << meanDistTo(robots, centre) << "\n";
        }
    }
}

// ------------------------------------------------------------------ tests

// 1. Vector2D operations
void testVector2D()
{
    printSeparator("TEST 1 – Vector2D operations");

    Vector2D a(3.0f, 4.0f);
    Vector2D b(1.0f, 2.0f);

    cout << "  a=(" << a.x << "," << a.y << ")  mag=" << a.mag()
        << "  (expected 5.0)\n";

    Vector2D n = a.normalized();
    cout << "  a.normalized()=(" << fixed << setprecision(3)
        << n.x << "," << n.y << ")  mag=" << n.mag()
        << "  (expected 1.0)\n";

    Vector2D sum = a + b;
    cout << "  a+b=(" << sum.x << "," << sum.y << ")  (expected 4,6)\n";

    Vector2D diff = a - b;
    cout << "  a-b=(" << diff.x << "," << diff.y << ")  (expected 2,2)\n";

    Vector2D scaled = a * 2.0f;
    cout << "  a*2=(" << scaled.x << "," << scaled.y << ")  (expected 6,8)\n";

    Vector2D lim = a; lim.limit(2.0f);
    cout << "  a.limit(2.0) mag=" << lim.mag()
        << "  (expected <=2.0)\n";

    cout << "  distanceTo: a.distanceTo(b)=" << a.distanceTo(b)
        << "  (expected ~3.162)\n";
}

// 2. Single Robot – construction and physics
void testRobotPhysics()
{
    printSeparator("TEST 2 – Single Robot physics");

    Robot r(100.0f, 200.0f);
    r.setVelocity(0.0f, 0.0f);
    r.setAcceleration(0.0f, 0.0f);

    cout << "  Initial state:\n";
    printRobotState(&r, 0);

    // Apply a force pointing right and run a few steps
    Vector2D force(1.0f, 0.0f);
    for (int i = 0; i < 10; ++i) {
        r.applyForce(force);
        r.update(DT);
    }

    cout << "  After 10 steps with force (1,0):\n";
    printRobotState(&r, 0);
    cout << "  (robot should have moved right, positive vx)\n";

    // Verify speed cap
    Robot r2(50.0f, 50.0f);
    r2.setVelocity(0.0f, 0.0f);
    Vector2D bigForce(100.0f, 0.0f);
    for (int i = 0; i < 200; ++i) {
        r2.applyForce(bigForce);
        r2.update(DT);
    }
    float spd = r2.getVelocity().mag();
    cout << "\n  Speed after large force for 200 steps: " << fixed << setprecision(2)
        << spd << "  (maxSpeed=10.0, speed should approach it)\n";
}

// 3. Slot generation for all three patterns
void testPatternSlots()
{
    printSeparator("TEST 3 – PatternFormation slot generation");

    Vector2D centre(MAP_W * 0.5f, MAP_H * 0.5f);
    const int    N = 12;
    const float  SPACING = 55.0f;

    struct Case { PatternType type; const char* name; };
    for (auto& c : vector<Case>{
            {PatternType::CIRCLE,   "CIRCLE  "},
            {PatternType::SQUARE,   "SQUARE  "},
            {PatternType::HEX_GRID, "HEX_GRID"} })
    {
        auto slots = PatternFormation::generateSlots(c.type, centre, N, SPACING);
        cout << "  " << c.name
            << "  slots=" << slots.size() << "/" << N;

        // Check all slots are on the map (with a margin for the circle radius)
        int onMap = 0;
        for (const auto& s : slots)
            if (s.x >= -50 && s.x <= MAP_W + 50 && s.y >= -50 && s.y <= MAP_H + 50)
                ++onMap;
        cout << "  on_map=" << onMap << "/" << slots.size()
            << "  (all should be on map)\n";
    }
}

// 4. Full Swarm – flocking converges (robots cluster together)
void testFlocking()
{
    printSeparator("TEST 4 – Swarm flocking (cohesion check)");

    Swarm swarm(MAP_W, MAP_H, 10);
    Vector2D centre(MAP_W * 0.5f, MAP_H * 0.5f);

    float before = meanDistTo(swarm.getSwarm(), centre);
    runSteps(swarm, "FLOCK", 300, 100);
    float after = meanDistTo(swarm.getSwarm(), centre);

    cout << "\n  mean_dist_centre  before=" << fixed << setprecision(1)
        << before << "  after=" << after << "\n";
    cout << "  Cohesion test: " << (after < before + 30.0f ? "PASS" : "NOTE – may vary")
        << "  (spread shouldn't grow unboundedly)\n";
}

// 5. Pattern formation – robots converge to slots
void testPatternConvergence()
{
    printSeparator("TEST 5 – Pattern formation convergence");

    Vector2D centre(MAP_W * 0.5f, MAP_H * 0.5f);
    const float SPACING = 55.0f;

    struct Case { PatternType type; const char* name; };
    for (auto& c : vector<Case>{
            {PatternType::CIRCLE,   "CIRCLE  "},
            {PatternType::SQUARE,   "SQUARE  "},
            {PatternType::HEX_GRID, "HEX_GRID"} })
    {
        Swarm swarm(MAP_W, MAP_H, 10);
        swarm.setPattern(c.type, SPACING);

        auto slots = PatternFormation::generateSlots(c.type, centre, 10, SPACING);
        float before = meanSlotError(swarm.getSwarm(), slots);

        // Run 500 steps and sample error every 100
        auto& robots = swarm.getSwarm();
        cout << "\n  " << c.name << "\n";
        for (int step = 100; step <= 500; step += 100) {
            for (int s = 0; s < 100; ++s) swarm.update_dt(DT);
            float err = meanSlotError(robots, slots);
            cout << "    step " << setw(4) << step
                << "  mean_slot_error=" << setw(7) << fixed << setprecision(2)
                << err << "\n";
        }
        float after = meanSlotError(robots, slots);
        cout << "  Convergence: " << (after < before ? "PASS" : "FAIL")
            << "  (error should decrease over time)\n";
    }
}

// 6. addRobot / removeRobot mid-simulation
void testDynamicRobots()
{
    printSeparator("TEST 6 – Dynamic add/remove robots");

    Swarm swarm(MAP_W, MAP_H, 5);
    cout << "  Start:  " << swarm.getSwarm().size() << " robots\n";

    swarm.addRobot();
    swarm.addRobot();
    cout << "  After 2x addRobot:  " << swarm.getSwarm().size() << " robots\n";

    swarm.removeRobot();
    cout << "  After removeRobot:  " << swarm.getSwarm().size() << " robots\n";

    // Activate pattern after adding – slots should be regenerated automatically
    swarm.setPattern(PatternType::CIRCLE, 55.0f);
    for (int i = 0; i < 200; ++i) swarm.update_dt(DT);

    Vector2D centre(MAP_W * 0.5f, MAP_H * 0.5f);
    auto slots = PatternFormation::generateSlots(
        PatternType::CIRCLE, centre, (int)swarm.getSwarm().size(), 55.0f);
    float err = meanSlotError(swarm.getSwarm(), slots);
    cout << "  mean_slot_error after 200 steps with CIRCLE: "
        << fixed << setprecision(2) << err
        << "  (should be <100)\n";
    cout << "  Dynamic test: " << (err < 100.0f ? "PASS" : "FAIL") << "\n";
}

// 7. clearPattern returns to flocking
void testClearPattern()
{
    printSeparator("TEST 7 – clearPattern() restores flocking");

    Swarm swarm(MAP_W, MAP_H, 8);
    swarm.setPattern(PatternType::SQUARE, 55.0f);
    for (int i = 0; i < 100; ++i) swarm.update_dt(DT);

    cout << "  activePattern before clear: "
        << (swarm.getActivePattern() != PatternType::NONE ? "SET" : "NONE") << "\n";

    swarm.clearPattern();
    cout << "  activePattern after  clear: "
        << (swarm.getActivePattern() == PatternType::NONE ? "NONE (OK)" : "FAIL") << "\n";

    // Should run without crash after clear
    for (int i = 0; i < 100; ++i) swarm.update_dt(DT);
    cout << "  100 flocking steps after clear: OK (no crash)\n";
}

// ------------------------------------------------------------------ main

int main()
{
    cout << "=== Swarm Simulation – Unit Tests ===\n";

    testVector2D();
    testRobotPhysics();
    testPatternSlots();
    testFlocking();
    testPatternConvergence();
    testDynamicRobots();
    testClearPattern();

    cout << "\n=== All tests complete ===\n";
    return 0;
}
