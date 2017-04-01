#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "coordinate.h"

class Path {
private:
	std::vector<Coordinate> _opArea;
	std::vector<Coordinate> _searchArea;
	std::vector<Coordinate> _waypoints;

public:
	Path();
	// Other public methods go here.

	bool ReadFromFile(std::string filename);
	void WritetoFile(std::string filename);
	void CreateSearchWaypoints(double VeiwRadius);
	void InsertTakeoffandHomeposition(Coordinate home, Coordinate takeoff);
	double CalculateCoordtoDec(double deg, double min, double sec);
};

#endif

//------------------------------------------------------------------------------------------------


