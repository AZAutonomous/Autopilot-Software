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

std::vector<Coordinate> Path::getSVector() const
{
	return this->_searchArea;
}

std::vector<Coordinate> Path::getWVector() const
{
	return this->_waypoints;
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

				minutes = temp.substr(2, 3);
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

void Path::CreateSearchWaypoints(double ViewRadius){
	int searchsize = 0;
	searchsize = _searchArea.size();
	double tempalt = 0.0;
	double templat = 0.0;
	double templon = 0.0;
	//get a coordinate that is in the center of the polygon
	for (int i = 0; i < searchsize; i++) {
		templon = templon + _searchArea.at(i).getLongitude();
		templat = templat + _searchArea.at(i).getLatitude();
		tempalt = tempalt + _searchArea.at(i).getAltitude();
	}
	Coordinate centerPoint;
	centerPoint.setAltitude(tempalt / searchsize);
	centerPoint.setLatitude(templat / searchsize);
	centerPoint.setLongitude(templon / searchsize);
	//shrink the search size by radius
	for (int j = 0; j < searchsize; j++) {
		Shrinkvector(&_searchArea.at(j), ViewRadius, centerPoint);
	}
	//create the searcharea vectors
	for (int k = 0; k < searchsize; k++) {
		if (k == searchsize-1) {
			CreatePirimeterVectors(_searchArea.at(k), _searchArea.at(0), ViewRadius, 100);
		}
		else {
			CreatePirimeterVectors(_searchArea.at(k), _searchArea.at(k + 1), ViewRadius, 100);
		}
	}
	//sort the searcharea veco
	//sort(0, _searchArea.size());
}

void Path::sort(int i, int k) {
	int j = 0;
	if (i >= k) {
		return;
	}
	j = partition(i, k);

	sort(i, j);
	sort(j + 1, k);
	return;
}
int Path::partition(int i, int k) {
	int l = 0;
	int h = 0;
	int mid = 0;
	double pivot = 0.0;
	Coordinate temp = Coordinate();
	bool done = false;
	int size = i + (k - i);//number of inputs
						   //vector fucntions look up begin(), end(), at(), size()
	mid = i + (k - i) / 2;
	pivot = _searchArea.at(mid).getLongitude();
	l = i;
	h = k;

	while (done != true) {
		while(_searchArea.at(l).getLongitude() < pivot){
			++l;
		}
		while(pivot < _searchArea.at(h).getLongitude()){
			--h;
		}
		if (l >= h) {
			done = true;
		}
		else {//swap the h and l time stamps
			temp = _searchArea.at(l);
			_searchArea.at(l) = _searchArea.at(h);
			_searchArea.at(h) = temp;
			++l;
			--h;
		}
	}
	return h;
}
void Path::CreatTestSearchPoints()
{
	Coordinate newpoint[100];
	for (int i = 0; i < 5; i++) {
		newpoint[i].setName("name");
		newpoint[i].setAltitude(100);
	}
	newpoint[0].setID("0");
	newpoint[1].setID("1");
	newpoint[2].setID("2");
	newpoint[3].setID("3");
	newpoint[4].setID("4");

	newpoint[0].setLongitude(0.2);
	newpoint[0].setLatitude(0.1);
	newpoint[1].setLongitude(0.4);
	newpoint[1].setLatitude(0.1);
	newpoint[2].setLongitude(0.5);
	newpoint[2].setLatitude(0.4);
	newpoint[3].setLongitude(0.3);
	newpoint[3].setLatitude(0.5);
	newpoint[4].setLongitude(0.1);
	newpoint[4].setLatitude(0.4);

	for (int j = 0; j < 5; j++) {
		_searchArea.push_back(newpoint[j]);
	}
}

void Path::CreatePirimeterVectors(Coordinate input1,Coordinate input2, double ViewRadius, double Altitude){
	double theta;
	double lat = 0.0;
	double lon = 0.0;
	double H = 0.0;
	lat = abs(input1.getLatitude() - input2.getLatitude());
	lon = abs(input1.getLongitude() - input2.getLongitude());
	H = pow((pow(lat, 2) + pow(lon, 2)), 0.5) - ViewRadius;
	theta = atan(lat / lon);
	int n = ceil(lon / ViewRadius);
	double step = lon / n;
	for (int i = 1; i < n; i++) {
		//fix me
		Coordinate tempcoor;
		tempcoor.setAltitude(Altitude);
		if (input1.getLongitude() < input2.getLongitude()) {
			tempcoor.setLongitude(i*step + input1.getLongitude());
		}
		if (input1.getLongitude() > input2.getLongitude()) {
			tempcoor.setLongitude(input1.getLongitude() - i*step);
		}
		if (input1.getLatitude() < input2.getLatitude()) {
			tempcoor.setLatitude(input1.getLatitude() + i*step*tan(theta));
		}
		if (input1.getLatitude() > input2.getLatitude()) {
			tempcoor.setLatitude(input1.getLatitude() - i*step*tan(theta));
		}
			_searchArea.push_back(tempcoor);
	}
}

void Path::Shrinkvector(Coordinate * inputCoordinate, double viewRadius, Coordinate centerCoordinate){
	double theta;
	double lat = 0;
	double lon = 0;
	lat = abs(inputCoordinate->getLatitude() - centerCoordinate.getLatitude());
	lon = abs(inputCoordinate->getLongitude() - centerCoordinate.getLongitude());
	theta = atan(lat / lon);
	//double newdist = pow((pow(lat, 2) + pow(lon, 2)), 0.5) - viewRadius;
	if (inputCoordinate->getLatitude() < centerCoordinate.getLatitude()) {
		inputCoordinate->setLatitude(viewRadius*sin(theta) + inputCoordinate->getLatitude());
	}
	if (inputCoordinate->getLatitude() > centerCoordinate.getLatitude()) {
		inputCoordinate->setLatitude(inputCoordinate->getLatitude() - viewRadius*sin(theta));
	}

	if (inputCoordinate->getLongitude() < centerCoordinate.getLongitude()) {
		inputCoordinate->setLongitude(viewRadius*cos(theta) + inputCoordinate->getLongitude());
	}
	if (inputCoordinate->getLongitude() > centerCoordinate.getLongitude()) {
		inputCoordinate->setLongitude(inputCoordinate->getLongitude() - viewRadius*cos(theta));
	}
}

double Path::CalculateCoordtoDec(double deg, double min, double sec){
	double decimal = 0.0;
	double degree;
	double minute;
	double second;

	decimal = deg + (min / 60.0) + (sec / 3600.0);

	return decimal;
}

double Path::GetDistance(double x1, double y1, double x2, double y2)
{
	double x = abs(x1 - x2);
	double y = abs(y1 - y2);
	double h = pow((x*x+y*y), 0.5);
	return h;
}

void Path::SwapSearchVectors(int size){

	int i = 1;
	double dist1;
	double dist2;
	double dist3;
	double dist4;
	double dist5;
	double theta1 = 0.0;
	double theta2 = 0.0;
	double theta3 = 0.0;
	double theta4 = 0.0;
	Coordinate tempCoor;
	while (i < size+1) {
		dist1 = GetDistance(_searchArea.at(i - 1).getLongitude(), _searchArea.at(i - 1).getLatitude(), _searchArea.at(i).getLongitude(), _searchArea.at(i).getLatitude());
		dist2 = GetDistance(_searchArea.at(i + 1).getLongitude(), _searchArea.at(i + 1).getLatitude(), _searchArea.at(i).getLongitude(), _searchArea.at(i).getLatitude());
		dist3 = GetDistance(_searchArea.at(i - 1).getLongitude(), _searchArea.at(i - 1).getLatitude(), _searchArea.at(i+1).getLongitude(), _searchArea.at(i+1).getLatitude());
		dist4 = GetDistance(_searchArea.at(i + 1).getLongitude(), _searchArea.at(i + 1).getLatitude(), _searchArea.at(i+2).getLongitude(), _searchArea.at(i+2).getLatitude());
		dist5 = GetDistance(_searchArea.at(i).getLongitude(), _searchArea.at(i).getLatitude(), _searchArea.at(i+2).getLongitude(), _searchArea.at(i+2).getLatitude());
		theta1 = acos((dist1*dist1 + dist2*dist2 - dist3*dist3) / 2 * dist1*dist2);
		theta2 = acos((dist2*dist2 + dist4*dist4 - dist5*dist5) / 2 * dist4*dist2);
		theta3 = acos((dist2*dist2 + dist3*dist3 - dist1*dist1) / 2 * dist3*dist2);
		theta4 = acos((dist2*dist2 + dist5*dist5 - dist4*dist4) / 2 * dist5*dist2);
		if (theta3 + theta4 > theta1 + theta2) {//then swap i with i+1
			tempCoor = _searchArea.at(i);
			_searchArea.at(i) = _searchArea.at(i + 1);
			_searchArea.at(i + 1) = tempCoor;
		}
		i++;
	}
	return;
}

void Path::PushtoWaypoints(int Searchsize, int Waypointsize)
{
	Coordinate tempcoor;
	for (int i = 0; i < Searchsize; i++) {
		string ID;
		stringstream ss;
		tempcoor = _searchArea.at(i);
		ss << Waypointsize;
		ID = ss.str();
		tempcoor.setID(ID);
		_waypoints.push_back(tempcoor);
		Waypointsize++;
	}
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


