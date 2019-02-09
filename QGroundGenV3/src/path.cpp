#define _USE_MATH_DEFINES
#include "path.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

Path::Path() {}

bool Path::ReadFromFile(string file_path, double search_alt)
{
	ifstream in;
	string buffer;
	Coordinate coord;
	string point, name, name2, latitude, longitude, altitude;
	string dd, mm, ss;
	double deg, min, sec;
	double lat, lon;
	int flag = 0;

	in.open(file_path);
	if (!in.is_open())
	{
		cout << "Could not open input file " << file_path << endl;
		return false;
	}

	while (getline(in, buffer))
	{
		istringstream ISS(buffer);
		ISS >> point >> name >> name2 >> latitude >> longitude >> altitude;

		if (point.compare(0, 1, "A") == 0)
			flag = 1;
		else if (point.compare(0, 1, "S") == 0)
			flag = 2;
		else if (point.compare(0, 1, "W") == 0)
			flag = 3;
		else
			return false;

		lon = stod(longitude);
		lat = stod(latitude);

		coord.x = lon;
		coord.y = lat;

		if (flag == 3)
			_waypoints.push_back(coord);
		else if (flag == 2)
			_search_corners.push_back(coord);
		else if (flag == 1)
			_op_area_corners.push_back(coord);
	}
	return true;
}

bool Path::WriteToFile(string file_path, Coordinate home, Coordinate takeoff, Coordinate descent, Coordinate ascent)
{
	ofstream fileout;
	fileout.open(file_path);
	if (!fileout.is_open()) {
		return false;
	}
	fileout << setprecision(10);
	fileout << "{\n";
	fileout << "\"firmwareType\": 12,\n\"groundStation\" : \"QGroundControl\",\n\"items\" : [\n";
	//print take off

	fileout << "{\n";
	fileout << "\"autoContinue\": true,\n\"command\": 22,\n\"coordinate\": [\n";
	fileout << takeoff.y << ",\n";
	fileout << takeoff.x << ",\n";
	fileout << takeoff.z << "\n";
	fileout << "],\n";
	fileout << "\"doJumpId\": " << "1" << ",\n";
	fileout << "\"frame\": 3,\n";
	fileout << "\"params\": [\n15,\n0,\n0,\n0\n],\n";
	fileout << "\"type\": \"SimpleItem\"\n";
	fileout << "},\n";
	//print take off way
	fileout << "{\n";
	fileout << "\"autoContinue\": true,\n\"command\": 16,\n\"coordinate\": [\n";
	fileout << ascent.y << ",\n";
	fileout << ascent.x << ",\n";
	fileout << ascent.z << "\n";
	fileout << "],\n";
	fileout << "\"doJumpId\": " << "1" << ",\n";
	fileout << "\"frame\": 3,\n";
	fileout << "\"params\": [\n15,\n0,\n0,\n0\n],\n";
	fileout << "\"type\": \"SimpleItem\"\n";
	fileout << "},\n";
	//print other coordinates
	int size = _waypoints.size();
	//size = 2;
	for (int i = 0; i < size; i++) {
		fileout << "{\n";
		fileout << "\"autoContinue\": true,\n\"command\": 16,\n\"coordinate\": [\n";
		fileout << _waypoints[i].y << ",\n";
		fileout << _waypoints[i].x << ",\n";
		fileout << _waypoints[i].z << "\n";
		fileout << "],\n";
		fileout << "\"doJumpId\": " << i + 2 << ",\n";
		fileout << "\"frame\": 3,\n";
		fileout << "\"params\": [\n0,\n0,\n0,\n0\n],\n";
		fileout << "\"type\": \"SimpleItem\"\n";
		fileout << "},\n";
	}
	//--------------------------------------------------------------
	//print pre land
	fileout << "{\n";
	fileout << "\"autoContinue\": true,\n\"command\": 16,\n\"coordinate\": [\n";
	fileout << descent.y << ",\n";
	fileout << descent.x << ",\n";
	fileout << descent.z << "\n";
	fileout << "],\n";
	fileout << "\"doJumpId\": " << "1" << ",\n";
	fileout << "\"frame\": 3,\n";
	fileout << "\"params\": [\n15,\n0,\n0,\n0\n],\n";
	fileout << "\"type\": \"SimpleItem\"\n";
	fileout << "},\n";
	//print land
	fileout << "{\n";
	fileout << "\"autoContinue\": true,\n\"command\": 21,\n\"coordinate\": [\n";
	fileout << home.y << ",\n";
	fileout << home.x << ",\n";
	fileout << home.z << "\n";
	fileout << "],\n";
	fileout << "\"doJumpId\": " << "1" << ",\n";
	fileout << "\"frame\": 3,\n";
	fileout << "\"params\": [\n15,\n0,\n0,\n0\n],\n";
	fileout << "\"type\": \"SimpleItem\"\n";
	fileout << "}\n";
	//print the home pos
	fileout << "],\n";
	fileout << "\"plannedHomePosition\": [\n";
	fileout << home.y << ",\n";
	fileout << home.x << ",\n";
	fileout << home.z << "\n";
	fileout << "],\n";
	fileout << "\"version\": 2\n";
	fileout << "}";
	return true;
}

