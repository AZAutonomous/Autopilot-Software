#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "coordinate.h"

class Path {
private:
	std::vector<Coordinate> _opArea;
	std::vector<Coordinate> _searchArea;
	std::vector<Coordinate> _waypoints;
	int partition(int i, int k);

public:
	Path();
	//geter for the vector
	std::vector<Coordinate> getSVector() const;

	std::vector<Coordinate> getWVector() const;

	// Other public methods go here.
	//----------------------------------------------------------------------------------------------------//
	bool ReadFromFile(std::string filename);

	void WritetoFile(std::string filename);

	//Creates the waypoints to create the search path
	//uses a given view radius
	void CreateSearchWaypoints(double VeiwRadius);
	//used to test functions -- delete after program is finished
	void CreatTestSearchPoints();
	//used in CreateSeachWaypoints -- creates a set of points along the pirimeter of the searcharea vectors
	void CreatePirimeterVectors(Coordinate input1, Coordinate input2, double ViewRadius, double Altitude);
	//sorts the vectors along the pirimeter into a ladder path
	void sort(int i, int k);
	//used in CreateSearchWaypint -- shrinks the original searchArea vectors
	void Shrinkvector(Coordinate* inputCoordinate, double viewRadius, Coordinate centerCoordinate);
	//swaps the search vectors so they are in a ladder pattern
	void SwapSearchVectors(int size);
	// pushes search vectors into waypoints
	void PushtoWaypoints(int Searchsize,int Waypointsize);

	void InsertTakeoffandHomeposition(Coordinate home, Coordinate takeoff);

	double CalculateCoordtoDec(double deg, double min, double sec);

	double GetDistance(double x1, double y1, double x2, double y2);
};

#endif

//------------------------------------------------------------------------------------------------


