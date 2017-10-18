#include "obstacle.h"

Obstacle::Obstacle(Coordinate loc = Coordinate(), double radius = 1)
{
	this->_loc = loc;
	this->_r = radius;
}

Coordinate Obstacle::getLocation() { return this->_loc; }
double Obstacle::getRadius() { return this->_r; }

void Obstacle::setLocation(Coordinate loc) { this->_loc = loc; }
void Obstacle::setRadius(double radius) { this->_r = radius; }