#include "coordinate.h"

using namespace std;

Coordinate::Coordinate(){

}

void Coordinate::setID(std::string id){
	this->_id = id;
}

void Coordinate::setName(std::string name){
	this->_name = name;
}

void Coordinate::setLatitude(double lat){
	this->_latitude = lat;
}

void Coordinate::setLongitude(double lon){
	this->_longitude = lon;
}

void Coordinate::setAltitude(double alt){
	this->_altitude = alt;
}

double Coordinate::getLatitude() const{
	return this->_latitude;
}

double Coordinate::getLongitude() const{
	return this->_longitude;
}

double Coordinate::getAltitude()const{
	return this->_altitude;
}
//------------------------------------------------------------------------------------------------


