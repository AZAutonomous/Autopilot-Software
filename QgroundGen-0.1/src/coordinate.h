#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <vector>
#include <string>

class Coordinate {
private:
	std::string _id;
	std::string _name;
	double _latitude;
	double _longitude;
	double _altitude;

public:
	Coordinate();

	void setID(std::string id);
	void setName(std::string name);

	void setLatitude(double lat);
	double getLatitude() const;

	void setLongitude(double lon);
	double getLongitude() const;

	void setAltitude(double alt);
	double getAltitude() const;

};

#endif
