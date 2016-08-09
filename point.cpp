#include "point.h"

#include <math.h>

Point::Point(double phi, double theta){
	sph.phi = phi;
	sph.theta = theta;
	transformCoordinates();
}
void Point::move(double dPhi, double dTheta){
	sph.phi+=dPhi;
	sph.theta+=dTheta;
	transformCoordinates();
}

bool Point::collidesWith(Point * point, double radius){
	if(point==this) return false;

	double dx = cart.x - point->cart.x;
	double dy = cart.y - point->cart.y;
	double dz = cart.z - point->cart.z;
	double d = sqrt(dx*dx+dy*dy+dz*dz);

	return d<2*radius;
}

void Point::transformCoordinates(){
	double sinPhi = sin(sph.phi);
	
	cart.x = sinPhi*cos(sph.theta);
	cart.y = sinPhi*sin(sph.theta);
	cart.z = cos(sph.phi);
}

/*
void TorusPoint::transformCoordinates(){
	double sinPhi = 3+sin(sph.phi);
	
	cart.x = sinPhi*cos(sph.theta);
	cart.y = sinPhi*sin(sph.theta);
	cart.z = cos(sph.phi);
}*/
