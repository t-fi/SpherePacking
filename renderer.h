#include "point.h"

#include <vector>

#ifndef RENDERER_H
#define RENDERER_H

class Renderer{
	public:

	int numImages;
	char * plotCommand;
	
	Renderer();	
	void createPlot(std::vector<Point>, double);
	void createImage(std::vector<Point>, double);
	void createVideo();
};

#endif
