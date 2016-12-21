#include "point.h"

#include <math.h>
#include <cmath>

Point::Point(double phi, double theta){
	sph.phi = phi;
	sph.theta = theta;
	transformCoordinates();
}
void Point::move(double dPhi, double dTheta){
	sph.phi += dPhi;
	sph.theta += dTheta;

	//make sure point does not wander outside the usual coordinate space
	//that wouldn't really matter though....

	sph.phi = fmod(sph.phi,2*M_PI)+(sph.phi < 0)*2*M_PI;
#ifdef torus
	sph.theta = fmod(sph.theta,2*M_PI)+(sph.theta < 0)*2*M_PI;
#endif
#ifdef sphere
	sph.theta = fmod(sph.theta,M_PI)+(sph.theta < 0)*M_PI;
#endif
	transformCoordinates();
}

bool Point::collidesWith(Point * point, double radius){

	double diameter = 2*radius;
	if(point==this) return false;

	double dx = cart.x - point->cart.x;
	if(std::abs(dx)>diameter) return false;
	double dy = cart.y - point->cart.y;
	if(std::abs(dy)>diameter) return false;
	double dz = cart.z - point->cart.z;
	if(std::abs(dz)>diameter) return false;
	double d = dx*dx+dy*dy+dz*dz;

	return d<diameter*diameter;
}

double Point::getDistance(Point * point){
	if(point==this) return 0.0;

	double dx = cart.x - point->cart.x;
	double dy = cart.y - point->cart.y;
	double dz = cart.z - point->cart.z;
	return sqrt(dx*dx+dy*dy+dz*dz);
}

void Point::transformCoordinates(){
#ifdef torus
	double cosTheta = 3 + cos(sph.theta);

	cart.x = cosTheta*cos(sph.phi);
	cart.y = cosTheta*sin(sph.phi);
	cart.z = sin(sph.theta);
#endif
#ifdef sphere
	double sinTheta = sin(sph.theta);

	cart.x = sinTheta*cos(sph.phi);
	cart.y = sinTheta*sin(sph.phi);
	cart.z = cos(sph.theta);
#endif
}
