#ifndef PAIR_H
#define PAIR_H

#include <stdlib.h>
#include <vector>
#include <string>
#include "coordinate.h"

class Pair {
private:
	Coordinate _firstLoc;
	Coordinate _lastLoc;

public:
	void setFirst(Coordinate coor);
	void setLast(Coordinate coor);
	Coordinate getFirst();
	Coordinate getLast();
};

#endif