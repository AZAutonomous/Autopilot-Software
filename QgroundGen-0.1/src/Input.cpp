/*#include "Input.h"
#include <stdio.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

Input::Input(double lat, double lon, double alt, char typ)
{
	this->_altitude = alt;
	this->_longitude = lon;
	this->_latitude = lat;
	this->_type = typ;
}

void Input::printVector(char * file, _waypoints points)
{
	int size = points.getVector().size();
	int i = 0;
	cout << endl;
	std::ofstream myfile;
	myfile.open(file);
	myfile << "{\n";
	myfile << "\"MAV_AUTOPILOT\": 3,\n";
	myfile << "\"complexItems\": [],\n";
	myfile << "\"groundStation\": \"QGroundControl\",\n";
	myfile << "\"items\": [\n";
	/////first take off location
	myfile << "{\n";
	myfile << "\"autoContinue\": true,\n";
	myfile << "\"command\": 22,\n";
	myfile << "\"coordinate\": [";
	myfile << points.getVector().at(i).getLatitude() << "," << points.getVector().at(i).getLongitude() << "," << points.getVector().at(i).getAltitude() << "],\n";
	myfile << "\"frame\": 2,\n";
	myfile << "\"id\": ";
	myfile << (i + 1) << ",\n";
	myfile << "\"param1\": 10,\n";
	myfile << "\"param2\": 0,\n";
	myfile << "\"param3\": 0,\n";
	myfile << "\"param4\": 0,\n";
	myfile << "\"type\": \"missionItem\"\n";
	myfile << "},\n";
	//end of first take off comand
	i++;
	//prints waypoints of normal flight
	while (i < size-1) {
		myfile << "{\n";
		myfile << "\"autoContinue\": true,\n";
		myfile << "\"command\": 16,\n";
		myfile << "\"coordinate\": [";
		myfile << points.getVector().at(i).getLatitude() << "," << points.getVector().at(i).getLongitude() << "," << points.getVector().at(i).getAltitude() << "],\n";
		myfile << "\"frame\": 3,\n";
		myfile << "\"id\": ";
		myfile << (i + 1) << ",\n";
		myfile << "\"param1\": 0,\n";
		myfile << "\"param2\": 0,\n";
		myfile << "\"param3\": 0,\n";
		myfile << "\"param4\": 0,\n";
		myfile << "\"type\": \"missionItem\"\n";
		myfile << "},\n";
		i++;
	}
	//printf landing location
	myfile << "{\n";
	myfile << "\"autoContinue\": true,\n";
	myfile << "\"command\": 21,\n";
	myfile << "\"coordinate\": [";
	myfile << points.getVector().at(i).getLatitude() << "," << points.getVector().at(i).getLongitude() << "," << points.getVector().at(i).getAltitude() << "],\n";
	myfile << "\"frame\": 3,\n";
	myfile << "\"id\": ";
	myfile << (i + 1) << ",\n";
	myfile << "\"param1\": 10,\n";
	myfile << "\"param2\": 0,\n";
	myfile << "\"param3\": 0,\n";
	myfile << "\"param4\": 0,\n";
	myfile << "\"type\": \"missionItem\"\n";
	myfile << "},\n";

	///////////////////////////////////////////////
	
	myfile << "],\n";
	//print planned home position
	myfile << "\"plannedHomePosition\": {\n";
	myfile << "\"autoContinue\": true,\n";
	myfile << "\"command\": 16,\n";
	myfile << "\"coordinate\": [";
	myfile << points.getVector().at(i).getLatitude() << "," << points.getVector().at(i).getLongitude() << "," << points.getVector().at(i).getAltitude() << "],\n";
	myfile << "\"frame\": 3,\n";
	myfile << "\"id\": ";
	myfile << (i + 1) << ",\n";
	myfile << "\"param1\": 0,\n";
	myfile << "\"param2\": 0,\n";
	myfile << "\"param3\": 0,\n";
	myfile << "\"param4\": 0,\n";
	myfile << "\"type\": \"missionItem\"\n";
	myfile << "},\n";

	//////////////////////////////////

	myfile << "\"version\": \"1.0\"\n";

	myfile << "}";
	myfile.close();
}

Input::Input()
{
	this->_latitude = 0.0;
	this->_longitude = 0.0;
	this->_altitude = 0.0;
}

char Input::getType() const
{
	return this->_type;
}

double Input::getLatitude() const
{
	return this->_latitude;
}

double Input::getLongitude() const
{
	return this->_longitude;
}

double Input::getAltitude() const
{
	return this->_altitude;
}

void Input::setType(char typ)
{
	this->_type = typ;
}


void Input::setLatitude(double lat)
{
	this->_latitude = lat;
}

void Input::setLongitude(double lon)
{
	this->_longitude = lon;
}

void Input::setAltitude(double alt)
{
	this->_altitude = alt;
}*/


