The goal of this algorithm is to create a path between two points that minimizes both the risk of collision with obstacles and the length of the path. 
The first implementation of this algorithm will be on the initial waypoint hitting paths. It will then be adapted and expanded to include all other tasks excluding the searching path.
This algorithm will be a core component of the dynamic pathfinding module in the avionics software. It will interact with the server intermediary code as well as the pixhawk communication code.
Inputs:
Information about dynamic obstacles:
	Previous coordinates
	Velocity (Given or calculated)
	Radii
	
Static Obstacles
	Coordinate
	Radii

Search Boundaries

Outputs:
Modified coordinate stack to be uploaded to the Pixhawk

Data Structures:
Dynamic obstacle Object
Static Obstacle Object (Could also be a special case of a dynamic obstacle object)
Coordinate Struct
Pixhawk coordinate stack

Main Stages of algorithm.
•	Check for predicted collisions
•	Create dodge anchor points
•	Generate Prediction Equations for dynamic obstacles
•	Generate alternative paths
•	Evaluate alternative paths
•	Select dodge point
•	Repeat algorithm with new points. Iterate until a sufficient path to the objective is determined.
Check for predicted collisions:
Inputs: Dynamic and Static Obstacle Objects
Outputs: Closest collision location on plane’s path
Overview: Create three vectors going from each dynamic obstacles location, at -45, 0, and 45 degrees from its current heading. Check each vector for intersection with the path. If there are intersections, average the coordinates of the intersections and mark that on the path.
For static obstacles, create four vectors around the object. Then do the same thing.
Create Dodge Anchor Points:
Input: Collision location Points
Output: 5 alternative dodge nodes for the closest predicted collision.
Overview: At the predicted collision location, determine the vector perpendicular to the intended path. Create one point on the path at the predicted collision location. Create two points above the path on the perpendicular vector, 100 ft apart (This value could change based on how well it works). Create two points below at the same distance apart.
Generate Prediction Equations for dynamic obstacles
	Input: Dynamic and static obstacles
	Output: Equations that define their predicted paths
	Overview: For each obstacle, create a prediction equation based off of the shapes that we had discussed. The equations should be such that for continuous time, a likely region can be determined.

Generate Alternative Paths:
	Input:
	Output:
