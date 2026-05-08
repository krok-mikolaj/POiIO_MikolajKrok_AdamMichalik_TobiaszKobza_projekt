#include "Vector2D.h"
#include "Robot.h"
#include <iostream>
#include <vector>

using namespace std;

int main()
    {
        Robot Robot1(0.0, 0.0, 0.0, 0.2, 0.3 );
        Robot1.constVel(0.1);
        return 0;
    }