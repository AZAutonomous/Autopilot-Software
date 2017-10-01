#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "obstacle_list.h"

using namespace std;


std::vector<Obstacle> createObstacles(string filename)
{
	std::vector<Obstacle> obstacles;
	ifstream inFile;
	string buffer;
	int id = 1;

	inFile.open(filename);
	if (!inFile.good()) {
		cout << "Could not open input file " << filename << endl;
		return obstacles;
	}
	while (getline(inFile, buffer)) {
		istringstream ISS(buffer);
		string rad;
		string lat;
		string lon;
		Obstacle tempO;

		ISS >> lat;
		ISS >> lon;
		ISS >> rad;

		tempO.set_latitude(stod(lat));
		tempO.set_longitude(stod(lon));
		tempO.set_radius(stod(rad));
		tempO.set_id(id);
		id++;

		obstacles.push_back(tempO);
	}
	return obstacles;
}
