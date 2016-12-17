#include "point.h"

#include <vector>
#include <random>

#ifndef SIMULATOR_H
#define SIMULATOR_H

class Simulator{
	public:

	std::mt19937_64 mt_rand;
	std::uniform_real_distribution<double> uniformDist;
	std::exponential_distribution<double> expDist;
	std::normal_distribution<double> normDist;
	std::uniform_int_distribution<int> intDist;

	std::vector<Point> points;
	double radius;
	int seed;

	void (Simulator::*transformCoordinates)(Point*);

	Simulator(int, double, double, int);
	void movePoint();
	void increaseRadius();
	void transformCoordinatesSpherical(Point*);
	void transformCoordinatesToroidal(Point*);
	int countNeighbours(Point*);
	bool hasCollision();
	bool hasCollisionSingle(Point*);
	void saveCoordsToFile(int);
	void saveCoordsToFileOpengl(int);
	void saveCoordsToFileQhull(int);
	double packingDensity();
	double MCpackingDensity();
};

#endif
