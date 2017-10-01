#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdlib.h>
#include <vector>
#include <string>

class Obstacle {
private:
	int _id;
	double _radius;
	double _latitude;
	double _longitude;

public:
	Obstacle();

	void set_id(int id);
	int get_id() const;

	void set_radius(double radius);
	double get_radius() const;

	void set_latitude(double latitude);
	double get_latitude() const;

	void set_longitude(double longitude);
	double get_longitude() const;



};

#endif

