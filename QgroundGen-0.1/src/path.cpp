#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include "path.h"
#include "coordinate.h"
#include "pair.h"

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

std::vector<Coordinate> Path::getOVector () const
{
	return this->_opArea;
}

void Path::setSVector(std::vector<Coordinate> S)
{
	this->_searchArea = S;
}

void Path::setWVector(std::vector<Coordinate> V)
{
	this->_waypoints = V;
}

void Path::setOVector (std::vector<Coordinate> O)
{
	this->_opArea = O;
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
	int size = getWVector().size();
	int i = 0;
	cout << endl;
	std::ofstream myfile;
	myfile.open(filename);
	myfile << "{\n";
	myfile << "\"MAV_AUTOPILOT\": 3,\n";
	myfile << "\"complexItems\": [],\n";
	myfile << "\"groundStation\": \"QGroundControl\",\n";
	myfile << "\"items\": [\n";
	/////first take off location
	myfile << "{\n";
	myfile << "\"autoContinue\": true,\n";
	myfile << "\"command\": 22,\n";
	myfile << "\"coordinate\": [";
	myfile << getWVector().at(i).getLatitude() << "," << getWVector().at(i).getLongitude() << "," << getWVector().at(i).getAltitude() << "],\n";
	myfile << "\"frame\": 2,\n";
	myfile << "\"id\": ";
	myfile << (i + 1) << ",\n";
	myfile << "\"param1\": 10,\n";
	myfile << "\"param2\": 0,\n";
	myfile << "\"param3\": 0,\n";
	myfile << "\"param4\": 0,\n";
	myfile << "\"type\": \"missionItem\"\n";
	myfile << "},\n";
	//end of first take off comand
	i++;
	//prints waypoints of normal flight
	while (i < size - 1) {
		myfile << "{\n";
		myfile << "\"autoContinue\": true,\n";
		myfile << "\"command\": 16,\n";
		myfile << "\"coordinate\": [";
		myfile << getWVector().at(i).getLatitude() << "," << getWVector().at(i).getLongitude() << "," << getWVector().at(i).getAltitude() << "],\n";
		myfile << "\"frame\": 3,\n";
		myfile << "\"id\": ";
		myfile << (i + 1) << ",\n";
		myfile << "\"param1\": 0,\n";
		myfile << "\"param2\": 0,\n";
		myfile << "\"param3\": 0,\n";
		myfile << "\"param4\": 0,\n";
		myfile << "\"type\": \"missionItem\"\n";
		myfile << "},\n";
		i++;
	}
	//printf landing location
	myfile << "{\n";
	myfile << "\"autoContinue\": true,\n";
	myfile << "\"command\": 21,\n";
	myfile << "\"coordinate\": [";
	myfile << getWVector().at(i).getLatitude() << "," << getWVector().at(i).getLongitude() << "," << getWVector().at(i).getAltitude() << "],\n";
	myfile << "\"frame\": 3,\n";
	myfile << "\"id\": ";
	myfile << (i + 1) << ",\n";
	myfile << "\"param1\": 10,\n";
	myfile << "\"param2\": 0,\n";
	myfile << "\"param3\": 0,\n";
	myfile << "\"param4\": 0,\n";
	myfile << "\"type\": \"missionItem\"\n";
	myfile << "},\n";

	///////////////////////////////////////////////

	myfile << "],\n";
	//print planned home position
	myfile << "\"plannedHomePosition\": {\n";
	myfile << "\"autoContinue\": true,\n";
	myfile << "\"command\": 16,\n";
	myfile << "\"coordinate\": [";
	myfile << getWVector().at(i).getLatitude() << "," << getWVector().at(i).getLongitude() << "," << getWVector().at(i).getAltitude() << "],\n";
	myfile << "\"frame\": 3,\n";
	myfile << "\"id\": ";
	myfile << (i + 1) << ",\n";
	myfile << "\"param1\": 0,\n";
	myfile << "\"param2\": 0,\n";
	myfile << "\"param3\": 0,\n";
	myfile << "\"param4\": 0,\n";
	myfile << "\"type\": \"missionItem\"\n";
	myfile << "},\n";

	//////////////////////////////////

	myfile << "\"version\": \"1.0\"\n";

	myfile << "}";
	myfile.close();

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
	//for (int j = 0; j < searchsize; j++) {
		//Shrinkvector(&_searchArea.at(j), ViewRadius, centerPoint);
	//}
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
		if (input1.getLongitude() <= input2.getLongitude()) {
			tempcoor.setLongitude(i*step + input1.getLongitude());
		}
		if (input1.getLongitude() > input2.getLongitude()) {
			tempcoor.setLongitude(input1.getLongitude() - i*step);
		}
		if (input1.getLatitude() <= input2.getLatitude()) {
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

void Path::TessellatePath(double turnRad, double viewRad){
	if (CheckTes(turnRad) == true) {
		return;
	}
	Coordinate startEdge = FindStartEdge(viewRad);
	Coordinate endEdge = FindEndEdge(viewRad);
	int tescase = 0;
	if (startEdge.getAltitude() != -1.0) {
		tescase = tescase + 2;
	}
	if (endEdge.getAltitude() != -1.0) {
		tescase = tescase + 1;
	}
	//create a new temp vector
	int size = _searchArea.size();
	std::vector<Pair> tempPairVector;
	for (int i = 0; i < size; i = i + 2) {
		Pair tempPair;
		tempPair.setFirst(_searchArea.at(i));
		tempPair.setLast(_searchArea.at(i + 1));
		tempPairVector.push_back(tempPair);
	}
	//set the waypoint vector by calling funtion to re-order vector
	setSVector(CreateTes(tempPairVector, tescase,startEdge,endEdge));
	//get the location of the edge node
	int loc = 0;
	for (int l = 0; l < size; l++) {
		if (_searchArea.at(l).getLatitude() == endEdge.getLatitude() && _searchArea.at(l).getLongitude() == endEdge.getLongitude() && _searchArea.at(l).getAltitude() == endEdge.getAltitude()) {
			loc = l;
			break;
		}
	}
	//insert tear drop node after end edge node 
	AddTearDropNode(loc, turnRad);

}

std::vector<Coordinate> Path::CreateTes(std::vector<Pair> PairVector, int Tcase,Coordinate start,Coordinate end)
{
	std::vector<Coordinate> Waypoints;
	std::vector<Pair> evens;
	std::vector<Pair> odds;
	int Vsize = PairVector.size();
	int F = 0;
	//create 2 vectors of the evens and odds of the input vector
	for (int i = 0; i < Vsize; i++) {
		if (i % 2 == 0) {
			odds.push_back(PairVector.at(i));
		}
		else {
			evens.push_back(PairVector.at(i));
		}
	}
	int evensize = evens.size();
	int oddsize = odds.size();
	//add the evens if they start
	if (Tcase == 2 || Tcase == 3) {
		Waypoints.push_back(start);
		for (int j = 0; j < evensize; j++) {
			if(j%2==0){
				Waypoints.push_back(evens.at(j).getFirst());
				Waypoints.push_back(evens.at(j).getLast());
				F = 0;
			}
			else {
				Waypoints.push_back(evens.at(j).getLast());
				Waypoints.push_back(evens.at(j).getFirst());
				F = 1;
			}
		}
	}
	//add the odds if they start
	if (Tcase == 0 || Tcase == 1) {
		for (int j = 0; j < oddsize; j++) {
			if (j % 2 == 0) {
				Waypoints.push_back(odds.at(j).getFirst());
				Waypoints.push_back(odds.at(j).getLast());
				F = 0;
			}
			else {
				Waypoints.push_back(odds.at(j).getLast());
				Waypoints.push_back(odds.at(j).getFirst());
				F = 1;
			}
		}
	}
	//add end edge if needed
	if (Tcase == 1 || Tcase == 3) {
		Waypoints.push_back(end);
	}
	//add evens backwards if they go
	if (Tcase == 0 || Tcase == 1) {
		for (int k = evensize - 1; k >= 0; k--) {
			if (F == 1) {
				Waypoints.push_back(evens.at(k).getFirst());
				Waypoints.push_back(evens.at(k).getLast());
				F = 0;
			}
			else {
				Waypoints.push_back(evens.at(k).getLast());
				Waypoints.push_back(evens.at(k).getFirst());
				F = 1;
			}
		}
	}
	//add odds in reverse if they go next
	if (Tcase == 2 || Tcase == 3) {
		for (int k = oddsize - 1; k >= 0; k--) {
			if (F == 1) {
				Waypoints.push_back(odds.at(k).getFirst());
				Waypoints.push_back(odds.at(k).getLast());
				F = 0;
			}
			else {
				Waypoints.push_back(odds.at(k).getLast());
				Waypoints.push_back(odds.at(k).getFirst());
				F = 1;
			}
		}
	}
	return Waypoints;
}

void Path::AddTearDropNode(int loc, double turnRad)
{	
	turnRad = turnRad * 2;
	Coordinate tempCoor;
	tempCoor.setLatitude((_searchArea.at(loc).getLatitude() + _searchArea.at(loc+1).getLatitude()) / 2);
	tempCoor.setLongitude((_searchArea.at(loc).getLongitude() + _searchArea.at(loc + 1).getLongitude()) / 2);
	tempCoor.setAltitude(_searchArea.at(loc).getAltitude());
	double x = tempCoor.getLongitude() - _searchArea.at(loc).getLongitude();
	double y = tempCoor.getLatitude() - _searchArea.at(loc).getLatitude();
	double L = GetDistance(_searchArea.at(loc + 1).getLongitude(), _searchArea.at(loc + 1).getLatitude(), tempCoor.getLongitude(), tempCoor.getLatitude());
	double y2 = (turnRad / L)*x;
	double x2 = (turnRad / L)*y;
	if (x > 0 && y > 0 || x < 0 && y < 0) {
		x2 = -1 * x2;
	}
	Coordinate test1;
	test1.setLongitude(tempCoor.getLongitude() + x2);
	test1.setLatitude(tempCoor.getLatitude() + y2);
	Coordinate test2;
	test2.setLongitude(tempCoor.getLongitude() - x2);
	test2.setLatitude(tempCoor.getLatitude() - y2);
	double theta1;
	double theta2;
	//calculate 2 thetas and use test with larger theta
	theta1 = CalculateTheta(_searchArea.at(loc - 1), _searchArea.at(loc), test1);
	theta2 = CalculateTheta(_searchArea.at(loc - 1), _searchArea.at(loc), test2);
	//this fixes that big problem i think?
	if (x > 0 && y > 0 || x < 0 && y < 0) {
		theta1 = 3.14159 - theta1;
		theta2 = 3.14159 - theta2;
	}
	//insert the bigger angle
	if (theta1 >= theta2) {
		_searchArea.insert(_searchArea.begin() + loc + 1, test1);
	}
	else {
		_searchArea.insert(_searchArea.begin() + loc + 1, test2);
	}
}

bool Path::CheckTes(double turnRad)
{
	int size = _searchArea.size();
	for (int i = 0; i < size - 1; i++) {//check if need to tessellate
		double tempdist = 0.0;
		tempdist = GetDistance(_searchArea.at(i).getLongitude(), _searchArea.at(i).getLatitude(), _searchArea.at(i + 1).getLongitude(), _searchArea.at(i + 1).getLatitude());
		if (tempdist < 2 * turnRad) {
			return false;
		}
	}
	return true;
}

double Path::CalculateTheta(Coordinate C1, Coordinate C2, Coordinate C3)
{
	double x1 = C2.getLongitude() - C1.getLongitude();
	double y1 = C2.getLatitude() - C1.getLatitude();
	double x2 = C3.getLongitude() - C2.getLongitude();
	double y2 = C3.getLatitude() - C2.getLatitude();
	double dist1 = GetDistance(C1.getLongitude(), C1.getLatitude(), C2.getLongitude(), C2.getLatitude());
	double dist2 = GetDistance(C2.getLongitude(), C2.getLatitude(), C3.getLongitude(), C3.getLatitude());
	double theta = acos(((x1*x2) + (y1*y2)) / (dist1*dist2));
	return theta;
		
}

Coordinate Path::FindStartEdge(double viewRad)
{
	viewRad = 0.95*viewRad;
	Coordinate temp;
	temp.setAltitude(-1.0);
	double delta = 0.0;
	double lon1 = _searchArea.at(0).getLongitude();
	double lon2 = _searchArea.at(1).getLongitude();
	delta = abs(lon1 - lon2);
	if (delta >= viewRad) {
		temp = _searchArea.at(0);
		_searchArea.erase(_searchArea.begin());
		return temp;
	}
	else {
		return temp;
	}
}

Coordinate Path::FindEndEdge(double viewRad)
{
	viewRad = 0.95*viewRad;
	int size = _searchArea.size();
	Coordinate temp;
	temp.setAltitude(-1.0);
	double delta = 0.0;
	delta = abs(_searchArea.at(size-1).getLongitude() - _searchArea.at(size-2).getLongitude());
	if (delta >= viewRad) {
		temp = _searchArea.at(size-1);
		_searchArea.pop_back();
		return temp;
	}
	else {
		return temp;
	}
}

void Path::SwapSearchVectors(int size, double ViewRadius){
	ViewRadius = (0 - ViewRadius)/2.0;
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
	double Lonchange = 0.0;
	Coordinate tempCoor;
	while (i < size-2) {
		dist1 = GetDistance(_searchArea.at(i - 1).getLongitude(), _searchArea.at(i - 1).getLatitude(), _searchArea.at(i).getLongitude(), _searchArea.at(i).getLatitude());
		dist2 = GetDistance(_searchArea.at(i + 1).getLongitude(), _searchArea.at(i + 1).getLatitude(), _searchArea.at(i).getLongitude(), _searchArea.at(i).getLatitude());
		dist3 = GetDistance(_searchArea.at(i - 1).getLongitude(), _searchArea.at(i - 1).getLatitude(), _searchArea.at(i+1).getLongitude(), _searchArea.at(i+1).getLatitude());
		dist4 = GetDistance(_searchArea.at(i + 1).getLongitude(), _searchArea.at(i + 1).getLatitude(), _searchArea.at(i+2).getLongitude(), _searchArea.at(i+2).getLatitude());
		dist5 = GetDistance(_searchArea.at(i).getLongitude(), _searchArea.at(i).getLatitude(), _searchArea.at(i+2).getLongitude(), _searchArea.at(i+2).getLatitude());
		theta1 = acos(((dist1*dist1) + (dist2*dist2) - (dist3*dist3)) / (2 * dist1*dist2));
		theta2 = acos(((dist2*dist2) + (dist4*dist4) - (dist5*dist5)) / (2 * dist4*dist2));
		theta3 = acos(((dist2*dist2) + (dist3*dist3) - (dist1*dist1)) / (2 * dist3*dist2));
		theta4 = acos(((dist2*dist2) + (dist5*dist5) - (dist4*dist4)) / (2 * dist5*dist2));
		Lonchange = _searchArea.at(i + 1).getLongitude() - _searchArea.at(i + 2).getLongitude();
		if (theta3 + theta4 > theta1 + theta2 && Lonchange > ViewRadius) {
			tempCoor = _searchArea.at(i+1);
			_searchArea.at(i+1) = _searchArea.at(i + 2);
			_searchArea.at(i + 2) = tempCoor;
			i++;
		}
		i ++;
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

void Path::SetVectorIds(){
	int size = getWVector().size();
	for (int i = 0; i < size-1; i++) {
		string ID;
		stringstream ss;
		ss << i+1;
		ID = ss.str();
		_waypoints.at(i).setID(ID);
	}
}
//------------------------------------------------------------------------------------------------


