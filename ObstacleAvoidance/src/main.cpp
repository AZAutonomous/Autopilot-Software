#include <iostream>
#include <list>
#include <Vector>
using namespace std;

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

//Create dynamic and static object classes
class static_obstacle
{
    //Access specifier
    public:
    
    //Data Members
    int id;
    float radius;
    
    
}
class dynamic_obstacle
{
    //Access specifier
    public:
    
    //Data Members
    int id;
    float radius;
    float turning_radius;
    float velocity [2]; //actual velocity with direction and magnitude?
    
    //Past coordinates
    std:: list<pair<int, int> > past_coordinates
    
    //Collision test vectors
    
    //Stored prediction equations
    pair<float, float> left_turn_radius_point
    pair<float, float> right_turn_radius_point
    
}


int main()
{
    
	//Initialize coordinate lists
	std::list<pair<int, int> > coordinate_list;

	//Add current and goal coordinates to the list
	coordinate_list.push_back(make_pair(5, 50)); //Start point
	coordinate_list.push_back(make_pair(95, 60)); //Goal Point


	//Initialize known obstacles
    
    //Initialize static obstacles
    static_obstacle static1;
    static1.id = 1;
    static1.radius = 5;
    
    //Initialize dynamic obstacles
    dynamic_obstacle dynamic1;
    dynamic1.id = 2;
    dynamic1.radius = 10;
    dynamic1.turning_radius = 20;
    dynamic1.velocity[0] = 5;
    dynamic1.velocity[1] = -5;
    dynamic1.past_coordinates.push_back(make_pair(10, 80)); 
	dynamic1.past_coordinates.push_back(make_pair(15, 75));
	dynamic1.past_coordinates.push_back(make_pair(20, 70));
    
       dynamic_obstacle dynamic2;
    dynamic2.id = 2;
    dynamic2.radius = 10;
    dynamic2.turning_radius = 20;
    dynamic2.velocity[0] = -5;
    dynamic2.velocity[1] = 5;
    dynamic2.past_coordinates.push_back(make_pair(90, 40)); 
	dynamic2.past_coordinates.push_back(make_pair(85, 45));
	dynamic2.past_coordinates.push_back(make_pair(80, 50));


	
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
