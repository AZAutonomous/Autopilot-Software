#ifndef COORDINATE_H
#define COORDINATE_H
#include <vector>

//BASIC LIBRARY FOR HANDLING COORDINATES, VECTORS, AND LINES.
//HOLDS DATA FOR 3D SPACE, BUT ALL FUNCTIONS BUT THE DOT/CROSS
//PRODUCTS ACT IN 2D SPACE. MADE FOR AZ AUTONOMOUS PATH GEN.

//Class Declarations
struct Coordinate;
class Vector;
class Line;
class LineSeg;
class Circle;

//Class Definitions
struct Coordinate
{
	Coordinate();
	Coordinate(double x, double y, double z);
	double x = 0;
	double y = 0;
	double z = 0;

	Coordinate& operator+=(const Vector& displacement_vector);
	Coordinate& operator-=(const Vector& displacement_vector);
	//Coordinate operator+(const Vector& displacement_vector);
};

class Vector
{
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

	double getX() const { return _x; }
	double getY() const { return _y; }
	double getZ() const { return _z; }
	double getMagnitude() const { return _magnitude; }

	void setX(double x);
	void setY(double y);
	void setZ(double z);

	Vector operator*(double mult);
	Vector operator/(double div);
	Vector operator+(Vector& vect);
	Vector operator-(Vector& vect);

	double getDirection(); //Returns the angle counterclockwise from the origin that the vector is pointing
};

class Line
{
protected:
	double _slope; //Slope of line formed by coord1 and coord2
	double _y_intercept; //Y intercept of resulting line from coord1 and coord2
	double CalculateSlope(Coordinate coord1, Coordinate coord2); //Helper for handling slope

public:
	Line();
	Line(Coordinate coord1, Coordinate coord2);
	Line(Coordinate point, double slope);

	double getSlope() { return _slope; }
	double getYIntercept() { return _y_intercept; }

	double FindYatX(double x); //Returns the Y value for a given X value
	double FindXatY(double y); //Returns the X value where a given Y value occurs
};

class LineSeg: public Line
{
private:
	Coordinate _coord1;
	Coordinate _coord2;

public:
	Coordinate getCoord1() { return _coord1; }
	Coordinate getCoord2() { return _coord2; }

	void setCoord1(Coordinate coord) { _coord1 = coord; }
	void setCoord2(Coordinate coord) { _coord2 = coord; }

	bool PointIsInRectangle(Coordinate point); //Returns true if the point provided is within the rectangle defined by the diagonal formed by coord1 and coord2
	bool PointIsBetweenPerpendiculars(Coordinate point); //Returns true if the point provided is between perpendicular lines passing through coord1 and coord2
};

class Circle
{
private:
	Coordinate _center;
	double _radius;

public:
	Circle(Coordinate center, double radius);

	Coordinate getCenter() { return _center; }
	double getRadius() { return _radius; }

	void setCenter(Coordinate center) { _center = center; }
	void setRadius(double radius) { _radius = radius; }

	double FindSlope(Coordinate point);
};

Coordinate const operator+(Coordinate lhs, const Vector& displacement_vector);
Coordinate const operator-(Coordinate lhs, const Vector& displacement_vector);
Vector CrossProduct(Vector vect1, Vector vect2); //Returns a vector product of 2 3-space vectors
double DotProduct(Vector vect1, Vector vect2); //Returns the 3D dot product of 2 3-space vectors
double AngleBetween(Vector vect1, Vector vect2); //Returns the angle between two vectors, with respect to xy-plane
double AngleBetween(Coordinate vertex, Coordinate coord1, Coordinate coord2); //Does the same as the above, but with coordinates instead of vectors
Coordinate FindSolution(Line line1, Line line2); //Returns the coordinate of the point that the two lines intersect at
std::vector<Coordinate> FindSolutions(Circle circ, Line line); //Finds the coordinates that a line intersects a circle
//std::vector<Coordinate> FindSolutions(Circle first, Circle second); //Finds the coordinates where a circle intersects another circle
//std::vector<Coordinate> FindTangentPoints(Circle circle, Coordinate point); //For finding the lines containing 'point' that are tangent to a circle
Coordinate Midpoint(Coordinate first, Coordinate second);
double DistanceBetween(Coordinate first, Coordinate second);
//bool isInBoundary(std::vector<Coordinate>& boundary, Coordinate object); //Returns true if the coordinate is within the boundary defined by the vector of coordinates. Assumes coordinate vector is ordered properly

bool longitudesort(const Coordinate& lhs, const Coordinate& rhs);
bool latitudesort(const Coordinate& lhs, const Coordinate& rhs);
#endif