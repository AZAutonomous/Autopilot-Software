#include "source.h"
#include "path.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

bool Source::ReadFromFile(string filename, Path* path, double SearchAlt){
	ifstream inFile;
	string buffer;
	string temp;
	Coordinate coord;
	string Point, Name, Name2, Latitude, Longitude, Altitude;
	string DD, MM, SS;
	double degs, mins, secs;
	double lat, lon, alt;
	int flag = 0;

	//Vectors to set for path
	std::vector<Coordinate> waypoints;
	std::vector<Coordinate> searchArea;
	std::vector<Coordinate> opArea;
	
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
			alt = stod(Altitude);
		}
		//add coordinate points--------------------------------------------------------------------
		
		newCoor.setLatitude(lat);
		newCoor.setLongitude(-1*lon);
		if (flag == 3) {
			int size = waypoints.size () + 1;
			newCoor.setID(to_string(size));
			waypoints.push_back (newCoor);
		}
		else if (flag == 2) {
			searchArea.push_back (newCoor);
		}
		else if (flag == 1) {
			opArea.push_back (newCoor);
		}

	}

	//Set the path vectors
	path->setWVector (waypoints);
	path->setSVector (searchArea);
	path->setOVector (opArea);

	return true;
}

double Source::CalculateCoordtoDec(double deg, double min, double sec)
{
	double decimal = 0.0;
	decimal = deg + (min / 60.0) + (sec / 3600.0);
	return decimal;
}

bool Source::WriteToFile(std::string filename,Coordinate home,Coordinate takeoff,Coordinate landway, Coordinate takeoffWay,Path path)
{
	ofstream fileout;
	fileout.open(filename);
	if (!fileout.good()) {
		return false;
	}
	fileout << "{\n";
	fileout << "\"firmwareType\": 12,\n\"groundStation\" : \"QGroundControl\",\n\"items\" : [\n";
	//print take off

	fileout << "{\n";
	fileout << "\"autoContinue\": true,\n\"command\": 22,\n\"coordinate\": [\n";
	fileout << to_string (takeoff.getLatitude()) << ",\n";
	fileout << to_string (takeoff.getLongitude()) << ",\n";
	fileout << to_string (takeoff.getAltitude()) << "\n";
	fileout << "],\n";
	fileout << "\"doJumpId\": " << "1" << ",\n";
	fileout << "\"frame\": 3,\n";
	fileout << "\"params\": [\n15,\n0,\n0,\n0\n],\n";
	fileout << "\"type\": \"SimpleItem\"\n";
	fileout << "},\n";
	//print take off way
	fileout << "{\n";
	fileout << "\"autoContinue\": true,\n\"command\": 16,\n\"coordinate\": [\n";
	fileout << to_string (takeoffWay.getLatitude()) << ",\n";
	fileout << to_string (takeoffWay.getLongitude()) << ",\n";
	fileout << to_string (takeoffWay.getAltitude()) << "\n";
	fileout << "],\n";
	fileout << "\"doJumpId\": " << "1" << ",\n";
	fileout << "\"frame\": 3,\n";
	fileout << "\"params\": [\n15,\n0,\n0,\n0\n],\n";
	fileout << "\"type\": \"SimpleItem\"\n";
	fileout << "},\n";
	//print other coordinates
	int size = path.getWVector().size();
	//size = 2;
	for (int i = 0; i < size; i++) {
		fileout << "{\n";
		fileout << "\"autoContinue\": true,\n\"command\": 16,\n\"coordinate\": [\n";
		fileout << to_string (path.getWVector().at(i).getLatitude()) << ",\n";
		fileout << to_string (path.getWVector().at(i).getLongitude()) << ",\n";
		fileout << to_string (path.getWVector().at(i).getAltitude()) << "\n";
		fileout << "],\n";
		fileout << "\"doJumpId\": " << i+2 << ",\n";
		fileout << "\"frame\": 3,\n";
		fileout << "\"params\": [\n0,\n0,\n0,\n0\n],\n";
		fileout << "\"type\": \"SimpleItem\"\n";
		fileout << "},\n";
	}
	
	
	//--------------------------------------------------------------
	//print pre land
	fileout << "{\n";
	fileout << "\"autoContinue\": true,\n\"command\": 16,\n\"coordinate\": [\n";
	fileout << to_string (landway.getLatitude()) << ",\n";
	fileout << to_string (landway.getLongitude()) << ",\n";
	fileout << to_string (landway.getAltitude()) << "\n";
	fileout << "],\n";
	fileout << "\"doJumpId\": " << "1" << ",\n";
	fileout << "\"frame\": 3,\n";
	fileout << "\"params\": [\n15,\n0,\n0,\n0\n],\n";
	fileout << "\"type\": \"SimpleItem\"\n";
	fileout << "},\n";
	//print land
	fileout << "{\n";
	fileout << "\"autoContinue\": true,\n\"command\": 21,\n\"coordinate\": [\n";
	fileout << to_string (home.getLatitude()) << ",\n";
	fileout << to_string (home.getLongitude()) << ",\n";
	fileout << to_string (home.getAltitude()) << "\n";
	fileout << "],\n";
	fileout << "\"doJumpId\": " << "1" << ",\n";
	fileout << "\"frame\": 3,\n";
	fileout << "\"params\": [\n15,\n0,\n0,\n0\n],\n";
	fileout << "\"type\": \"SimpleItem\"\n";
	fileout << "}\n";
	//print the home pos
	fileout << "],\n";
	fileout << "\"plannedHomePosition\": [\n";
	fileout << to_string (home.getLatitude()) << ",\n";
	fileout << to_string (home.getLongitude()) << ",\n";
	fileout << to_string (home.getAltitude()) << "\n";
	fileout << "],\n";
	fileout << "\"version\": 2\n";
	fileout << "}";
	return true;
}

