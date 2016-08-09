#include "renderer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

Renderer::Renderer(){
	//asprintf(&plotCommand, "splot (3+cos(v))*cos(u), (3+cos(v))*sin(u), sin(v)");
	asprintf(&plotCommand, "splot cos(v)*cos(u), cos(v)*sin(u), sin(v)");
}

void Renderer::createPlot(std::vector<Point> points, double radius){
	std::vector<char const *> setupCommands = {
		"set term wxt",
		"set view equal xyz",
		"set hidden3d",
		"set parametric",
		"set urange [0:2*pi]"  ,        
		"set vrange [-pi/2.0:pi/2.0]",
		"set isosamples 20,20",
		"unset key"
	};

	FILE * gnuplotPipe = popen("gnuplot -p", "w");

	for (auto &command: setupCommands){
		fprintf(gnuplotPipe, "%s \n", command);
	}
	char * dummy;

	for (auto &point: points){
		asprintf(&dummy, ", %f + %f*cos(v)*cos(u), %f + %f*cos(v)*sin(u), %f + %f*sin(v)",
			point.cart.x, radius, point.cart.y, radius, point.cart.z,radius);
		asprintf(&plotCommand, "%s %s", plotCommand, dummy);
	}

	fprintf(gnuplotPipe, "%s \n", plotCommand);
}

void Renderer::createVideo(){
	//TODO: create more plots, save them as picture and then create video via ffmpeg
}