void Path::DefineNormalBoundingBox(double search_alt)
{
	if (_search_corners.empty())
		return;

	double west = _search_corners.front().x, east = _search_corners.front().x; //Holds the longitude for the western/eastern-most points
	double north = _search_corners.front().y, south = _search_corners.front().y; //Same as above for north and south

	for (unsigned int i = 0; i < _search_corners.size(); i++) //Find the western/eastern-most points, and northern/southern-most
	{
		if (_search_corners[i].x < west)
			west = _search_corners[i].x;
		else if (_search_corners[i].x > east)
			east = _search_corners[i].x;

		if (_search_corners[i].y > north)
			north = _search_corners[i].y;
		else if (_search_corners[i].y < south)
			south = _search_corners[i].y;
	}

	Coordinate corners[4]; //Corners start at northwest and go clockwise
	corners[0].x = west;
	corners[0].y = north;
	corners[0].z = search_alt;
	corners[1].x = east;
	corners[1].y = north;
	corners[1].z = search_alt;
	corners[2].x = east;
	corners[2].y = south;
	corners[2].z = search_alt;
	corners[3].x = west;
	corners[3].y = south;
	corners[3].z = search_alt;

	_bounding_box.push_back(corners[0]);
	_bounding_box.push_back(corners[1]);
	_bounding_box.push_back(corners[2]);
	_bounding_box.push_back(corners[3]);
}

//The function almost works. The bounding box doesn't have 90 degree interior angles as it should
void Path::DefineBoundingBox(double search_alt)
{
	double max_distance = 0.0;
	Vector edge_vector;
	Line longest_edge_line;
	int coord1_index = 0, coord2_index = 1;
	double perpendicular_slope; //Slope of any line perpendicular to the longest edge
	Coordinate farthest_point, intersect_point;

	for (unsigned int i = 0; i < _search_corners.size(); i++) //Find the longest edge of search area
	{
		edge_vector.setX(_search_corners[(i + 1) % _search_corners.size()].x - _search_corners[i].x);
		edge_vector.setY(_search_corners[(i + 1) % _search_corners.size()].y - _search_corners[i].y);
		if (edge_vector.getMagnitude() > max_distance)
		{
			Coordinate temp1(_search_corners[(i + 1) % _search_corners.size()].x, _search_corners[(i + 1) % _search_corners.size()].y, search_alt);
			Coordinate temp2(_search_corners[i].x, _search_corners[i].y, search_alt);
			coord1_index = i;
			coord2_index = (i + 1) % _search_corners.size();
			max_distance = edge_vector.getMagnitude();
			longest_edge_line = Line(temp1, temp2);
		}
	}

	perpendicular_slope = -1 / longest_edge_line.getSlope();
	max_distance = 0;

	for (int i = (coord2_index + 1) % _search_corners.size(); i != coord1_index; i = (i + 1) % _search_corners.size()) //Find the point farthest from longest edge
	{
		Line temp_line(_search_corners[i], perpendicular_slope);
		Coordinate temp;
		temp = FindSolution(longest_edge_line, temp_line);
		edge_vector.setX(temp.x - _search_corners[i].x);
		edge_vector.setY(temp.y - _search_corners[i].y);

		if (edge_vector.getMagnitude() > max_distance)
		{
			farthest_point = _search_corners[i];
			max_distance = edge_vector.getMagnitude();
			intersect_point = temp;
		}
	}

	_bounding_box.push_back(_search_corners[coord1_index]);
	_bounding_box.push_back(_search_corners[coord2_index]);

	//Find the vector that when added to the ends of the longest edge will extend the bounding box far enough to cover the farthest point
	Vector far_point_vector(intersect_point, farthest_point);
	Coordinate corner1(_bounding_box[0].x + far_point_vector.getX(), _bounding_box[0].y + far_point_vector.getY(), search_alt);
	Coordinate corner2(_bounding_box[1].x + far_point_vector.getX(), _bounding_box[1].y + far_point_vector.getY(), search_alt);
	_bounding_box.push_back(corner2);
	_bounding_box.push_back(corner1);
}

