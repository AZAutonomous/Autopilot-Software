#include "obstacle.h"

using namespace std;

Obstacle::Obstacle()
{
	this->_id = 0;
	this->_latitude = 0.0;
	this->_longitude = 0.0;
	this->_radius = 0;
}

void Obstacle::set_id(int id)
{
	this->_id = id;
}

int Obstacle::get_id() const
{
	return this->_id;
}

void Obstacle::set_radius(double radius)
{
	this->_radius = radius;
}

double Obstacle::get_radius() const
{
	return this->_radius;
}

void Obstacle::set_latitude(double latitude)
{
	this->_latitude = latitude;
}

double Obstacle::get_latitude() const
{
	return this->_latitude;
}

void Obstacle::set_longitude(double longitude)
{
	this->_longitude = longitude;
}

double Obstacle::get_longitude() const
{
	return this->_longitude;
}
