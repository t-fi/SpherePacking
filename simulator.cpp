#include "simulator.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

Simulator::Simulator(int numPoints, double _radius){
	radius = _radius;

	mt_rand.seed(0);
	uniformDist = std::uniform_real_distribution<double>(0.,6.28318530718);
	intDist = std::uniform_int_distribution<int>(0,numPoints-1);

	for(int i = 0; i < numPoints; i++){
		points.emplace_back(uniformDist(mt_rand), uniformDist(mt_rand));
		while(hasCollisionSingle(&points.back())){
			points.back() = Point(uniformDist(mt_rand), uniformDist(mt_rand));
		}
	}
	
	uniformDist = std::uniform_real_distribution<double>(0.,1.);	
}

void Simulator::movePoint(double d){
	double dPhi, dTheta;

	int rand = intDist(mt_rand);

	double oPhi = points[rand].sph.phi;
	double oTheta = points[rand].sph.theta;


	dPhi = d*(.5-uniformDist(mt_rand));
	dTheta = d*(.5-uniformDist(mt_rand));
	points[rand].move(dPhi,dTheta);
	
	if(!hasCollisionSingle(&points[rand])){
#ifdef sphere
		double probability = std::abs(sin(points[rand].sph.theta)/sin(oTheta));
#endif
#ifdef torus
		double probability = std::abs((3+sin(points[rand].sph.theta))/(3+sin(oTheta)));
#endif
		if(probability>uniformDist(mt_rand))
			return;
	}	
	points[rand].sph.phi = oPhi;
	points[rand].sph.theta = oTheta;
	points[rand].transformCoordinates();
}

void Simulator::increaseRadius(double d){
	double oldRadius = radius;
	radius += d*uniformDist(mt_rand);
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