void Path::CreateNormalEdgeNodes(double view_radius)
{
	if (_bounding_box.empty())
		return;

	Coordinate temp_coord;
	const double kViewRadiusMult = 1.00; //Used to scale the view radius
	view_radius *= kViewRadiusMult;
	temp_coord.z = _bounding_box[0].z; //Set the altitude to be used
	//Determine the shortest edge
	Vector edge1(_bounding_box[0], _bounding_box[1]); //Vector along northern edge (same as southern edge)
	Vector edge2(_bounding_box[1], _bounding_box[2]); //Vector along eastern edge (same as western edge)
	Vector temp_vector;
	double distance_between_points;
	int num_points;

	//If northern/southern edge is smaller than the eastern/western edge or they are equal
	if (edge1.getMagnitude() < edge2.getMagnitude() || edge1.getMagnitude() == edge2.getMagnitude())
	{
		num_points = floor(edge1.getMagnitude() / view_radius) - 1;

		if (num_points == 0) //To avoid a divide by zero
			return;

		distance_between_points = edge1.getMagnitude() / num_points;
		_use_vertical = false;
		temp_coord.y = _bounding_box[0].y; //Since this is adding points to the northern edge, y is constant

		for (int i = 1; i < num_points; i++)
		{
			temp_coord.x = _bounding_box[0].x + (distance_between_points * i); //Adding points from left to right
			temp_vector.setX(_bounding_box[1].x - temp_coord.x);
			_bounding_box.push_back(temp_coord);
		}

		temp_coord.y = _bounding_box[3].y; //Now along southern edge, y is constant
		for (int i = 1; i < num_points; i++)
		{
			temp_coord.x = _bounding_box[3].x + (distance_between_points * i); //Adding points from left to right
			temp_vector.setX(_bounding_box[2].x - temp_coord.x);
			_bounding_box.push_back(temp_coord);
		}
	}
	else if (edge1.getMagnitude() > edge2.getMagnitude()) //If the eastern/western edge is smaller
	{
		num_points = floor(edge2.getMagnitude() / view_radius) - 1;

		if (num_points == 0)
			return;

		distance_between_points = edge2.getMagnitude() / num_points;
		_use_vertical = true;
		temp_coord.x = _bounding_box[1].x; //Work on the eastern edge first, holding x constant

		for (int i = 1; i < num_points; i++)
		{
			temp_coord.y = _bounding_box[1].y - (distance_between_points * i); //Add points from top to bottom
			temp_vector.setY(_bounding_box[2].y - temp_coord.y);
			_bounding_box.push_back(temp_coord);
		}

		temp_coord.x = _bounding_box[0].x; //Work on the western edge
		for (int i = 1; i < num_points; i++)
		{
			temp_coord.y = _bounding_box[0].y - (distance_between_points * i); //Add points from top to bottom
			temp_vector.setY(_bounding_box[3].y - temp_coord.y);
			_bounding_box.push_back(temp_coord);
		}
	}
}

