#ifndef SOURCE_H
#define SOURCE_H
#include <vector>
#include <string>
#include "coordinate.h"
class Source
{
public:
	bool ReadFromFile(std::string filename, double SearchAlt);
	//converts polar coordinates into dec
	double CalculateCoordtoDec(double deg, double min, double sec);
	//writes to file taking in alt, turn around dist, spacing, camera
	bool WriteToFile(std::string filename,Coordinate home, double alt, double turnDist, double Space, double Camera);
	bool WritetoFile2(std::string filename, Coordinate home, double alt, double turnDist, double Space, double Camera);
private:
	std::vector<Coordinate> _waypoints;
	std::vector<Coordinate> _searchArea;
	std::vector<Coordinate> _opArea;
	
	
};


#endif