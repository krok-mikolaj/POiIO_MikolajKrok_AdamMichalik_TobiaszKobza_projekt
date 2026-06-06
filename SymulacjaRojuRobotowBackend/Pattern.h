#pragma once

#include "Robot.h"
#include "Vector2D.h"

#include <vector>

enum class PatternType
{
    Circle,
    Square,
    Grid
};

class Pattern
{
private:

    std::vector<Vector2D> targets;

    PatternType patternType;

    float centerX;
    float centerY;

    float occupationRadius = 20.0f;

public:

    Pattern(
        PatternType type,
        float cx,
        float cy);

    void generateTargets(
        unsigned int robotCount);

    void update(
        const std::vector<Robot*>& swarm,
        double dt);

private:

    void createCircle(
        unsigned int n);

    void createSquare(
        unsigned int n);

    void createGrid(
        unsigned int n);

    int findNearestFreeTarget(
        Robot* robot,
        const std::vector<Robot*>& swarm);
};