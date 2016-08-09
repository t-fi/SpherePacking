#include "renderer.h"
#include "simulator.h"

#include <iostream>
int main(int argc, char * argv[])
{
	//Renderer renderer;
	Simulator simulator(6,0.1);

	//renderer.createPlot(simulator.points, simulator.radius);
	//renderer.createImage(simulator.points, simulator.radius);
	for(int i=0;i<10000;i++){
		//if(i%100==0) renderer.createImage(simulator.points, simulator.radius);
		simulator.movePoints(0.01);
		simulator.radius += 0.0001;
		if(simulator.hasCollision()) simulator.radius -= 0.0001;
	}

	simulator.saveCoordsToFile();
	//renderer.createPlot(simulator.points, simulator.radius);	
	
	return 0;
}
