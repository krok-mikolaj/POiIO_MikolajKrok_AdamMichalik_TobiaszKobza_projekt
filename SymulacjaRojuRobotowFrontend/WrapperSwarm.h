#pragma once
#include "Swarm.h"
#include "PatternFormation.h"

using namespace System;
using namespace System::Collections::Generic;

public ref class WrapperSwarm
{
    Swarm* swarm;

public:
    // Używaj tych stałych żeby wybrać algorytm (odpowiadają PatternType w backendzie)
    static const int PATTERN_NONE = 0;  // swobodny flocking (ławica)
    static const int PATTERN_CIRCLE = 1;  // formacja okrąg
    static const int PATTERN_SQUARE = 2;  // formacja kwadrat
    static const int PATTERN_HEX_GRID = 3;  // formacja siatka hex

    WrapperSwarm(float x_map, float y_map, unsigned int num_r);
    ~WrapperSwarm();

    void addRobot();
    void removeRobot();
    void update(double dt);

    List<Tuple<float, float>^>^ getSwarmPositions();

    // Pattern API
    void setPattern(int patternType, float spacing);
    void setPattern(int patternType);   // spacing = 60.0f
    void clearPattern();
    int  getActivePattern();
};
