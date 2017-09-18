#include "Obstacle.h"

Obsticale::Obsticale()
{


}

void Obsticale::setID(std::string id)
{
	this->_id = id;
}

std::string Obsticale::getID() const
{
	return this->_id;
}

void Obsticale::setName(std::string name)
{
	this->_name = name;
}

std::string Obsticale::getName() const
{
	return this->_name;
}

void Obsticale::setLatitude(double lat)
{
	this->_latitude = lat;
}

double Obsticale::getLatitude() const
{
	return this->_latitude;
}

void Obsticale::setLongitude(double lon)
{
	this->_longitude = lon;
}

double Obsticale::getLongitude() const
{
	return this->_longitude;
}

void Obsticale::setAltitude(double alt)
{
	this->_altitude = alt;
}

double Obsticale::getAltitude() const
{
	return this->_altitude;
}

void Obsticale::setRadius(double rad)
{
	this->_radius = rad;
}

double Obsticale::getRadius() const
{
	return this->_radius;
}
