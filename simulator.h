#include "point.h"

#include <vector>
#include <random>

#ifndef SIMULATOR_H
#define SIMULATOR_H

class Simulator{
	public:

	std::mt19937_64 mt_rand;
	std::uniform_real_distribution<double> uniformDist;
	std::uniform_int_distribution<int> intDist;
	
	std::vector<Point> points;
	double radius;

	void (Simulator::*transformCoordinates)(Point*);
	
	Simulator(int, double);
	void movePoint(double);
	void transformCoordinatesSpherical(Point*);
	void transformCoordinatesToroidal(Point*);
	bool hasCollision();
	bool hasCollisionSingle(Point*);
	void saveCoordsToFile(int);
	void saveCoordsToFileOpengl(int);
	double probability();
};

#endif

