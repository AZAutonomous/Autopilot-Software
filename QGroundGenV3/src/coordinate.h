#ifndef COORDINATE_H
#define COORDINATE_H

//BASIC LIBRARY FOR HANDLING COORDINATES, VECTORS, AND LINES.
//HOLDS DATA FOR 3D SPACE, BUT ALL FUNCTIONS BUT THE DOT/CROSS
//PRODUCTS ACT IN 2D SPACE. MADE FOR AZ AUTONOMOUS PATH GEN.

struct Coordinate {
	Coordinate();
	Coordinate(double x, double y, double z);
	double x = 0;
	double y = 0;
	double z = 0;
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

	double getX() const;
	double getY() const;
	double getZ() const;
	double getMagnitude() const;

	void setX(double x);
	void setY(double y);
	void setZ(double z);

	Vector operator*(double mult);
	Vector operator/(double div);
};

class LinearEq {
private:
	Coordinate _coord1;
	Coordinate _coord2;
	double _slope; //Slope of line formed by coord1 and coord2
	double _y_intercept; //Y intercept of resulting line from coord1 and coord2
	double CalculateSlope(); //Helper for handling slope

public:
	LinearEq();
	LinearEq(Coordinate coord1, Coordinate coord2);
	LinearEq(Coordinate point, double slope);

	Coordinate getCoord1();
	Coordinate getCoord2();
	double getSlope();
	double getYIntercept();

	void setCoord1(Coordinate coord);
	void setCoord2(Coordinate coord);

	double FindYatX(double x); //Returns the Y value for a given X value
	double FindXatY(double y); //Returns the X value where a given Y value occurs
};

Vector CrossProduct(Vector vect1, Vector vect2); //Returns a vector product of 2 3-space vectors
double DotProduct(Vector vect1, Vector vect2); //Returns the 3D dot product of 2 3-space vectors
double AngleBetween(Vector vect1, Vector vect2); //Returns the angle between two vectors, with respect to xy-plane
double AngleBetween(Coordinate vertex, Coordinate coord1, Coordinate coord2); //Does the same as the above, but with coordinates instead of vectors
Coordinate FindSolution(LinearEq eq1, LinearEq eq2); //Returns the coordinate of the point that the two lines intersect at

#endif