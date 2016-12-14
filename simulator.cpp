#include "simulator.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cmath>

#define PI 3.14159265359

Simulator::Simulator(int numPoints, double lambda, double sigma, int _seed){
	radius = 0.0;
	seed = _seed;

	// initialize the randomizers
	mt_rand.seed(seed);
	expDist = std::exponential_distribution<double>(lambda);
	normDist = std::normal_distribution<double>(0,sigma);
	uniformDist = std::uniform_real_distribution<double>(0.,1.);
	intDist = std::uniform_int_distribution<int>(0,numPoints-1);

	for(int i = 0; i < numPoints; i++){
#ifdef sphere
		points.emplace_back(2*PI*uniformDist(mt_rand), acos(2*uniformDist(mt_rand)-1));
#endif
#ifdef torus
		//http://math.stackexchange.com/questions/2017079/uniform-random-points-on-a-torus
		double theta = 2*PI*uniformDist(mt_rand);
		double phi = 2*PI*uniformDist(mt_rand);
		if(3+cos(theta)/4>=uniformDist(mt_rand))
			points.emplace_back(theta, phi);
		else
			i--;
#endif
	}
}

void Simulator::movePoint(){
	double dPhi, dTheta;

	int rand = intDist(mt_rand);

	double oPhi = points[rand].sph.phi;
	double oTheta = points[rand].sph.theta;


	dPhi = normDist(mt_rand);
	dTheta = normDist(mt_rand);
	points[rand].move(dPhi,dTheta);

	if(!hasCollisionSingle(&points[rand])){
#ifdef sphere
		double probability = std::abs(sin(points[rand].sph.theta)/sin(oTheta));
#endif
#ifdef torus
		double probability = std::abs((3+cos(points[rand].sph.theta))/(3+cos(oTheta)));
#endif
		if(probability>uniformDist(mt_rand))
			return;
	}
	points[rand].sph.phi = oPhi;
	points[rand].sph.theta = oTheta;
	points[rand].transformCoordinates();
}

void Simulator::increaseRadius(){
	double oldRadius = radius;
	radius += expDist(mt_rand);
	if(this->hasCollision()) radius = oldRadius;
}


bool Simulator::hasCollision(){
	for(int i = 0; i < points.size(); i++){
		for(int j = i + 1; j < points.size(); j++){
			if(points[i].collidesWith(&points[j], radius)) return true;
		}
	}
	return false;
}

void Simulator::saveCoordsToFile(int i){
	std::stringstream ss;
	ss << std::setw(6) << std::setfill('0') << i;

	std::ofstream myfile;
	myfile.open ("data/coordinates"+ss.str()+".dat", std::ios::trunc);
	myfile << "#Phi\tTheta\tx\ty\tz\tr\n";
	for(auto &point: points){
		myfile << std::fixed << std::setprecision(5) << point.sph.phi<< "\t" << point.sph.theta << "\t" << point.cart.x << "\t" << point.cart.y << "\t" << point.cart.z << "\t" << radius << "\n";
	}
	myfile.close();
}

void Simulator::saveCoordsToFileOpengl(int i){
	std::stringstream ss;
	#ifdef sphere
	    ss << "_sphere_" << points.size() << "_" << seed << "_" <<  std::setw(6) << std::setfill('0') << i;
	#endif
	#ifdef torus
	    ss << "_torus_" << points.size() << "_" << seed << "_" <<  std::setw(6) << std::setfill('0') << i;
	#endif

	std::ofstream myfile;
	myfile.open ("data/coordinates"+ss.str()+".dat", std::ios::trunc);

	myfile << points.size() << std::endl;
	#ifdef sphere
		myfile << "-2 2" << std::endl;
		myfile << "-2 2" << std::endl;
		myfile << "-2 2" << std::endl;
	#endif
	#ifdef torus
			myfile << "-4 4" << std::endl;
			myfile << "-4 4" << std::endl;
			myfile << "-4 4" << std::endl;
	#endif

	for(auto &point: points){
		myfile << std::fixed << std::setprecision(7) << point.cart.x << " " << point.cart.y << " " << point.cart.z
		<< " " << 2*radius << " " << countNeighbours(&point) << "\n";
	}
	myfile.close();
}

void Simulator::saveCoordsToFileQhull(int i){
	std::stringstream ss;
	#ifdef sphere
	    ss << "_sphere_" << points.size() << "_" << seed << "_" <<  std::setw(6) << std::setfill('0') << i;
	#endif
	#ifdef torus
	    ss << "_torus_" << points.size() << "_" << seed << "_" <<  std::setw(6) << std::setfill('0') << i;
	#endif

	std::ofstream myfile;
	myfile.open ("data/hull"+ss.str()+".dat", std::ios::trunc);

  myfile << 3 << "Diameter=" << 2*radius << std::endl;

	//add artificial point at 0; torus needs more additional points or a different method
	myfile << points.size()+1 << std::endl;
  myfile << std::fixed << std::setprecision(7) << 0.0 << " " << 0.0 << " " << 0.0 << "\n";

	for(auto &point: points){
		myfile << std::fixed << std::setprecision(7) << point.cart.x << " " << point.cart.y << " " << point.cart.z << "\n";
	}
	myfile.close();
}

bool Simulator::hasCollisionSingle(Point * refPoint){
	for(auto &point: points){
		if(point.collidesWith(refPoint, radius)) return true;
	}
	return false;
}

//dummy
int Simulator::countNeighbours(Point * refPoint){
	int counter = -1; //distance to self is 0

	double epsilon = 2 * radius * 1.000001;

	for(auto &point: points){
		if(point.getDistance(refPoint) < epsilon) counter++;
	}
	return counter;
}

double Simulator::packingDensity(){
#ifdef sphere
	double totalArea = 4*PI;
#endif
#ifdef torus
	double totalArea = 4*PI*PI*3;
#endif
	double particleArea = radius*radius*PI*points.size();
	return particleArea/totalArea;

}