void Path::CreateEdgeNodes(double view_radius)
{

}

void Path::SortNormalBoundingBoxNodes() //Sorts the points by (lat/long)itude beforehand, then swaps into ladder pattern
{
	Coordinate temp; //For swapping coordinates
	const unsigned int size = _bounding_box.size(); //Just to prevent an unnecessary number of calls for the size

	//Choose the sorting method depending on which sides hold the points
	if (!_use_vertical) //If the north/south sides (horizontal edges) hold the points
		sort(_bounding_box.begin(), _bounding_box.end(), longitudesort);
	else //If the east/west sides hold the points
		sort(_bounding_box.begin(), _bounding_box.end(), latitudesort);

	//Rearranging points into a ladder pattern after finding the best point to start at
	if (_waypoints.size() > 1)//Make sure there is more than one point so a heading can be calculated
	{
		Vector last_path(_waypoints[_waypoints.size() - 2], _waypoints.back()); //Create a vector that defines the current heading
		unsigned int index = 0; //Index of the best starting point in the vector
		double min_turn = M_PI; //Used to find the path with the lowest turn angle
		for (unsigned int i = size - 2; i < size + 2; i++) //Scan the ending point pairs for the best starting point
		{
			Vector temp_vector(_waypoints.back(), _bounding_box[i % size]);
			if (AngleBetween(last_path, temp_vector) < min_turn)
			{
				min_turn = AngleBetween(last_path, temp_vector);
				index = i % size;
			}
		}

		//Swap the points into a ladder pattern
		if (index == 0 || index == 1) //If the starting point is on the lesser edge (lower lat/longitude)
		{
			for (unsigned int i = (index ? 0 : 2); i < size; i += 4)
			{
				temp = _bounding_box[i];
				_bounding_box[i] = _bounding_box[i + 1];
				_bounding_box[i + 1] = temp;
			}
		}
		else if (index == size - 2 || index == size - 1) //If the starting point is on the greater edge (largest lat/longitude)
		{
			for (unsigned int i = ((size - 2) % 4 ? 2 : 0); i < size; i += 4)
			{
				temp = _bounding_box[i];
				_bounding_box[i] = _bounding_box[i + 1];
				_bounding_box[i + 1] = temp;
			}
		}
	}
	else //Just make a ladder pattern using the bottom right point as the start
	{
		for (unsigned int i = 2; i < size; i += 4) //Go through and swap alternate pairs
		{
			temp = _bounding_box[i];
			_bounding_box[i] = _bounding_box[i + 1];
			_bounding_box[i + 1] = temp;
		}
	}
}

