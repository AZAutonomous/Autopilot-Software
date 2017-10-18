#ifndef OBSTACLE_H
#define OBSTACLE_H
#include "coordinate.h"

class Obstacle {
private:
	Coordinate _loc;
	double _r;

public:
	Obstacle(Coordinate loc, double r);

	Coordinate getLocation();
	double getRadius();

	void setLocation(Coordinate loc);
	void setRadius(double radius);
};

#endif