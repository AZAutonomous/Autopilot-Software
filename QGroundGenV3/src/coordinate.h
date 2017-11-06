#ifndef COORDINATE_H
#define COORDINATE_H
#include <vector>

//BASIC LIBRARY FOR HANDLING COORDINATES, VECTORS, AND LINES.
//HOLDS DATA FOR 3D SPACE, BUT ALL FUNCTIONS BUT THE DOT/CROSS
//PRODUCTS ACT IN 2D SPACE. MADE FOR AZ AUTONOMOUS PATH GEN.

//Class Declarations
class Coordinate;
class Vector;
class Line;
class Circle;

//Class Definitions
struct Coordinate {
	Coordinate();
	Coordinate(double x, double y, double z);
	double x = 0;
	double y = 0;
	double z = 0;

	Coordinate operator+(Vector& displacement_vector);
};

class Vector {
private:
	double _x;
	double _y;
	double _z;
	double _magnitude;

public:
	Vector();
	Vector(double x, double y, double z);
	Vector(Coordinate coord1, Coordinate coord2);
	Vector(double magnitude, double angle); //Create a vector given the counterclockwise angle and magnitude

	double getX() const;
	double getY() const;
	double getZ() const;
	double getMagnitude() const;

	void setX(double x);
	void setY(double y);
	void setZ(double z);

	Vector operator*(double mult);
	Vector operator/(double div);
	Vector operator+(Vector& vect);
	Vector operator-(Vector& vect);

	double getDirection(); //Returns the angle counterclockwise from the origin that the vector is pointing
};

class Line {
private:
	double _slope; //Slope of line formed by coord1 and coord2
	double _y_intercept; //Y intercept of resulting line from coord1 and coord2
	double CalculateSlope(Coordinate coord1, Coordinate coord2); //Helper for handling slope

public:
	Line();
	Line(Coordinate coord1, Coordinate coord2);
	Line(Coordinate point, double slope);

	double getSlope();
	double getYIntercept();

	double FindYatX(double x); //Returns the Y value for a given X value
	double FindXatY(double y); //Returns the X value where a given Y value occurs
};

class Circle {
private:
	Coordinate _center;
	double _radius;

public:
	Circle(Coordinate center, double radius);

	Coordinate getCenter();
	double getRadius();

	void setCenter(Coordinate center);
	void setRadius(double radius);

	double FindSlope(Coordinate point);
};

Vector CrossProduct(Vector vect1, Vector vect2); //Returns a vector product of 2 3-space vectors
double DotProduct(Vector vect1, Vector vect2); //Returns the 3D dot product of 2 3-space vectors
double AngleBetween(Vector vect1, Vector vect2); //Returns the angle between two vectors, with respect to xy-plane
double AngleBetween(Coordinate vertex, Coordinate coord1, Coordinate coord2); //Does the same as the above, but with coordinates instead of vectors
Coordinate FindSolution(Line line1, Line line2); //Returns the coordinate of the point that the two lines intersect at
std::vector<Coordinate> FindSolutions(Circle circ, Line line); //Finds the coordinates that a line intersects a circle
std::vector<Coordinate> FindTangentPoints(Circle circle, Coordinate point); //Finds lines tangent to a circle that contain the coordinate 'point'

#endif