/*
Steps for Shrinking Nodes to Fit the search area
1. Find the closest direct search area boundary line to move the points towards
2. Find the shortest distance from a pair of points to the line
3. Shift both points towards the line by that shortest distance
*/
void Path::ShrinkNormalNodesToFit()
{
	double min_distance = 180;
	double displacement = 0.0;
	int index = 0; //The index of the closest corner for shifting start/end points
	bool use_vertical_edge = false; //Used to decide which edge to work along
	const double kSearchAlt = _bounding_box[0].z;
	Coordinate swap_coord, intersect_coord;
	Coordinate temp_coord1, temp_coord2; //coord1 holds the point with the most positive x/y, depending on bounding box orientation
	Coordinate edge_coord1, edge_coord2; //Points that define the closest edge to move pairs towards
	Vector temp_vector1, temp_vector2;
	Line edge1, edge2, temp_edge;

	//Determine edge to work on
	if (_bounding_box[0].y == _bounding_box[1].y) //If the long edge is horizontal
		use_vertical_edge = true; //Work along the vertical edge
	else //If the long edge is vertical
		use_vertical_edge = false; //Work along the horizontal edge

	//Loop through all alternating pairs
	//The first point pair is _bounding_box[1] and _bounding_box[2]
	for (unsigned int i = 1; i < _bounding_box.size() - 1; i += 2)
	{
		min_distance = 180;
		for (unsigned int j = 0; j < _search_corners.size(); j++) //Loop for the first point in the pair, look for closest direct edge
		{
			temp_coord1 = _search_corners[j];
			temp_coord2 = _search_corners[(j + 1) % _search_corners.size()]; //Uses modulo so the index circles back around

			if (use_vertical_edge && temp_coord1.y < temp_coord2.y) //Swap the coordinates so the most positive one is coord1
			{
				swap_coord = temp_coord2;
				temp_coord2 = temp_coord1;
				temp_coord1 = swap_coord;
			}
			else if (!use_vertical_edge && temp_coord1.x < temp_coord2.x)
			{
				swap_coord = temp_coord2;
				temp_coord2 = temp_coord1;
				temp_coord1 = swap_coord;
			}

			if (use_vertical_edge && _bounding_box[i].y <= temp_coord1.y && _bounding_box[i].y >= temp_coord2.y) //If the current bounding box point is between two search area corners
			{
				temp_edge = Line(temp_coord1, temp_coord2);
				intersect_coord.x = temp_edge.FindXatY(_bounding_box[i].y);
				intersect_coord.y = temp_edge.FindYatX(intersect_coord.x);
				intersect_coord.z = kSearchAlt;
				Vector temp_vector(intersect_coord, _bounding_box[i]);

				if (temp_vector.getMagnitude() < min_distance)
				{
					min_distance = temp_vector.getMagnitude();
					edge_coord1 = temp_coord1;
					edge_coord2 = temp_coord2;
				}
			}
			else if (!use_vertical_edge && _bounding_box[i].x <= temp_coord1.x && _bounding_box[i].x >= temp_coord2.x)
			{
				temp_edge = Line(temp_coord1, temp_coord2);
				intersect_coord.y = temp_edge.FindYatX(_bounding_box[i].x);
				intersect_coord.x = temp_edge.FindXatY(intersect_coord.y);
				intersect_coord.z = kSearchAlt;
				Vector temp_vector(intersect_coord, _bounding_box[i]);

				if (temp_vector.getMagnitude() < min_distance)
				{
					min_distance = temp_vector.getMagnitude();
					edge_coord1 = temp_coord1;
					edge_coord2 = temp_coord2;
				}
			}
		}

		edge1 = Line(edge_coord1, edge_coord2);

		if (use_vertical_edge)
			displacement = _bounding_box[i].x - edge1.FindXatY(_bounding_box[i].y);
		else
			displacement = _bounding_box[i].y - edge1.FindYatX(_bounding_box[i].x);

		min_distance = 180;
		for (unsigned int j = 0; j < _search_corners.size(); j++) //Loop for the second point in the pair
		{
			temp_coord1 = _search_corners[j];
			temp_coord2 = _search_corners[(j + 1) % _search_corners.size()];
			if (use_vertical_edge && temp_coord1.y < temp_coord2.y)
			{
				swap_coord = temp_coord2;
				temp_coord2 = temp_coord1;
				temp_coord1 = swap_coord;
			}
			else if (!use_vertical_edge && temp_coord1.x < temp_coord2.x)
			{
				swap_coord = temp_coord2;
				temp_coord2 = temp_coord1;
				temp_coord1 = swap_coord;
			}

			if (use_vertical_edge && _bounding_box[i + 1].y <= temp_coord1.y && _bounding_box[i + 1].y >= temp_coord2.y)
			{
				temp_edge = Line(temp_coord1, temp_coord2);
				intersect_coord.x = temp_edge.FindXatY(_bounding_box[i + 1].y);
				intersect_coord.y = temp_edge.FindYatX(intersect_coord.x);
				intersect_coord.z = kSearchAlt;
				Vector temp_vector(intersect_coord, _bounding_box[i + 1]);

				if (temp_vector.getMagnitude() < min_distance)
				{
					min_distance = temp_vector.getMagnitude();
					edge_coord1 = temp_coord1;
					edge_coord2 = temp_coord2;
				}
			}
			else if (!use_vertical_edge && _bounding_box[i + 1].x <= temp_coord1.x && _bounding_box[i + 1].x >= temp_coord2.x)
			{
				temp_edge = Line(temp_coord1, temp_coord2);
				intersect_coord.y = temp_edge.FindYatX(_bounding_box[i + 1].x);
				intersect_coord.x = temp_edge.FindXatY(intersect_coord.y);
				intersect_coord.z = kSearchAlt;
				Vector temp_vector(intersect_coord, _bounding_box[i + 1]);

				if (temp_vector.getMagnitude() < min_distance)
				{
					min_distance = temp_vector.getMagnitude();
					edge_coord1 = temp_coord1;
					edge_coord2 = temp_coord2;
				}
			}
		}

		//Set up the line to move the point pair towards
		edge2 = Line(edge_coord1, edge_coord2);

		//Using a greater-than comparison results in pairs fitting to the inside
		//Using a less-than comparison results in pairs fitting to the outside (path turns out better this way)
		if (use_vertical_edge && fabs(_bounding_box[i + 1].x - edge2.FindXatY(_bounding_box[i + 1].y)) < fabs(displacement))
			displacement = _bounding_box[i + 1].x - edge2.FindXatY(_bounding_box[i + 1].y);
		else if (!use_vertical_edge && fabs(_bounding_box[i + 1].y - edge2.FindYatX(_bounding_box[i + 1].x)) < fabs(displacement))
			displacement = _bounding_box[i + 1].y - edge2.FindYatX(_bounding_box[i + 1].x);

		//Change the x or y position of the coordinate pair depending on the edge the points were on
		if (use_vertical_edge)
		{
			_bounding_box[i].x -= displacement;
			_bounding_box[i + 1].x -= displacement;
		}
		else
		{
			_bounding_box[i].y -= displacement;
			_bounding_box[i + 1].y -= displacement;
		}
	}

	//Move the starting and ending points so they aren't so far from the search area
	//This should be changed. It doesn't work as intended in all cases and is just a placeholder for now.
	min_distance = 180;
	for (unsigned int i = 0; i < _search_corners.size(); i++) //Find the closest search corner and move the starting point to it
	{
		Vector temp_vector(_bounding_box.front(), _search_corners[i]);
		if (temp_vector.getMagnitude() < min_distance)
		{
			min_distance = temp_vector.getMagnitude();
			index = i;
		}
	}
	_bounding_box.front() = _search_corners[index];
	min_distance = 180;
	for (unsigned int i = 0; i < _search_corners.size(); i++) //Same thing but for the ending point
	{
		Vector temp_vector(_bounding_box.back(), _search_corners[i]);
		if (temp_vector.getMagnitude() < min_distance)
		{
			min_distance = temp_vector.getMagnitude();
			index = i;
		}
	}
	_bounding_box.back() = _search_corners[index];
}

