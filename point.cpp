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
#ifdef torus
	double cosTheta = 2 + cos(sph.theta);
	
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

