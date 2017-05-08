***************************************************************
-- QGroundControl Flight Path Generator --
***************************************************************
This program is to parse input coordinates into qgroundcontrol readable files

-- Procedure of Program -- 
----------------------------------------------------------------
- Reads in an input file located in agroundgen/files
- Prompts user for 4 coordinates -- Takeoff, TakeoffWaypoint,Land/Home,LandWaypoint
Note - if user enters all 0s the coordinate will default to a test coordinate, this is for quick debugging purposes
- Prompts the user for the Alt,spacing and turn around dist of the search area path
- Creates a file called "missionOutput.mission" in /files to be read as an input in Qgroundcontrol
*****************************************************************

-- Program Running Specs --
----------------------------------------------------------------
After inputs are parsed program creates file as follows
-Adds the takeoff location and takeoff waypoint to the begining of the file
-Reads the input file and converts each coordinate from deg,min,sec into decimal
-Adds each waypoint vector to the next part of the file
-Adds the search path with user input specs after the Waypoints 
(this means that the program does not work if there are waypoints that must be traveled to after the search area)
-Adds LandWaypoint and Land to the file
*****************************************************************
