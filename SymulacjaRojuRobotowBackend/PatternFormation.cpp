#include "pch.h"
#include "PatternFormation.h"
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

std::vector<Vector2D> PatternFormation::generateSlots(
    PatternType type,
    Vector2D center,
    int numRobots,
    float spacing)
{
    switch (type) {
    case PatternType::CIRCLE:   return circleSlots(center, numRobots, spacing);
    case PatternType::SQUARE:   return squareSlots(center, numRobots, spacing);
    case PatternType::HEX_GRID: return hexGridSlots(center, numRobots, spacing);
    default:                    return {};
    }
}

// ---------- CIRCLE ----------
// Robots evenly distributed on a single circle.
// Radius is chosen so the arc spacing between neighbors equals `spacing`.
std::vector<Vector2D> PatternFormation::circleSlots(Vector2D center, int n, float spacing)
{
    std::vector<Vector2D> slots;
    if (n <= 0) return slots;

    // r * 2*PI / n = spacing  =>  r = n*spacing / (2*PI)
    // Minimum radius guard so it never collapses to a point.
    float radius = (n > 1) ? (n * spacing) / (2.0f * (float)M_PI) : spacing;

    for (int i = 0; i < n; ++i) {
        float angle = (2.0f * (float)M_PI * i) / n;
        slots.push_back(Vector2D(
            center.x + radius * std::cos(angle),
            center.y + radius * std::sin(angle)
        ));
    }
    return slots;
}

// ---------- SQUARE GRID ----------
// Fills a square grid row by row. Grid side = ceil(sqrt(n)).
// Excess slots (bottom-right corner) are still generated so every
// robot always gets a unique target; the last few slots just extend
// the last row.
std::vector<Vector2D> PatternFormation::squareSlots(Vector2D center, int n, float spacing)
{
    std::vector<Vector2D> slots;
    if (n <= 0) return slots;

    int side = (int)std::ceil(std::sqrt((float)n));

    // Offset so the grid is centred on `center`
    float offsetX = -(side - 1) * spacing * 0.5f;
    float offsetY = -(side - 1) * spacing * 0.5f;

    for (int i = 0; i < n; ++i) {
        int row = i / side;
        int col = i % side;
        slots.push_back(Vector2D(
            center.x + offsetX + col * spacing,
            center.y + offsetY + row * spacing
        ));
    }
    return slots;
}

// ---------- HEX GRID ----------
// Classic offset hex grid (flat-topped rows, every odd row shifted by
// spacing/2 horizontally).
// Horizontal step  = spacing
// Vertical step    = spacing * sqrt(3)/2
std::vector<Vector2D> PatternFormation::hexGridSlots(Vector2D center, int n, float spacing)
{
    std::vector<Vector2D> slots;
    if (n <= 0) return slots;

    const float hStep = spacing;                         // horizontal
    const float vStep = spacing * std::sqrt(3.0f) / 2.0f; // vertical

    int cols = (int)std::ceil(std::sqrt((float)n));
    int rows = (int)std::ceil((float)n / cols);

    float offsetX = -(cols - 1) * hStep * 0.5f;
    float offsetY = -(rows - 1) * vStep * 0.5f;

    for (int r = 0; r < rows && (int)slots.size() < n; ++r) {
        float rowShift = (r % 2 == 1) ? hStep * 0.5f : 0.0f;
        for (int c = 0; c < cols && (int)slots.size() < n; ++c) {
            slots.push_back(Vector2D(
                center.x + offsetX + rowShift + c * hStep,
                center.y + offsetY + r * vStep
            ));
        }
    }
    return slots;
}
