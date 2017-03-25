#include <iostream>
#include "path.h"

//------------------------------------------------------------------------------------------------

using namespace std;

//------------------------------------------------------------------------------------------------

int main(int argc, char *argv[]) {
	Coordinate homePosition;
	Coordinate takeoffPosition;
	double lat = 0.0;
	double lon = 0.0;

	cout << "Enter HOME Coordinate (lat long): ";
	cin >> lat >> lon;
	homePosition.setLatitude(lat);
	homePosition.setLongitude(lon);
	cout << "Enter TAKEOFF Coordinate (lat long): ";
	cin >> lat >> lon;
	takeoffPosition.setLatitude(lat);
	takeoffPosition.setLongitude(lon);

	// Read from file
	Path navigation;

	navigation.ReadFromFile(argv[1]);

	navigation.InsertTakeoffandHomeposition(homePosition, takeoffPosition);

	navigation.WritetoFile(argv[2]);


	return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------------------------
