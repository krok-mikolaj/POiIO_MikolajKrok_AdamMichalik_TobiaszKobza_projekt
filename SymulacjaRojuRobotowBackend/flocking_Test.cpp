#include <iostream>

#include "Swarm.h"

using namespace std;

int main()
{
	Swarm Roj(500, 500, 10);
	int i = 0;
	int j = 1;
	Vector2D P, V, S_P;

	while (i <= 200)
	{
		Roj.update();
		cout << "Tick #" << i << endl;
		i++;

		for (Robot* bot : Roj.getSwarm())
		{
			P = bot->getPosition();
			V = bot->getVelocity();
			S_P = S_P + P; 
			cout << "Robot " << j << " | Position [ " << P.x << ", " << P.y << " ] | Velocity [ " << V.x << ", " << V.y << " ]\n";
			j++;
		}
		S_P = S_P * (1.0f/j);
		cout << "AvgPosition [ " << S_P.x << ", " << S_P.y << " ]\n";
		j = 1;
		cout << endl << endl;
	}

	return 0;
}