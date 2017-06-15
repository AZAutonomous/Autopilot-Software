#include "source.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

bool Source::ReadFromFile(string filename, double SearchAlt){
	ifstream inFile;
	string buffer;
	string temp;
	Coordinate coord;
	string Point, Name, Name2, Latitude, Longitude, Altitude;
	string DD, MM, SS;
	double degs, mins, secs;
	double lat, lon, alt;
	int flag = 0;
	
	inFile.open(filename);
	if (!inFile.good()) {
		cout << "Could not open input file " << filename << endl;
		return false;
	}
	while (getline(inFile,buffer)) {
		istringstream ISS(buffer);
		ISS >> Point;
		ISS >> Name;
		ISS >> Name2;
		ISS >> Latitude;
		ISS >> Longitude;
		ISS >> Altitude;
		if (Point.compare(0, 1, "A") == 0) {
			flag = 1;
		}
		// Add Coordinate to Search Area vector
		else if (Point.compare(0, 1, "S") == 0) {
			flag = 2;
		}
		// Add Coordinate to Waypoint vector
		else if (Point.compare(0, 1, "W") == 0) {
			flag = 3;
		}
		// get latitude --------------------------------------------------------------------------
		DD = Latitude.substr(1, 2);
		degs = stod(DD);

		MM = Latitude.substr(4, 2);
		mins = stod(MM);

		SS = Latitude.substr(7, 5);
		secs = stod(SS);
		lat = CalculateCoordtoDec(degs, mins, secs);
		//_________________________________________________________________________________________
		//get longitude ---------------------------------------------------------------------------
		DD = Longitude.substr(1, 2);
		degs = stod(DD);

		MM = Longitude.substr(4, 2);
		mins = stod(MM);

		SS = Longitude.substr(7, 5);
		secs = stod(SS);
		lon = CalculateCoordtoDec(degs, mins, secs);
		//_________________________________________________________________________________________
		Coordinate newCoor;
		//get altitude-----------------------------------------------------------------------------
		if (Altitude == "-") {
			newCoor.setAltitude(SearchAlt);
		}
		else {
			newCoor.setAltitude(stod(Altitude)*0.3048);
		}
		//add coordinate points--------------------------------------------------------------------
		
		newCoor.setLatitude(lat);
		newCoor.setLongitude(-1*lon);
		if (flag == 3) {
			int size = _waypoints.size() + 1;
			newCoor.setID(to_string(size));
				_waypoints.push_back(newCoor);
		}
		else if (flag == 2) {
			_searchArea.push_back(newCoor);
		}
		else if (flag == 1) {
			_opArea.push_back(newCoor);
		}

	}

	return true;
}

double Source::CalculateCoordtoDec(double deg, double min, double sec)
{
	double decimal = 0.0;
	decimal = deg + (min / 60.0) + (sec / 3600.0);
	return decimal;
}

