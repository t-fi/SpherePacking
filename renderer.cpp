#include "renderer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

Renderer::Renderer(){
#ifdef torus
	asprintf(&plotCommand, "splot (3+cos(v))*cos(u), (3+cos(v))*sin(u), sin(v)");
#endif
#ifdef sphere
	asprintf(&plotCommand, "splot cos(v)*cos(u), cos(v)*sin(u), sin(v)");
#endif
	this->numImages = 0;
}

void Renderer::createImage(std::vector<Point> points, double radius){
	numImages++;

	std::vector<const char *> setupCommands = {
		"set term pngcairo",
		"set view equal xyz",
		"set hidden3d",
		"set parametric",
#ifdef torus
		"set urange [0:2*pi]"  ,
		"set vrange [-pi:pi]",
#endif
#ifdef sphere
		"set urange [0:2*pi]"  ,        
		"set vrange [-pi/2.0:pi/2.0]",		
#endif
		"set isosamples 20,20",
		"unset key",
		//"set output 'plot\\image%05d'," + std::to_string(numImages) + ")"
	};

	FILE * gnuplotPipe = popen("gnuplot -p", "w");

	for (auto &command: setupCommands){
		fprintf(gnuplotPipe, "%s \n", command);
	}
	
	//printf("set output 'plot\\image%05d'\n",numImages);
	fprintf(gnuplotPipe, "set output 'plot/image%05d.png'\n",numImages);

	char * dummy = "";

	for (auto &point: points){
		asprintf(&dummy, "%s, %f + %f*cos(v)*cos(u), %f + %f*cos(v)*sin(u), %f + %f*sin(v)", dummy, point.cart.x, radius, point.cart.y, radius, point.cart.z,radius);
	}

	//printf("%s %s \n", plotCommand, dummy);

	fprintf(gnuplotPipe, "%s %s \n", plotCommand, dummy);
}

void Renderer::createPlot(std::vector<Point> points, double radius){

	std::vector<const char *> setupCommands = {
		"reset",
		"set term wxt",
		"set view equal xyz",
		"set hidden3d",
		"set parametric",
#ifdef torus
		"set urange [0:2*pi]"  ,        
		"set vrange [-pi:pi]",
#endif
#ifdef sphere
		"set urange [0:2*pi]"  ,        
		"set vrange [-pi/2.0:pi/2.0]",		
#endif
		"set isosamples 20,20",
		"unset key"
	};

	FILE * gnuplotPipe = popen("gnuplot -p", "w");

	for (auto &command: setupCommands){
		fprintf(gnuplotPipe, "%s \n", command);
		//std::cout << command << std::endl;
	}

	char * dummy = "";

	for (auto &point: points){
		asprintf(&dummy, "%s, %f + %f*cos(v)*cos(u), %f + %f*cos(v)*sin(u), %f + %f*sin(v)", dummy, point.cart.x, radius, point.cart.y, radius, point.cart.z,radius);
	}

	//printf("%s %s \n", plotCommand, dummy);

	fprintf(gnuplotPipe, "%s %s \n", plotCommand, dummy);
}

void Renderer::createVideo(){
	system("ffmpeg -y -framerate 60 -i plot/image%05d.png -c:v libx264 -vf ""fps=60,format=yuv420p"" plot/out.mp4");
}
