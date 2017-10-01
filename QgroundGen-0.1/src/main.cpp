#include <iostream>
#include "path.h"
#include "source.h"
#include "obstacle_list.h"

//------------------------------------------------------------------------------------------------

using namespace std;

//------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
	std::vector<Obstacle> obstacles = createObstacles("../testFiles/obstacles.txt");
	Source datasource;
	Path navigation;
	string file;
	file = "../testFiles/2017Suasmission.txt";
	datasource.ReadFromFile (file, &navigation, 50.0);
	string outputfile;
	outputfile = "../testFiles/testOutput.mission";
	double lat = 0.0;
	double lon = 0.0;
	int SearchSize = 0;
	int WaypointSize = 0;
	
	//remove this comment when not testing functions

	/*
	cout << "Enter HOME Coordinate (lat long): ";
	cin >> lat >> lon;
	homePosition.setLatitude(lat);
	homePosition.setLongitude(lon);
	cout << "Enter TAKEOFF Coordinate (lat long): ";
	cin >> lat >> lon;
	takeoffPosition.setLatitude(lat);
	takeoffPosition.setLongitude(lon);
	*/
	// Read from file
	//Heres a bunch of test vectors delete these later plz --------------------------------------------------------------
	Coordinate homePosition;
	Coordinate takeoffPosition;
	Coordinate landingPosition;
	Coordinate landingway;
	Coordinate takeoffway;
	homePosition.setAltitude(0.0);
	homePosition.setLatitude(38.1509);
	homePosition.setLongitude(-76.4342);
	takeoffPosition.setAltitude(60.96);
	takeoffPosition.setLatitude(38.1453);
	takeoffPosition.setLongitude(-76.4291);
	takeoffway.setAltitude(91.44);
	takeoffway.setLatitude(38.1492);
	takeoffway.setLongitude(-76.4295);
	landingway.setAltitude(50.0);
	landingway.setLatitude(38.1509);
	landingway.setLongitude(-76.4342);
	landingPosition.setAltitude(0.0);
	landingPosition.setLatitude(38.1509);
	landingPosition.setLongitude(-76.4342);
	//---------------------------------------------------------------------------------------------------------------

	//navigation.ReadFromFile(argv[1]);
	//delete these lines when done with testing ------------------------------------------
	/*navigation.CreatTestSearchPoints();
	Coordinate tempCoor;
	tempCoor.setAltitude(100);
	tempCoor.setLatitude(0.1);
	tempCoor.setLongitude(0.1);*/
	//-----------------------------------------------------------------------------
	// Search Path Generation ----------------------------------------------------------------------
	navigation.CreateSearchWaypoints(0.0005);

	SearchSize = navigation.getSVector().size();
	navigation.sort(0, SearchSize - 1);

	WaypointSize = navigation.getWVector().size();
	navigation.SwapSearchVectors(SearchSize, 0.0005);

	//place tessellate function here
	navigation.TessellatePath(0.0003, 0.0005); //Comment out if tesselation not desired
	//--------------------------------------------------------------------------------------------------
	navigation.PushtoWaypoints(SearchSize, WaypointSize);
	//------------------------------------------------------------------------------------------------
	//navigation.InsertTakeoffandHomeposition(homePosition, takeoffPosition);

	navigation.SetVectorIds();		

	//navigation.WritetoFile(argv[2]);
	//datasource.WriteToFile ("../testFiles/testOutput.mission", homePosition, takeoffPosition, landingPosition, takeoffway, navigation); //For fprintf
	datasource.WriteToFile(outputfile, homePosition, takeoffPosition, landingPosition, takeoffway,navigation);

	return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------------------------