void Path::PushToWaypoints()
{
	for (unsigned int i = 0; i < _bounding_box.size(); i++)
		_waypoints.push_back(_bounding_box[i]);
}

void Path::PushSearchToWaypoints()
{
	for (unsigned int i = 0; i < _search_corners.size(); i++)
		_waypoints.push_back(_search_corners[i]);
}

void Path::PushOpToWaypoints()
{
	for (unsigned int i = 0; i < _op_area_corners.size(); i++)
	{
		_waypoints.push_back(_op_area_corners[i]);
	}
}

/*void Path::PushObsToWaypoints()
{
	for (unsigned int i = 0; i < _obstacles.size(); i++)
	{
		_waypoints.push_back(_obstacles[i].getCenter());
	}
}*/

/*bool Path::ReadObstacles(string file_path)
{
	ifstream in;
	in.open(file_path);
	if (!in.is_open())
	{
		cout << "Error opening the obstacles file" << endl;
		return false;
	}
	string buffer;
	while (!in.eof())
	{
		getline(in, buffer);
		istringstream inSS(buffer);
		double x, y, r;
		inSS >> x >> y >> r;
		Coordinate temp(x, y, 0);
		Circle obs(temp, r);
		_obstacles.push_back(obs);
	}
	return true;
}*/

int Path::hasCollision(Coordinate coorA, Coordinate coorB, Circle O)
{
	Vector OAv = Vector(coorA, O.getCenter());
	double OA = OAv.getMagnitude();
	Vector OBv = Vector(coorB, O.getCenter());
	double OB = OBv.getMagnitude();
	double thetaOBA = AngleBetween(coorB, coorA, O.getCenter());
	double thetaOAB = AngleBetween(coorA, coorB, O.getCenter());
	double height = OA*sin(thetaOAB);
	if (thetaOBA >= M_PI/2 || thetaOAB >= M_PI/2) { //Used to have a check for theta being 0
		if (OA <= O.getRadius() && OB <= O.getRadius()) {
			return 4;
		}
		if (OA <= O.getRadius()) {
			return 2;
		}
		if (OB <= O.getRadius()) {
			return 3;
		}
	}
	else {
		if (height < O.getRadius()) {
			if (OA <= O.getRadius() && OB <= O.getRadius()) {
				return 4;
			}
			if (OA <= O.getRadius()) {
				return 2;
			}
			if (OB <= O.getRadius()) {
				return 3;
			}
			return 1;
		}
	}
	return 0;
}

