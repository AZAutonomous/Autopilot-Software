#ifndef PATH_H
#define PATH_H
#include <vector>
#include "coordinate.h"
using namespace std;

class Path {
private:
	vector<Coordinate> _waypoints; //Holds every point in the order that they will be traveled to
	vector<Coordinate> _search_corners; //Holds the coordinates for the search area corners
	vector<Coordinate> _op_area_corners; //Holds the coordinates for the op area corners
	vector<Coordinate> _bounding_box; //Coordinates that define the box around the search area
	double DmsToDecimal(double deg, double min, double sec); //Used in the ReadFromFile function, has no use otherwise

public:
	Path();

	vector<Coordinate> getWaypoints() const;
	vector<Coordinate> getSearchCorners() const;
	vector<Coordinate> getOpAreaCorners() const;
	vector<Coordinate> getBoundingBox() const;

	void setWaypoints(vector<Coordinate> waypoints);
	void setSearchCorners(vector<Coordinate> search_corners);
	void setOpAreaCorners(vector<Coordinate> op_area_corners);
	void setBoundingBox(vector<Coordinate> bounding_box); //Shouldn't be used except for testing purposes

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
};

#endif