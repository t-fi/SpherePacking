#include "point.h"

#include <vector>
#include <random>

#ifndef SIMULATOR_H
#define SIMULATOR_H

class Simulator{
	public:

	std::mt19937_64 mt_rand;
	std::uniform_real_distribution<double> uniformDist;
	
	std::vector<Point> points;
	double radius;

	void (Simulator::*transformCoordinates)(Point*);
	
	Simulator(int, double);
	void movePoints(double);
	void transformCoordinatesSpherical(Point*);
	void transformCoordinatesToroidal(Point*);
	bool hasCollision();
	bool hasCollisionSingle(Point*);
	void saveCoordsToFile();
};

#endif