/*Coordinate Path::AvoidObstacle(Coordinate& A, Coordinate& B, Circle O, int index, int flag = 1)
{
	Coordinate avoidance_point;
	double kDistance = 0.00001;
	if (flag == 1) //If the obstacle is on the path between point B and A
	{
		Line line_ab(A, B);
		vector<Coordinate> intersections = FindSolutions(O, line_ab);
		if (intersections.size() == 2) //The path intersects the obstacle twice
		{
			Line tangent1(intersections[0], O.FindSlope(intersections[0]));
			Line tangent2(intersections[1], O.FindSlope(intersections[1]));
			avoidance_point = FindSolution(tangent1, tangent2);
			Vector obstacle_to_avoidance(O.getCenter(), avoidance_point);
			avoidance_point += (obstacle_to_avoidance / obstacle_to_avoidance.getMagnitude() * (O.getRadius() + kDistance));
		}
		else if (intersections.size() == 1) //The path intersects the obstacle once
		{
			avoidance_point = intersections[0];
			Vector obstacle_to_avoidance(O.getCenter(), avoidance_point);
			avoidance_point += (obstacle_to_avoidance / obstacle_to_avoidance.getMagnitude() * (O.getRadius() + kDistance));
		}
		else if (intersections.size() == 0) //There is no intersection
		{
			
		}
	}
	else if (flag == 2) //If the obstacle covers point A
	{
		Vector obstacle_to_point(O.getCenter(), A);
		A += (obstacle_to_point / obstacle_to_point.getMagnitude() * (O.getRadius() + kDistance));
		Line line_ab(A, B);
		vector<Coordinate> intersections = FindSolutions(O, line_ab);
		if (intersections.size() == 2) //The path intersects the obstacle twice
		{
			Line tangent1(intersections[0], O.FindSlope(intersections[0]));
			Line tangent2(intersections[1], O.FindSlope(intersections[1]));
			avoidance_point = FindSolution(tangent1, tangent2);
			Vector obstacle_to_avoidance(O.getCenter(), avoidance_point);
			avoidance_point += (obstacle_to_avoidance / obstacle_to_avoidance.getMagnitude() * (O.getRadius() + kDistance));
		}
		else if (intersections.size() == 1) //The path intersects the obstacle once
		{
			avoidance_point = intersections[0];
			Vector obstacle_to_avoidance(O.getCenter(), avoidance_point);
			avoidance_point += (obstacle_to_avoidance / obstacle_to_avoidance.getMagnitude() * (O.getRadius() + kDistance));
		}
		else if (intersections.size() == 0) //There is no intersection
		{
			
		}
	}
	else if (flag == 3) //If the obstacle covers point B
	{
		Vector obstacle_to_point(O.getCenter(), B);
		B += (obstacle_to_point / obstacle_to_point.getMagnitude() * (O.getRadius() + kDistance));
		Line line_ab(A, B);
		vector<Coordinate> intersections = FindSolutions(O, line_ab);
		if (intersections.size() == 2) //The path intersects the obstacle twice
		{
			Line tangent1(intersections[0], O.FindSlope(intersections[0]));
			Line tangent2(intersections[1], O.FindSlope(intersections[1]));
			avoidance_point = FindSolution(tangent1, tangent2);
			Vector obstacle_to_avoidance(O.getCenter(), avoidance_point);
			avoidance_point += (obstacle_to_avoidance / obstacle_to_avoidance.getMagnitude() * (O.getRadius() + kDistance));
		}
		else if (intersections.size() == 1) //The path intersects the obstacle once
		{
			avoidance_point = intersections[0];
			Vector obstacle_to_avoidance(O.getCenter(), avoidance_point);
			avoidance_point += (obstacle_to_avoidance / obstacle_to_avoidance.getMagnitude() * (O.getRadius() + kDistance));
		}
		else if (intersections.size() == 0) //There is no intersection
		{
			
		}
	}
	else if (flag == 4) //If the obstacle covers both points
	{
		Vector obstacle_to_a(O.getCenter(), A);
		A += (obstacle_to_a / obstacle_to_a.getMagnitude() * (O.getRadius() + kDistance));
		Vector obstacle_to_b(O.getCenter(), B);
		B += (obstacle_to_b / obstacle_to_b.getMagnitude() * (O.getRadius() + kDistance));
		Line line_ab(A, B);
		vector<Coordinate> intersections = FindSolutions(O, line_ab);
		if (intersections.size() == 2) //The path intersects the obstacle twice
		{
			Line tangent1(intersections[0], O.FindSlope(intersections[0]));
			Line tangent2(intersections[1], O.FindSlope(intersections[1]));
			avoidance_point = FindSolution(tangent1, tangent2);
			Vector obstacle_to_avoidance(O.getCenter(), avoidance_point);
			avoidance_point += (obstacle_to_avoidance / obstacle_to_avoidance.getMagnitude() * (O.getRadius() + kDistance));
		}
		else if (intersections.size() == 1) //The path intersects the obstacle once
		{
			avoidance_point = intersections[0];
			Vector obstacle_to_avoidance(O.getCenter(), avoidance_point);
			avoidance_point += (obstacle_to_avoidance / obstacle_to_avoidance.getMagnitude() * (O.getRadius() + kDistance));
		}
		else if (intersections.size() == 0) //There is no intersection
		{
			
		}
	}
	return avoidance_point;
}*/

