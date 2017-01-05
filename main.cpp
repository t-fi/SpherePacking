#include "simulator.h"
#include "renderer.h"
#include <iostream>
#include <limits>
#include <iomanip>
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

	// starting simulator
	Simulator simulator(numPoints, lambda, sigma, seed);

	//succes rate (SR) counters
	double percSRradius = 0.0;
	double percSRposition = 0.0;
	long int SRradius = 0;
	long int SRposition = 0;
	long int trials = 0;

	// main loop
	for(long int i=0; i<steps; i++){

		// log every 1000th step
		if(i%(steps/1000)==0){
			percSRradius = (double)SRradius/(double)trials;
			percSRposition = (double)SRposition/(double)trials;
			trials = 0;
			SRradius = 0;
			SRposition = 0;
			std::cerr << simulator.elapsedTime() << " " << i << " " << std::fixed << std::setprecision(19) << 2*simulator.radius << " " << std::fixed << std::setprecision(10) << percSRradius << " " << percSRposition << std::endl;
		}

		trials++;
		SRradius += simulator.increaseRadius();
		SRposition += simulator.movePoint();
	}

	simulator.saveFiles(0);

	return 0;
}
