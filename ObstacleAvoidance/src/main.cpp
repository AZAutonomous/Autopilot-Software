#include <iostream>
#include <list>
#include <Vector>
#include "coordinate.h"
#include "obstacle.h"

using namespace std;
class static_obstacle;
class dynamic_obstacle;

//This program takes in the goal/goal coordinates of waypoints, and finds the best path from the plane's
//current location to the objectives, considering risk and distance.

//Main data structures
//Container of static obstacles 
//Container of dynamic obstacles 
//Stack of planned coordinates
//Mini container for test coordinates 


//Input: Current Location, goal/goals location, obstacle last two/three locations.
//Create Vector for plane's direction
//Create three vectors per obstacle
//Check if vectors will cross
//If so, assign collision location and plot 5 options for the first obstacle.
//Vote on each option by random/eval
//Start process again from voted point
//Go until all obstacles are cleared
//Further optimize path/smooth

//Global variables
//int latitude_prefix;
//int longitude_prefix;




int main()
{
	cout << "test\n";
	//Initialize coordinate lists
	std::list<pair<int, int> > coordinate_list;

	//Add current and goal coordinates to the list
	coordinate_list.push_back(make_pair(5, 50)); //Start point
	coordinate_list.push_back(make_pair(95, 60)); //Goal Point


	//Initialize known obstacles
    
    //Initialize static obstacles
	obstacle teststatic1;
    
    //Initialize dynamic obstacles
   


	
	//Test to see if there could be any collisions
	//Create/use vector for plane direction
	//Use three vectors for obstacle direction






	//Print the starting and ending coordiates
	std::cout << "Stack coordinates" << endl;

	//Declare iterator because it's messy
	list<pair<int, int> >::iterator it;
	
	for( it = coordinate_list.begin(); it != coordinate_list.end(); ++it)
	{
		std::cout << (*it).first << ", " << (*it).second << endl;
	}
	
	//Print the obstacle last known locations


	//Pause the program at the end
	getchar();
	return 0;

}
