#ifndef POINT_H
#define POINT_H
struct Coord3D{
	double x, y, z;
};

struct Coord2D{
	double phi, theta;
};


class Point{
	public:
	
	Coord3D cart;
	Coord2D sph;
	
	Point(double, double);
	void move(double, double);
	bool collidesWith(Point*, double);
	void transformCoordinates();
};
#endif