double Path::PathLength(int begin, int end)
{
	double length = 0.0;
	for (int i = begin; i < end - 1; i++)
	{
		Vector temp(_waypoints[i], _waypoints[i + 1]);
		length += temp.getMagnitude();
	}
	return length;
}

bool Path::DumpDataToFile(string file_path)
{
	ofstream out_file(file_path);
	if (!out_file.is_open())
	{
		cout << "Error opening data output file";
		return false;
	}

	for (unsigned int i = 0; i < _waypoints.size(); i++)
		out_file << "W " << _waypoints[i].x << " " << _waypoints[i].y << " " << _waypoints[i].z << endl;
	for (unsigned int i = 0; i < _search_corners.size(); i++)
		out_file << "S " << _search_corners[i].x << " " << _search_corners[i].y << " " << _search_corners[i].z << endl;
	for (unsigned int i = 0; i < _op_area_corners.size(); i++)
		out_file << "B " << _op_area_corners[i].x << " " << _op_area_corners[i].y << " " << _op_area_corners[i].z << endl;
	//for (unsigned int i = 0; i < _obstacles.size(); i++)
		//out_file << "O " << _obstacles[i].getCenter().x << " " << _obstacles[i].getCenter().y << " " << _obstacles[i].getCenter().z << " " << _obstacles[i].getRadius() << endl;

	out_file.close();
	return true;
}

double Path::DmsToDecimal(double deg, double min, double sec)
{
	return deg + (min / 60.0) + (sec / 3600.0);
}