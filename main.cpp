#include "simulator.h"
#include "renderer.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <chrono>

int main(int argc, char * argv[])
{
	int steps=1000000;
	int numPoints=12;
	double movingDistance = 0.01;
#ifdef sphere
	double radiusIncrement=1./(double)steps;
#endif
#ifdef torus
	double radiusIncrement=3./(double)steps;
#endif



	//std::cout << "Arguments are:" << std::endl;
	
	for(int i=0; i<argc; i++){
		//std::cout << i << " " << argv[i] << std::endl;
	}

	for(int i=0; i<argc; i++){
		if(i==1) steps = atoi(argv[i]);
		if(i==2) numPoints = atoi(argv[i]);
		if(i==3) radiusIncrement = atof(argv[i]);
		if(i==4) movingDistance = atof(argv[i]);
	}

	//std::cout << "steps: " << steps << std::endl;
	//std::cout << "points: " << numPoints << std::endl;
	//std::cout << "radInc: " << radiusIncrement << std::endl;

	//std::cout << "deleting data/*" << std::endl;
	//system("rm data/*");

	std::chrono::time_point<std::chrono::system_clock> start, end;
	start = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds;

	Simulator simulator(numPoints,radiusIncrement);
                    //2147483647
	for(int i=0; i<steps; i++){
		if(i%(steps/1000)==0){
			end = std::chrono::system_clock::now();
			elapsed_seconds = end-start;
			std::cout << elapsed_seconds.count() << " " << i << " " << std::fixed << std::setprecision(19) << 2*simulator.radius << std::endl;
			//simulator.saveCoordsToFileOpengl(i/(steps/1000));
		}

		simulator.movePoint(movingDistance);
		simulator.increaseRadius(radiusIncrement);
	}

	
	
	return 0;
}
