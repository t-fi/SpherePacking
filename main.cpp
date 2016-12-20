#include "simulator.h"
#include "renderer.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <chrono>
#include <cmath>

#define PI 3.14159265359

int main(int argc, char * argv[])
{
	// argument parsing
	int numPoints, seed;
	long int steps;
	double lambda, sigma;

	// reading arguments
	for(int i=0; i<argc; i++){
		if(i==1) numPoints = atoi(argv[i]);
		if(i==2) steps     = atol(argv[i]);
		if(i==3) lambda    = atof(argv[i]);
		if(i==4) sigma     = atof(argv[i]);
		if(i==5) seed      = atof(argv[i]);
	}

	// setting defaults for unspecified args
	for(int i=5; i>=argc; i--){
		if(i==1) numPoints = 12;
		if(i==2) steps     = 1000000;
		if(i==3) lambda    = 1.;
		if(i==4) sigma     = 1.;
		if(i==5) seed      = 0;
	}

	// error handling
	if(numPoints <= 0  ||
	   steps     <= 0  ||
     lambda    <= 0. ||
	   sigma     <= 0.)
	{
		std::cout << "An argument was invalid! Program terminated." << std::endl;
		return 1;
	}

	// printing arguments
	std::cout << '#' << " numPoints = " << numPoints << std::endl;
	std::cout << '#' << " steps     = " << steps     << std::endl;
	std::cout << '#' << " lambda    = " << lambda    << std::endl;
	std::cout << '#' << " sigma     = " << sigma     << std::endl;
	std::cout << '#' << " seed      = " << seed      << std::endl;

	// timekeeping
	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds;

	// starting simulator
	Simulator simulator(numPoints, lambda, sigma, seed);

	// main loop
	for(long int i=0; i<steps; i++){

		// log every 1000th step
		if(i%(steps/1000)==0){
			elapsed_seconds = std::chrono::system_clock::now()-start;
			std::cout << elapsed_seconds.count() << " " << i << " " << std::fixed << std::setprecision(19) << 2*simulator.radius << std::endl;
		}

		simulator.increaseRadius();
		simulator.movePoint();
	}

  simulator.saveCoordsToFileQhull(0);

	double mcDensity = simulator.MCpackingDensity();

	elapsed_seconds = std::chrono::system_clock::now()-start;

	std::cerr << numPoints << " simple disc packing density: " << std::fixed << std::setprecision(19) << simulator.packingDensity() << std::endl;
	std::cerr << numPoints << " MC packing density:          " << std::fixed << std::setprecision(19) << mcDensity << std::endl;
	#ifdef sphere
		double exactPackingDensity = numPoints*2*PI*(1-simulator.radius*simulator.radius)*(1-sqrt(1-simulator.radius*simulator.radius))/(4*PI*(1-simulator.radius*simulator.radius));
		std::cerr << std::fixed << std::setprecision(19) << "Packing density with correction factor:       " << exactPackingDensity << std::endl;
	#endif

	std::cerr << std::fixed << std::setprecision(19) << elapsed_seconds.count() << " Final Radius for " << numPoints << " Particles: " << simulator.radius << " Diameter: " << 2*simulator.radius << std::endl;

	return 0;
}
