#ifndef PATH_H
#define PATH_H
#include <vector>
#include "coordinate.h"
#include "obstacle.h"
using namespace std;

class Path {
private:
	vector<Coordinate> _waypoints; //Holds every point in the order that they will be traveled to
	vector<Coordinate> _search_corners; //Holds the coordinates for the search area corners
	vector<Coordinate> _op_area_corners; //Holds the coordinates for the op area corners
	vector<Coordinate> _bounding_box; //Coordinates that define the box around the search area
	vector<Obstacle> _obstacles; //Holds the obstacles
	double DmsToDecimal(double deg, double min, double sec); //Used in the ReadFromFile function, has no use otherwise

public:
	Path();

	vector<Coordinate> getWaypoints() const;
	vector<Coordinate> getSearchCorners() const;
	vector<Coordinate> getOpAreaCorners() const;
	vector<Coordinate> getBoundingBox() const;
	vector<Obstacle> getObstacles() const;

	void setWaypoints(vector<Coordinate> waypoints);
	void setSearchCorners(vector<Coordinate> search_corners);
	void setOpAreaCorners(vector<Coordinate> op_area_corners);
	void setBoundingBox(vector<Coordinate> bounding_box); //Shouldn't be used except for testing purposes
	void setObstacles(vector<Obstacle> obstacles);

	//void PartitionSearchArea(); //Possible approach, not a priority. Divides the search area into the smallest number of rectangles that covers the entire area
	bool ReadFromFile(string file_path, double search_alt); //Reads data from a file containing points and their designations and assigns them to the waypoint or corner vectors
	bool WriteToFile(string file_path, Coordinate home, Coordinate takeoff, Coordinate descent, Coordinate ascent);
	void DefineNormalBoundingBox(double search_alt); //Sets up the rectangle that bounds the search area, always oriented perpendicular to cardinal directions
	void DefineBoundingBox(double search_alt); //Sets up the rectangle, oriented so its longest edges are parallel to the longest edges of the search area
	void CreateNormalEdgeNodes(double view_radius); //Adds points to the short edge of the rectangle created by DefineNormalBoundingBox to use as waypoints
	void CreateEdgeNodes(double view_radius); //Same as above but for a bounding box that is not aligned with the cardinal directions
	void SortNormalBoundingBoxNodes(); //Takes the nodes of the bounding box and puts them in the order they are to be flown to
	void ShrinkNormalNodesToFit(); //Takes staggered pairs of points and shifts them closer to the search area to reduce flight outside of search area for normal bounding box
	void PushToWaypoints(); //Takes the bounding box waypoints and adds them to the waypoint vector, adding the search area to the path
	void PushSearchToWaypoints(); //For debugging. Pushes the search boundary to waypoints
	void PushOpToWaypoints(); //For debugging. Pushes the op area boundary to waypoints
	void PushObsToWaypoints(); //For debugging. Pushes the obstacles to the waypoint vector so they can be seen on the map
	bool ReadObstacles(string file_path);
	int hasCollision(Coordinate coorA, Coordinate coorB, Obstacle O); //returns 0 if no collosion between points, 1 if between points 2 if on coorA and 3 if on coorB
	Coordinate AvoidObstacle(Coordinate A, Coordinate B, Circle O, int flag); //After a detection, this will run to create an avoidance point
};

#endif