bool Source::WriteToFile (char* filename, Coordinate home, Coordinate takeoff, Coordinate descent, Coordinate ascent, Path path)
{
	FILE* output = fopen (filename, "w");
	if (output == NULL)
		return false;

	fprintf (output, "{\n");
	fprintf (output, "\"firmwareType\": 12,\n\"groundStation\" : \"QGroundControl\",\n\"items\" : [\n");
	fprintf (output, "{\n");
	fprintf (output, "\"autoContinue\": true,\n\"command\": 22,\n\"coordinate\": [\n");
	fprintf (output, "%lf,\n", takeoff.getLatitude ());
	fprintf (output, "%lf,\n", takeoff.getLongitude ());
	fprintf (output, "%lf\n", takeoff.getAltitude ());
	fprintf (output, "],\n");
	fprintf (output, "\"doJumpId\": 1,\n");
	fprintf (output, "\"frame\": 3,\n");
	fprintf (output, "\"params\": [\n15,\n0,\n0,\n0\n],\n");
	fprintf (output, "\"type\": \"SimpleItem\"\n");
	fprintf (output, "},\n");

	fprintf (output, "{\n");
	fprintf (output, "\"autoContinue\": true,\n\"command\": 16,\n\"coordinate\": [\n");
	fprintf (output, "%lf,\n", ascent.getLatitude ());
	fprintf (output, "%lf,\n", ascent.getLongitude ());
	fprintf (output, "%lf\n", ascent.getAltitude ());
	fprintf (output, "],\n");
	fprintf (output, "\"doJumpId\": 1,\n");
	fprintf (output, "\"frame\": 3,\n");
	fprintf (output, "\"params\": [\n15,\n0,\n0,\n0\n],\n");
	fprintf (output, "\"type\": \"SimpleItem\"\n");
	fprintf (output, "},\n");
	int size = path.getWVector ().size ();
	for (int i = 0; i < size; i++)
	{
		fprintf (output, "{\n");
		fprintf (output, "\"autoContinue\": true,\n\"command\": 16,\n\"coordinate\": [\n");

		fprintf (output, "%lf,\n", path.getWVector ().at (i).getLatitude ());
		fprintf (output, "%lf,\n", path.getWVector ().at (i).getLongitude ());
		fprintf (output, "%lf\n", path.getWVector ().at (i).getAltitude ());

		fprintf (output, "],\n");
		fprintf (output, "\"doJumpId\": %d,\n", i + 2);
		fprintf (output, "\"frame\": 3,\n");
		fprintf (output, "\"params\": [\n0,\n0,\n0,\n0\n],\n");
		fprintf (output, "\"type\": \"SimpleItem\"\n");
		fprintf (output, "},\n");
	}
	fprintf (output, "{\n");
	fprintf (output, "\"autoContinue\": true,\n\"command\": 16,\n\"coordinate\": [\n");

	fprintf (output, "%lf,\n", descent.getLatitude ());
	fprintf (output, "%lf,\n", descent.getLongitude ());
	fprintf (output, "%lf\n", descent.getAltitude ());
	
	fprintf (output, "],\n");
	fprintf (output, "\"doJumpId\": 1,\n");
	fprintf (output, "\"frame\": 3,\n");
	fprintf (output, "\"params\": [\n15,\n0,\n0,\n0\n],\n");
	fprintf (output, "\"type\": \"SimpleItem\"\n");
	fprintf (output, "},\n");

	fprintf (output, "{\n");
	fprintf (output, "\"autoContinue\": true,\n\"command\": 21,\n\"coordinate\": [\n");

	fprintf (output, "%lf,\n", home.getLatitude ());
	fprintf (output, "%lf,\n", home.getLongitude ());
	fprintf (output, "%lf\n", home.getAltitude ());

	fprintf (output, "],\n");
	fprintf (output, "\"doJumpId\": 1,\n");
	fprintf (output, "\"frame\": 3,\n");
	fprintf (output, "\"params\": [\n15,\n0,\n0,\n0\n],\n");
	fprintf (output, "\"type\": \"SimpleItem\"\n");
	fprintf (output, "}\n");

	fprintf (output, "],\n");
	fprintf (output, "\"plannedHomePosition\": [\n");

	fprintf (output, "%lf,\n", home.getLatitude ());
	fprintf (output, "%lf,\n", home.getLongitude ());
	fprintf (output, "%lf\n", home.getAltitude ());

	fprintf (output, "],\n");
	fprintf (output, "\"version\": 2\n");
	fprintf (output, "}");

	return true;
}