#include "renderer.h"
#include "simulator.h"

#include <iostream>
int main()
{
	Renderer renderer;
	Simulator simulator(100,0.1);

	renderer.createPlot(simulator.points, simulator.radius);

	for(int i=0;i<10000;i++){
		simulator.movePoints(0.01);
		simulator.radius += 0.0001;
		if(simulator.hasCollision()) simulator.radius -= 0.0001;
	}

	renderer.createPlot(simulator.points, simulator.radius);	
	
	return 0;
}