bool Source::WriteToFile(std::string filename,Coordinate home, double alt, double turnDist, double Space, double Camera)
{
	ofstream fileout;
	fileout.open(filename);
	if (!fileout.good()) {
		return false;
	}
	fileout << "{\n";
	fileout << "\"firmwareType\": 12,\n\"groundStation\" : \"QGroundControl\",\n\"items\" : [\n";
	//print take off
	/*
	fileout << "{\n";
	fileout << "\"autoContinue\": true,\n\"command\": 22,\n\"coordinate\": [\n";
	fileout << takeoff.getLatitude() << ",\n";
	fileout << takeoff.getLongitude() << ",\n";
	fileout << takeoff.getAltitude() << "\n";
	fileout << "],\n";
	fileout << "\"doJumpId\": " << "1" << ",\n";
	fileout << "\"frame\": 3,\n";
	fileout << "\"params\": [\n15,\n0,\n0,\n0\n],\n";
	fileout << "\"type\": \"SimpleItem\"\n";
	fileout << "},\n";
	//print take off way
	fileout << "{\n";
	fileout << "\"autoContinue\": true,\n\"command\": 16,\n\"coordinate\": [\n";
	fileout << takeoffWay.getLatitude() << ",\n";
	fileout << takeoffWay.getLongitude() << ",\n";
	fileout << takeoffWay.getAltitude() << "\n";
	fileout << "],\n";
	fileout << "\"doJumpId\": " << "1" << ",\n";
	fileout << "\"frame\": 3,\n";
	fileout << "\"params\": [\n15,\n0,\n0,\n0\n],\n";
	fileout << "\"type\": \"SimpleItem\"\n";
	fileout << "},\n";
	*/
	//print other coordinates
	int size = _waypoints.size();
	for (int i = 0; i < size; i++) {
		fileout << "{\n";
		fileout << "\"autoContinue\": true,\n\"command\": 16,\n\"coordinate\": [\n";
		fileout << _waypoints.at(i).getLatitude() << ",\n";
		fileout << _waypoints.at(i).getLongitude() << ",\n";
		fileout << _waypoints.at(i).getAltitude() << "\n";
		fileout << "],\n";
		fileout << "\"doJumpId\": " << i+2 << ",\n";
		fileout << "\"frame\": 3,\n";
		fileout << "\"params\": [\n0,\n0,\n0,\n0\n],\n";
		fileout << "\"type\": \"SimpleItem\"\n";
		fileout << "},\n";
	}
	//--------------------------------------------------------------
	//print the search path
	fileout << "{\n";
	fileout << "\"cameraTrigger\": true,\n";
	fileout << "\"cameraTriggerDistance\": " << Camera << ",\n";
	fileout << "\"complexItemType\": \"survey\",\n";
	fileout << "\"fixedValueIsAltitude\": false,\n";
	fileout << "\"grid\": {\n";
	fileout << "\"altitude\": " << alt*0.3048 << ",\n";
	fileout << "\"angle\": 0,\n";
	fileout << "\"relativeAltitude\": true,\n";
	fileout << "\"spacing\": " << Space << ",\n";
	fileout << "\"turnAroundDistance\": " << turnDist << "\n";
	fileout << "},\n";
	fileout << "\"manualGrid\": true,\n";
	fileout << "\"polygon\": [\n";
	int size2 = _searchArea.size();
	for (int i = 0; i < size2-1; i++) {
		fileout << "[\n";
		fileout << _searchArea.at(i).getLatitude() << ",\n";
		fileout << _searchArea.at(i).getLongitude() << "\n";
		fileout << "],\n";
	}
	fileout << "[\n";
	fileout << _searchArea.at(size2-1).getLatitude() << ",\n";
	fileout << _searchArea.at(size2-1).getLongitude() << "\n";
	fileout << "]\n";

	fileout << "],\n";
	fileout << "\"type\": \"ComplexItem\",\n";
	fileout << "\"version\": 3\n";
	fileout << "}\n";
	//--------------------------------------------------------------
	//print pre land
	/*
	fileout << "{\n";
	fileout << "\"autoContinue\": true,\n\"command\": 16,\n\"coordinate\": [\n";
	fileout << landway.getLatitude() << ",\n";
	fileout << landway.getLongitude() << ",\n";
	fileout << landway.getAltitude() << "\n";
	fileout << "],\n";
	fileout << "\"doJumpId\": " << "1" << ",\n";
	fileout << "\"frame\": 3,\n";
	fileout << "\"params\": [\n15,\n0,\n0,\n0\n],\n";
	fileout << "\"type\": \"SimpleItem\"\n";
	fileout << "},\n";
	//print land
	fileout << "{\n";
	fileout << "\"autoContinue\": true,\n\"command\": 21,\n\"coordinate\": [\n";
	fileout << home.getLatitude() << ",\n";
	fileout << home.getLongitude() << ",\n";
	fileout << home.getAltitude() << "\n";
	fileout << "],\n";
	fileout << "\"doJumpId\": " << "1" << ",\n";
	fileout << "\"frame\": 3,\n";
	fileout << "\"params\": [\n15,\n0,\n0,\n0\n],\n";
	fileout << "\"type\": \"SimpleItem\"\n";
	fileout << "}\n";
	*/
	//print the home pos
	fileout << "],\n";
	fileout << "\"plannedHomePosition\": [\n";
	fileout << home.getLatitude() << ",\n";
	fileout << home.getLongitude() << ",\n";
	fileout << home.getAltitude() << "\n";
	fileout << "],\n";
	fileout << "\"version\": 2\n";
	fileout << "}";
	return true;
}

