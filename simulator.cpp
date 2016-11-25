#include "simulator.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cmath>

#define PI 3.14159265359

Simulator::Simulator(int numPoints, double lambda, double sigma, int seed){
	radius = 0.0;

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
	ss << std::setw(6) << std::setfill('0') << i;	
	
	std::ofstream myfile;
	myfile.open ("data/coordinates"+ss.str()+".dat", std::ios::trunc);
	
	myfile << points.size() << std::endl;
	myfile << "-2 2" << std::endl;
	myfile << "-2 2" << std::endl;
	myfile << "-2 2" << std::endl;
	for(auto &point: points){
		myfile << std::fixed << std::setprecision(7) << point.cart.x << " " << point.cart.y << " " << point.cart.z << " " << 2*radius << "\n";
	}
	myfile.close();
}

bool Simulator::hasCollisionSingle(Point * refPoint){
	for(auto &point: points){
		if(point.collidesWith(refPoint, radius)) return true;
	}
	return false;
}

double Simulator::packingDensity(){
#ifdef sphere
	double totalArea = 4*PI;
#endif
#ifdef torus
	double totalArea = 2*PI*PI*3;
#endif
	double particleArea = radius*radius*PI*points.size();
	return particleArea/totalArea;

}
