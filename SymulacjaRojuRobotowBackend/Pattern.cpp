#include "pch.h"
#include "Pattern.h"

#include <cmath>
#include <limits>

Pattern::Pattern(
    PatternType type,
    float cx,
    float cy)
{
    patternType = type;

    centerX = cx;
    centerY = cy;
}

void Pattern::createCircle(
    unsigned int n)
{
    targets.clear();

    float radius = 200.0f;

    for (unsigned int i = 0;i < n;i++)
    {
        float angle =
            2.0f *
            3.14159265f *
            i / n;

        targets.push_back(
            Vector2D(
                centerX +
                radius * cos(angle),

                centerY +
                radius * sin(angle)
            )
        );
    }
}

void Pattern::createSquare(
    unsigned int n)
{
    targets.clear();

    float side = 400.0f;

    unsigned int edgeCount =
        std::max(
            1u,
            n / 4);

    for (unsigned int i = 0;i < edgeCount;i++)
    {
        float t =
            (float)i /
            std::max(
                1u,
                edgeCount - 1);

        targets.push_back(
            Vector2D(
                centerX -
                side / 2 +
                t * side,

                centerY -
                side / 2
            )
        );

        targets.push_back(
            Vector2D(
                centerX +
                side / 2,

                centerY -
                side / 2 +
                t * side
            )
        );

        targets.push_back(
            Vector2D(
                centerX +
                side / 2 -
                t * side,

                centerY +
                side / 2
            )
        );

        targets.push_back(
            Vector2D(
                centerX -
                side / 2,

                centerY +
                side / 2 -
                t * side
            )
        );
    }
}

void Pattern::createGrid(
    unsigned int n)
{
    targets.clear();

    int cols =
        (int)ceil(
            sqrt((float)n));

    float spacing = 70.0f;

    for (unsigned int i = 0;i < n;i++)
    {
        int row = i / cols;
        int col = i % cols;

        targets.push_back(
            Vector2D(
                centerX +
                (col - cols / 2)
                * spacing,

                centerY +
                (row - cols / 2)
                * spacing
            )
        );
    }
}

void Pattern::generateTargets(
    unsigned int robotCount)
{
    switch (patternType)
    {
    case PatternType::Circle:

        createCircle(
            robotCount);

        break;

    case PatternType::Square:

        createSquare(
            robotCount);

        break;

    case PatternType::Grid:

        createGrid(
            robotCount);

        break;
    }
}

int Pattern::findNearestFreeTarget(
    Robot* robot,
    const std::vector<Robot*>& swarm)
{
    float bestDistance =
        std::numeric_limits<float>::max();

    int bestIndex = -1;

    Vector2D robotPos =
        robot->getPosition();

    for (int i = 0;
        i < (int)targets.size();
        i++)
    {
        bool occupied = false;

        for (Robot* other : swarm)
        {
            if (other == robot)
                continue;

            if (other
                ->getPosition()
                .distanceTo(
                    targets[i])
                <
                occupationRadius)
            {
                occupied = true;
                break;
            }
        }

        if (occupied)
            continue;

        float d =
            robotPos.distanceTo(
                targets[i]);

        if (d < bestDistance)
        {
            bestDistance = d;
            bestIndex = i;
        }
    }

    return bestIndex;
}

void Pattern::update(
    const std::vector<Robot*>& swarm,
    double dt)
{
    for (Robot* robot : swarm)
    {
        int idx =
            findNearestFreeTarget(
                robot,
                swarm);

        if (idx < 0)
            continue;

        Vector2D desired =
            targets[idx]
            -
            robot->getPosition();

        desired =
            desired.normalized();

        robot->applyForce(
            desired);
    }

    for (Robot* robot : swarm)
    {
        robot->update(dt);
    }
}