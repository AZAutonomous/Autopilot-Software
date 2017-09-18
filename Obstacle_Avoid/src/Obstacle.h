#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdlib.h>
#include <vector>
#include <string>

class Obsticale {
private:
	std::string _id;
	std::string _name;
	double _latitude;
	double _longitude;
	double _altitude;
	double _radius;

public:
	Obsticale();

	void setID(std::string id);
	std::string getID() const;

	void setName(std::string name);
	std::string getName() const;

	void setLatitude(double lat);
	double getLatitude() const;

	void setLongitude(double lon);
	double getLongitude() const;

	void setAltitude(double alt);
	double getAltitude() const;

	void setRadius(double rad);
	double getRadius() const;

};

#endif
