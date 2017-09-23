#include "pair.h"

using namespace std;

void Pair::setFirst(Coordinate coor)
{
	this->_firstLoc = coor;
}

void Pair::setLast(Coordinate coor)
{
	this->_lastLoc = coor;
}

Coordinate Pair::getFirst()
{
	return this->_firstLoc;
}

Coordinate Pair::getLast()
{
	return this->_lastLoc;
}
