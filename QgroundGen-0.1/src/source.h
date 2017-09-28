#ifndef SOURCE_H
#define SOURCE_H
#include <vector>
#include <string>
#include "coordinate.h"
#include "path.h"

class Source
{
public:
	bool ReadFromFile(std::string filename, Path* path, double SearchAlt);
	//converts polar coordinates into dec
	double CalculateCoordtoDec(double deg, double min, double sec);
	//writes to file taking in alt, turn around dist, spacing, camera
	bool WriteToFile(std::string filename,Coordinate home,Coordinate Takeoff,Coordinate landway, Coordinate takeoffWay,Path path);
	bool WriteToFile (char* filename, Coordinate home, Coordinate takeoff, Coordinate descent, Coordinate ascent, Path path);
};


#endif