#ifndef PATH_H
#define PATH_H
#include <vector>
#include "coordinate.h"
#include "obstacle.h"


struct Path;

struct Path {
private:
	std::vector<Coordinate> _coordinates;
public:
	/*
	calc if there is a collision between _coordinates[0] and _coordinates[1]
	calculate this over all obstacles
	returns all obsticals it collides with(in under x time)
	*/
	std::vector<obstacle> hasCollition(std::vector<obstacle> obstacles);
	/*
	calculates the anchor point to draw avoidance over
	chooses intersection point closest to first waypoint
	*/
	Coordinate calculateAnchorPoint(std::vector<obstacle> obstacles);
	/*
	creats the intersection point over a single obstacle
	if all 3 path lines intersect choose middle intersection point
	if 2 path lines intersect take the average of their intersection points
	if 1 path line intersects take its intersection point
	*/
	Coordinate makeAnchorPoint(obstacle obstacle);

	/*
	runs steps 4 and 5 (4 and 5 names go here their the functions that come below this)
	calculate new avoicande path and append it to the path
	steps:
	1. calculate the 5 (black) avoidance anchors (calculated from anchor point)
	2. (function 4) creat path for each (black) avoidance anchor and calc danger stat
	3. (function 5) adjust path for each avoidance anchor
	4. choose the new path with lowest danger stat
	5. append the new path to the path
	*/
	void appendAvoidanceNodes(Coordinate anchorPoint);

	/*
	function step 4
	creates a vector of avoidance coordinates given the relative anchor point (one of the 5 black points created in step 4)
	*/
	std::vector<Coordinate> createAvoidancePath(Coordinate relativeAnchorPoint);

	//function step 5-------------------------

	/*
	adjust and calculate the avoidance path x num of times
	after x num of times return the current danger stat
	calls function 6 and 7
	*/
	double calculateAvoidancePath(std::vector<Coordinate> avoidancePath);

	/*
	function step 6
	readjust the avoidancepath by moving nodes perp to the old path
	*/
	void adjustAvoidancePath(std::vector<Coordinate> avoidancePath);

	/*
	functio step 7
	returns double representing danger stat
	*/
	double evaluateAvoidancePath(std::vector<Coordinate> avoidancePath);
};


#endif