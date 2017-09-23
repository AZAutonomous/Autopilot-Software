#include <iostream>
#include "path.h"
#include "source.h"

//------------------------------------------------------------------------------------------------

using namespace std;

//------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
	Source datasource;
	string file;
	file = "../testFiles/2017Suasmission.txt";
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
	homePosition.setAltitude(0);
	homePosition.setLatitude(0.0);
	homePosition.setLongitude(0.0);
	takeoffPosition.setAltitude(0);
	takeoffPosition.setLatitude(0.0);
	takeoffPosition.setLongitude(0.0);
	takeoffway.setAltitude(0);
	takeoffway.setLatitude(0.0);
	takeoffway.setLongitude(0.0);
	landingway.setAltitude(0);
	landingway.setLatitude(0.0);
	landingway.setLongitude(0.0);
	landingPosition.setAltitude(0);
	landingPosition.setLatitude(0.0);
	landingPosition.setLongitude(0.0);
	//---------------------------------------------------------------------------------------------------------------
	Path navigation;

	//navigation.ReadFromFile(argv[1]);
	//delete these lines when done with testing ------------------------------------------
	navigation.CreatTestSearchPoints();
	Coordinate tempCoor;
	tempCoor.setAltitude(100);
	tempCoor.setLatitude(0.1);
	tempCoor.setLongitude(0.1);
	//-----------------------------------------------------------------------------
	// Search Path Generation ----------------------------------------------------------------------
	navigation.CreateSearchWaypoints(0.05);

	SearchSize = navigation.getSVector().size();
	navigation.sort(0, SearchSize - 1);

	WaypointSize = navigation.getWVector().size();
	navigation.SwapSearchVectors(SearchSize, 0.05);

	//place tessellate function here
	//navigation.TessellatePath(0.03, 0.05);
	//--------------------------------------------------------------------------------------------------
	navigation.PushtoWaypoints(SearchSize, WaypointSize);
	//------------------------------------------------------------------------------------------------
	//navigation.InsertTakeoffandHomeposition(homePosition, takeoffPosition);

	navigation.SetVectorIds();

	//navigation.WritetoFile(argv[2]);
	datasource.WriteToFile(outputfile, homePosition, takeoffPosition, landingPosition, takeoffway,navigation);

	return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------------------------
