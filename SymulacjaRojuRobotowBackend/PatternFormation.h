#pragma once
#include "Vector2D.h"
#include <vector>

enum class PatternType {
    NONE,
    CIRCLE,
    SQUARE,
    HEX_GRID
};

class PatternFormation {
public:
    // Generates target slot positions for a given pattern.
    // center     - center of the formation on the map
    // numRobots  - how many slots to generate (one per robot)
    // spacing    - distance between adjacent slots / radius unit
    static std::vector<Vector2D> generateSlots(
        PatternType type,
        Vector2D center,
        int numRobots,
        float spacing
    );

private:
    static std::vector<Vector2D> circleSlots(Vector2D center, int n, float spacing);
    static std::vector<Vector2D> squareSlots(Vector2D center, int n, float spacing);
    static std::vector<Vector2D> hexGridSlots(Vector2D center, int n, float spacing);
};
