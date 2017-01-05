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
	startTime = std::chrono::system_clock::now();

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

bool Simulator::movePoint(){
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
			return true;
	}
	points[rand].sph.phi = oPhi;
	points[rand].sph.theta = oTheta;
	points[rand].transformCoordinates();
	return false;
}

bool Simulator::increaseRadius(){
	double oldRadius = radius;
	radius += expDist(mt_rand);
	if(this->hasCollision()){
		radius = oldRadius;
		return false;
	}
	return true;
}


bool Simulator::hasCollision(){
	for(int i = 0; i < points.size(); i++){
		for(int j = i + 1; j < points.size(); j++){
			if(points[i].collidesWith(&points[j], radius)) return true;
		}
	}
	return false;
}

void Simulator::saveCoordsToFile(std::string folderPath){
	std::ofstream myfile;
	myfile.open (folderPath+"coordinatesRAW.dat", std::ios::trunc);

	myfile << "#Phi\tTheta\tx\ty\tz\tr\n";
	for(auto &point: points){
		myfile << std::fixed << std::setprecision(5) << point.sph.phi<< "\t" << point.sph.theta << "\t" << point.cart.x << "\t" << point.cart.y << "\t" << point.cart.z << "\t" << radius << "\n";
	}
	myfile.close();
}

void Simulator::saveCoordsToFileOpengl(std::string folderPath){
	std::ofstream myfile;
	myfile.open (folderPath+"coordinatesOGL.dat", std::ios::trunc);

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
		<< " " << 2*radius << "\n";
	}
	myfile.close();
}

void Simulator::saveCoordsToFileOpenglColourTouch(std::string folderPath){
	std::ofstream myfile;
	myfile.open (folderPath+"coordinatesOGLColor.dat", std::ios::trunc);

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

void Simulator::saveCoordsToFileQhull(std::string folderPath){
	std::ofstream myfile;
	myfile.open (folderPath+"coordinatesQhull.dat", std::ios::trunc);

  myfile << 3 << "Diameter=" << 2*radius << std::endl;

	//add artificial point at 0; torus needs more additional points or a different method
	myfile << points.size()+1 << std::endl;
  myfile << std::fixed << std::setprecision(7) << 0.0 << " " << 0.0 << " " << 0.0 << "\n";

	for(auto &point: points){
		myfile << std::fixed << std::setprecision(7) << point.cart.x << " " << point.cart.y << " " << point.cart.z << "\n";
	}
	myfile.close();
}

void Simulator::saveObservablesToFile(std::string folderPath){
	std::ofstream myfile;
	myfile.open (folderPath+"Observables.dat", std::ios::trunc);

	double exactPackingDensity = 0;

	#ifdef sphere
		exactPackingDensity = points.size()*2*PI*(1-radius*radius)*(1-sqrt(1-radius*radius))/(4*PI*(1-radius*radius));
	#endif

  myfile << std::fixed << std::setprecision(19) << "Radius=" << 2*radius << std::endl;
	myfile << std::fixed << std::setprecision(19) << "discPackingDensity=" << discPackingDensity() << std::endl;
	myfile << std::fixed << std::setprecision(19) << "exactPackingDensity=" << exactPackingDensity << std::endl;
	myfile << std::fixed << std::setprecision(19) << "MCPackingDensity=" << MCpackingDensity(100000) << std::endl;
	myfile.close();
}

void Simulator::saveTimestamp(std::string folderPath){
	std::ofstream myfile;
	myfile.open (folderPath+"timestamp.dat", std::ios::trunc);
	myfile << std::fixed << std::setprecision(19) << elapsedTime() << std::endl;
	myfile.close();
}

bool Simulator::hasCollisionSingle(Point * refPoint){
	for(auto &point: points){
		if(point.collidesWith(refPoint, radius)) return true;
	}
	return false;
}

int Simulator::countNeighbours(Point * refPoint){
	int counter = -1; //distance to self is 0

	double epsilon = 2.01 * radius;

	for(auto &point: points){
		if(point.getDistance(refPoint) < epsilon) counter++;
	}
	return counter;
}

double Simulator::discPackingDensity(){
#ifdef sphere
	double totalArea = 4*PI;
#endif
#ifdef torus
	double totalArea = 4*PI*PI*3;
#endif
	double particleArea = radius*radius*PI*points.size();
	return particleArea/totalArea;

}

double Simulator::MCpackingDensity(long totalSamples){

	long particleSamples = 0;

	for(long i = 0; i < totalSamples; i++){

		#ifdef sphere
			double phi = 2*PI*uniformDist(mt_rand);
			double theta = acos(2*uniformDist(mt_rand)-1);
		#endif
		#ifdef torus
			//http://math.stackexchange.com/questions/2017079/uniform-random-points-on-a-torus

			double theta, phi = 2*PI*uniformDist(mt_rand);

			do {
				theta = 2*PI*uniformDist(mt_rand);
			} while(3+cos(theta)/4<uniformDist(mt_rand));

		#endif
		Point * samplePoint = new Point(phi,theta);

		for(auto &point: points){
			if(point.collidesWith(samplePoint, radius/2)){
				particleSamples++; break;
			}
		}
	}

	return (double)particleSamples/(double)totalSamples;
}

void Simulator::printReport(){
	double mcDensity = MCpackingDensity(10000000);

	std::cerr << points.size() << " simple disc packing density: " << std::fixed << std::setprecision(19) << discPackingDensity() << std::endl;
	std::cerr << points.size() << " MC packing density:          " << std::fixed << std::setprecision(19) << mcDensity << std::endl;
	#ifdef sphere
		double exactPackingDensity = points.size()*2*PI*(1-radius*radius)*(1-sqrt(1-radius*radius))/(4*PI*(1-radius*radius));
		std::cerr << std::fixed << std::setprecision(19) << "Packing density with correction factor: " << exactPackingDensity << std::endl;
	#endif

	std::cerr << std::fixed << std::setprecision(19) << "Final Radius for " << points.size() << " Particles: " << radius << " Diameter: " << 2*radius << std::endl;
}

double Simulator::elapsedTime(){
	elapsed_seconds = std::chrono::system_clock::now()-startTime;
	return elapsed_seconds.count();
}

void Simulator::saveFiles(int i){
	std::stringstream folderPathSS;
	#ifdef sphere
	    folderPathSS << "data/sphere/numParticles:" << points.size() << "/seed:" << seed << "/iteration:" <<  std::setw(10) << std::setfill('0') << i << "/";
	#endif
	#ifdef torus
	    folderPathSS << "data/torus/numParticles:" << points.size() << "/seed:" << seed << "/iteration:" <<  std::setw(10) << std::setfill('0') << i << "/";
	#endif

	std::string folderPath = folderPathSS.str();

	std::stringstream mkdirSS;

	mkdirSS << "mkdir -p " << folderPath;

	int dummy = system(mkdirSS.str().c_str());

	saveCoordsToFile(folderPath);
	saveCoordsToFileOpengl(folderPath);
	saveCoordsToFileOpenglColourTouch(folderPath);
	saveCoordsToFileQhull(folderPath);
	saveObservablesToFile(folderPath);
	saveTimestamp(folderPath);
}
