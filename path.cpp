#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include "path.h"
#include "coordinate.h"

using namespace std;

Path::Path(){

}

bool Path::ReadFromFile(string filename){
	ifstream inFile;
	string buffer;
	string temp;
	Coordinate coord;
	string longitude, latitude, minutes, seconds, degrees;
	double degs, mins, secs;
	double lat, lon, alt;
	int flag = 0;

	inFile.open(filename);
	if (!inFile.good()) {
		cout << "Could not open input file " << filename << endl;
		return false;
	}

	while (!inFile.eof()){
		getline(inFile, buffer);
		if (buffer.empty()){
			continue;
		}
		if (buffer.compare(0,5,"Point") == 0){
			continue;
		}
		// Switch vectors
		if (buffer == ",,,,"){
			continue;
		}
		istringstream linestream(buffer);
		// Add Coordinate to Op Area vector
		if (buffer.compare(0, 1, "A") == 0){
			flag = 1;
		}
		// Add Coordinate to Search Area vector
		else if (buffer.compare(0, 1, "S") == 0){
			flag = 2;
		}
		// Add Coordinate to Waypoint vector
		else if (buffer.compare(0, 1, "W") == 0){
			flag = 3;
		}
		int i = 0;

		while (linestream.good()){
			getline(linestream, temp, ',');
			// Record Waypoint ID
			if (i == 0){
				temp.erase(0, 1);
				coord.setID(temp);
				i = 1;
			}
			// Record Waypoint Name
			else if (i == 1){
				coord.setName(temp);
				i = 2;
			}
			// Extract Latitude Coordinate, convert to decimal
			else if (i == 2){
				temp.erase(0, 1);

				degrees = temp.substr(0, 2);
				degs = stod(degrees);

				minutes = temp.substr(3, 2);
				mins = stod(minutes);

				seconds = temp.substr(6, 5);
				secs = stod(seconds);

				lat = CalculateCoordtoDec(degs, mins, secs);
				coord.setLatitude(lat);
				i = 3;
			}
			// Extract Longitude Coordinate, convert to decimal
			else if (i == 3){
				temp.erase(0, 1);

				degrees = temp.substr(0, 3);
				degs = stod(degrees);

				minutes = temp.substr(4, 2);
				mins = stod(minutes);

				seconds = temp.substr(7, 5);
				secs = stod(seconds);

				lon = CalculateCoordtoDec(degs, mins, secs);
				coord.setLongitude(lon);
				i = 4;
			}
			else if (i == 4){
				alt = stod(temp);
				coord.setAltitude(alt);
			}
		}
		if (flag == 1){
			_opArea.push_back(coord);
		}
		else if (flag == 2){
			_searchArea.push_back(coord);
		}
		else if (flag == 3){
			_waypoints.push_back(coord);
		}

	}
	
	return true;
}

void Path::WritetoFile(string filename){
	ofstream outFile;

	outFile.open(filename);
	if (!outFile.good()) {
		cout << "Could not open output file " << filename << endl;
	}

	outFile << "{" << endl;
	outFile << "\t\"MAV_AUTOPILOT\": 3," << endl;
	outFile << "\t\"complexItems\": [" << endl;
	outFile << "\t]," << endl;
	outFile << "\t\"groundStation\": \"QGroundControl\"," << endl;
	outFile << "\t\"items\": [" << endl;
	outFile << "\t\t{" << endl;

}

double Path::CalculateCoordtoDec(double deg, double min, double sec){
	double decimal = 0.0;
	double degree;
	double minute;
	double second;

	decimal = deg + (min / 60.0) + (sec / 3600.0);

	return decimal;
}

void Path::InsertTakeoffandHomeposition(Coordinate home, Coordinate takeoff){
	home.setAltitude(0);
	home.setID("0");

	takeoff.setID("1");
	takeoff.setAltitude(_waypoints.at(0).getAltitude());

	_waypoints.push_back(home);
	_waypoints.insert(_waypoints.begin(), takeoff);
}
//------------------------------------------------------------------------------------------------


