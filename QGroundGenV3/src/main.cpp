
#include <iostream>
#include <fstream>
#include "coordinate.h"
#include "path.h"
using namespace std;

int main()
{
	string in_path = "test.txt";
	string out_path = "../test_files/test_output.mission";
	//string obs_path = "../test_files/obstacle_list.txt";
	Path navigation;
	if (navigation.ReadFromFile(in_path, 100) == false) {
		printf("could not open\n");
		system("pause");
		return 0;
	}

	Coordinate home(-76.4342, 38.1509, 0);
	Coordinate takeoff(-76.4291, 38.1453, 60.96);
	Coordinate ascent(-76.4295, 38.1492, 91.44);
	Coordinate descent(-76.4342, 38.1509, 50);

	//This block is for an unrotated bounding box
	navigation.DefineNormalBoundingBox(100);
	navigation.CreateNormalEdgeNodes(0.00025); //0.00025 for actual testing
	navigation.SortNormalBoundingBoxNodes();
	navigation.ShrinkNormalNodesToFit();

	//collision detection testing
	//Coordinate coorA = Coordinate(1.0, 1.0, 0.0);
	//Coordinate coorB = Coordinate(2.0, 2.0, 0.0);
	//Circle obs(Coordinate(1.5, 1.5, 0.0), 0.1);
	//int hasCollision = navigation.hasCollision(coorA, coorB, obs);
	//cout << hasCollision << endl;
	//navigation.DefineBoundingBox(100);

	navigation.PushToWaypoints();
	navigation.PushSearchToWaypoints(); //For debugging, to see the search boundary
	//navigation.PushOpToWaypoints(); //For debugging, to see the op area
	//navigation.ReadObstacles(obs_path);
	//navigation.PushObsToWaypoints(); //For debugging, to see the obstacles on the map

	navigation.WriteToFile(out_path, home, takeoff, descent, ascent);
	navigation.DumpDataToFile(string("../test_files/points.txt"));
	printf("completed\n");
	return EXIT_SUCCESS;
}