#include "simulator.h"
#include "renderer.h"
#include <iostream>
#include <limits>
int main(int argc, char * argv[])
{
	int steps=1000000;
	int numPoints=12;
#ifdef sphere
	double radiusIncrement=1./(double)steps;
#endif
#ifdef torus
	double radiusIncrement=3./(double)steps;
#endif
	std::cout << "Arguments are:" << std::endl;
	
	for(int i=0; i<argc; i++){
		std::cout << i << " " << argv[i] << std::endl;
	}

	for(int i=0; i<argc; i++){
		if(i==1) steps = atoi(argv[i]);
		if(i==2) numPoints = atoi(argv[i]);
		if(i==3) radiusIncrement = atof(argv[i]);
	}

	std::cout << "steps: " << steps << std::endl;
	std::cout << "points: " << numPoints << std::endl;
	std::cout << "radInc: " << radiusIncrement << std::endl;

	std::cout << "deleting data/*" << std::endl;
	system("rm data/*");

	Simulator simulator(numPoints,radiusIncrement);
                    //2147483647
	for(int i=0; i<steps; i++){
		if(i%(steps/1000)==0) simulator.saveCoordsToFile(i/(steps/1000));

		simulator.movePoint(0.1);
		simulator.radius += radiusIncrement;
		if(simulator.hasCollision()) simulator.radius -= radiusIncrement;
	}

	return 0;
}
