#ifndef AVOIDANCE_H
#define AVOIDANCE_H

#include <stdlib.h>
#include <vector>
#include <string>
#include "coordinate.h"
#include "Obstacle.h"

class Avoidance {
private:
	//type of avoidance, 1 for cylindar, 2 for sphere
	int _type;

public:
	Avoidance();
	//getters / setters
	void setType(int t);
	int getType() const;
	//_______________________________________________________
	bool isCollision(Coordinate coor1, Coordinate coor2, Obsticale obst);
	Coordinate getAvoidLoc(Coordinate coor1, Coordinate coor2, Obsticale obst);
};

#endif
