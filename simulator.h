#include "point.h"

#include <vector>
#include <random>
#include <chrono>

#ifndef SIMULATOR_H
#define SIMULATOR_H

class Simulator{
	public:

	std::mt19937_64 mt_rand;
	std::uniform_real_distribution<double> uniformDist;
	std::exponential_distribution<double> expDist;
	std::normal_distribution<double> normDist;
	std::uniform_int_distribution<int> intDist;

	std::chrono::time_point<std::chrono::system_clock> startTime;
	std::chrono::duration<double> elapsed_seconds;


	std::vector<Point> points;
	double radius;
	int seed;

	void (Simulator::*transformCoordinates)(Point*);

	Simulator(int, double, double, int);
	bool movePoint();
	bool increaseRadius();
	void transformCoordinatesSpherical(Point*);
	void transformCoordinatesToroidal(Point*);
	int countNeighbours(Point*);
	bool hasCollision();
	bool hasCollisionSingle(Point*);
	void saveCoordsToFile(std::string);
	void saveCoordsToFileOpengl(std::string);
	void saveCoordsToFileOpenglColourTouch(std::string);
	void saveCoordsToFileQhull(std::string);
	void saveObservablesToFile(std::string);
	void saveTimestamp(std::string);
	void saveFiles(int);
	double discPackingDensity();
	double MCpackingDensity(long);
	double elapsedTime();
	void printReport();
};

#endif
