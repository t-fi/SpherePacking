#include "simulator.h"

#include <iostream>

Simulator::Simulator(int numPoints, double _radius){
	radius = _radius;

	mt_rand.seed(0);
	uniformDist = std::uniform_real_distribution<double>(0.,6.28318530718);

	for(int i = 0; i < numPoints; i++){
		points.emplace_back(uniformDist(mt_rand), uniformDist(mt_rand));
		while(hasCollisionSingle(&points.back())){
			points.back() = Point(uniformDist(mt_rand), uniformDist(mt_rand));
		}
	}
	
	uniformDist = std::uniform_real_distribution<double>(0.,1.);	
}

void Simulator::movePoints(double d){
	double dPhi, dTheta;
	for(auto &point: points){
		int tries = 1000000;

		double oPhi = point.sph.phi;
		double oTheta = point.sph.theta;
		do{	
			point.sph.phi = oPhi;
			point.sph.theta = oTheta;

			if(tries--==0) break;

			dPhi = d*(.5-uniformDist(mt_rand));
			dTheta = d*(.5-uniformDist(mt_rand));
			point.move(dPhi,dTheta);
		} while (hasCollisionSingle(&point));
	}
}

bool Simulator::hasCollision(){
	for(int i = 0; i < points.size(); i++){
		for(int j = i + 1; j < points.size(); j++){
			if(points[i].collidesWith(&points[j], radius)) return true;
		}
	}
	return false;
}

bool Simulator::hasCollisionSingle(Point * refPoint){
	for(auto &point: points){
		if(point.collidesWith(refPoint, radius)) return true;
	}
	return false;
}
