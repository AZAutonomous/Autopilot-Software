#include <iostream>
#include <string>
#include "source.h"
#include "coordinate.h"

//------------------------------------------------------------------------------------------------

using namespace std;

//------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
	Source datasource;
	string file;
	file  = "C:/Users/james/Desktop/QgroundGen/test1.txt";
	string outputfile;
	outputfile = "C:/Users/james/Desktop/QgroundGen/out1.txt";
	datasource.ReadFromFile(file, 100);
	Coordinate homePos;
	homePos.setAltitude(0.0);
	homePos.setLatitude(38.5);
	homePos.setLongitude(39.1);
	Coordinate takeoff;
	takeoff.setAltitude(0.0);
	takeoff.setLongitude(39.11);
	takeoff.setLatitude(38.51);
	datasource.WriteToFile(outputfile,homePos,takeoff,100,10,10,25);

	return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------------------------
