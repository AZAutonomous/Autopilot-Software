#include <iostream>
#include <fstream>
#include "coordinate.h"
#include "path.h"
using namespace std;

int main()
{
	string in_path = "../test_files/2017Suasmission.txt";
	string out_path = "../test_files/test_output.mission";
	Path navigation;
	navigation.ReadFromFile(in_path, 100);

	Coordinate home(-76.4342, 38.1509, 0);
	Coordinate takeoff(-76.4291, 38.1453, 60.96);
	Coordinate ascent(-76.4295, 38.1492, 91.44);
	Coordinate descent(-76.4342, 38.1509, 50);

	//This block is for an unrotated bounding box
	
	//navigation.DefineNormalBoundingBox(100);
	//navigation.CreateNormalEdgeNodes(0.00025);
	//navigation.SortNormalBoundingBoxNodes();
	//navigation.ShrinkNormalNodesToFit();
	

	navigation.DefineBoundingBox(100);

	navigation.PushToWaypoints();

	navigation.WriteToFile(out_path, home, takeoff, descent, ascent);
	return EXIT_SUCCESS;
}