bool Source::WritetoFile2(std::string filename, Coordinate home, double alt, double turnDist, double Space, double Camera)
{
	ofstream fileout;
	fileout.open(filename);
	if (!fileout.good()) {
		return false;
	}
	fileout << "{\n";
	fileout << "\"firmwareType\": 12,\n\"groundStation\" : \"QGroundControl\",\n\"items\" : [\n";
	//print the search path
	fileout << "{\n";
	fileout << "\"cameraTrigger\": true,\n";
	fileout << "\"cameraTriggerDistance\": " << Camera << ",\n";
	fileout << "\"complexItemType\": \"survey\",\n";
	fileout << "\"fixedValueIsAltitude\": false,\n";
	fileout << "\"grid\": {\n";
	fileout << "\"altitude\": " << alt*0.3048 << ",\n";
	fileout << "\"angle\": 0,\n";
	fileout << "\"relativeAltitude\": true,\n";
	fileout << "\"spacing\": " << Space << ",\n";
	fileout << "\"turnAroundDistance\": " << turnDist << "\n";
	fileout << "},\n";
	fileout << "\"manualGrid\": true,\n";
	fileout << "\"polygon\": [\n";
	int size2 = _searchArea.size();
	for (int i = 0; i < size2 - 1; i++) {
		fileout << "[\n";
		fileout << _searchArea.at(i).getLatitude() << ",\n";
		fileout << _searchArea.at(i).getLongitude() << "\n";
		fileout << "],\n";
	}
	fileout << "[\n";
	fileout << _searchArea.at(size2 - 1).getLatitude() << ",\n";
	fileout << _searchArea.at(size2 - 1).getLongitude() << "\n";
	fileout << "]\n";

	fileout << "],\n";
	fileout << "\"type\": \"ComplexItem\",\n";
	fileout << "\"version\": 3\n";
	fileout << "},\n";
	//--------------------------------------------------------------
	//print other coordinates
	int size = _waypoints.size();
	for (int i = 0; i < size; i++) {
		fileout << "{\n";
		fileout << "\"autoContinue\": true,\n\"command\": 16,\n\"coordinate\": [\n";
		fileout << _waypoints.at(i).getLatitude() << ",\n";
		fileout << _waypoints.at(i).getLongitude() << ",\n";
		fileout << _waypoints.at(i).getAltitude() << "\n";
		fileout << "],\n";
		fileout << "\"doJumpId\": " << i + 2 << ",\n";
		fileout << "\"frame\": 3,\n";
		fileout << "\"params\": [\n0,\n0,\n0,\n0\n],\n";
		fileout << "\"type\": \"SimpleItem\"\n";
		if (i == (size - 1)) {
			fileout << "}\n";
		}
		else {
			fileout << "},\n";
		}
	}
	//--------------------------------------------------------------


	fileout << "],\n";
	fileout << "\"plannedHomePosition\": [\n";
	fileout << home.getLatitude() << ",\n";
	fileout << home.getLongitude() << ",\n";
	fileout << home.getAltitude() << "\n";
	fileout << "],\n";
	fileout << "\"version\": 2\n";
	fileout << "}";
	return true;
}



