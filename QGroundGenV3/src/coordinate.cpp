#include "coordinate.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>

Coordinate::Coordinate() {}

Coordinate::Coordinate(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

////////////////////
//VECTOR FUNCTIONS//
////////////////////

Vector::Vector() //Default constructor creates a zero vector
{
	this->_x = 0.0;
	this->_y = 0.0;
	this->_z = 0.0;
	this->_magnitude = 0.0;
}

Vector::Vector(double x, double y, double z) //This constructor creates a vector given x, y, z displacements
{
	this->_x = x;
	this->_y = y;
	this->_z = z;
	this->_magnitude = sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
}

Vector::Vector(Coordinate coord1, Coordinate coord2) //This constructor does the same as the previous but uses two points to find the displacements
{
	this->_x = coord2.x - coord1.x;
	this->_y = coord2.y - coord1.y;
	this->_z = coord2.z - coord1.z;
	this->_magnitude = sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
}

double Vector::getX() const { return _x; }
double Vector::getY() const { return _y; }
double Vector::getZ() const { return _z; }
double Vector::getMagnitude() const { return _magnitude; }

void Vector::setX(double x)
{
	this->_x = x;
	this->_magnitude = sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
}

void Vector::setY(double y)
{
	this->_y = y;
	this->_magnitude = sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
}

void Vector::setZ(double z)
{
	this->_z = z;
	this->_magnitude = sqrt(pow(_x, 2) + pow(_y, 2) + pow(_z, 2));
}

Vector Vector::operator*(double mult)
{
	Vector result(_x * mult, _y * mult, _z * mult);
	return result;
}

Vector Vector::operator/(double div)
{
	Vector result(_x / div, _y / div, _z / div);
	return result;
}

Vector Vector::operator+(Vector& vect)
{
	Vector result(_x + vect.getX(), _y + vect.getY(), _z + vect.getZ());
	return result;
}

Vector Vector::operator-(Vector& vect)
{
	Vector result(_x - vect.getX(), _y - vect.getY(), _z - vect.getZ());
	return result;
}

/////////////////////////////
//LINEAR EQUATION FUNCTIONS//
/////////////////////////////

LinearEq::LinearEq()
{
	Coordinate coord1, coord2;
	this->_coord1 = coord1;
	this->_coord2 = coord2;
	this->_slope = 0.0;
	this->_y_intercept = 0.0;
}

LinearEq::LinearEq(Coordinate coord1, Coordinate coord2)
{
	this->_coord1 = coord1;
	this->_coord2 = coord2;
	this->_slope = CalculateSlope();
	this->_y_intercept = _coord1.y - (_slope * _coord1.x);
}

LinearEq::LinearEq(Coordinate point, double slope)
{
	this->_slope = slope;
	this->_y_intercept = point.y - (_slope * point.x);
	this->_coord1 = point;
	Coordinate coord2(point.x + 1, point.y + _slope, 0);
	this->_coord2 = coord2;
}

Coordinate LinearEq::getCoord1() { return this->_coord1; }
Coordinate LinearEq::getCoord2() { return this->_coord2; }
double LinearEq::getSlope() { return this->_slope; }
double LinearEq::getYIntercept() { return this->_y_intercept; }

void LinearEq::setCoord1(Coordinate coord)
{
	this->_coord1 = coord;
	this->_slope = CalculateSlope();
	this->_y_intercept = _coord1.y - (_slope * _coord1.x);
}

void LinearEq::setCoord2(Coordinate coord)
{
	this->_coord2 = coord;
	this->_slope = CalculateSlope();
	this->_y_intercept = _coord1.y - (_slope * _coord1.x);
}

double LinearEq::CalculateSlope()
{
	if (_coord1.x - _coord2.x == 0) //This makes the slope undefined
		return std::numeric_limits<double>::quiet_NaN();
	else
		return (_coord1.y - _coord2.y) / (_coord1.x - _coord2.x);
}

double LinearEq::FindYatX(double x)
{
	return (_slope * x) + _y_intercept;
}

double LinearEq::FindXatY(double y)
{
	if (_slope == 0)
		return 0.0;
	else
		return (y - _y_intercept) / _slope;
}

///////////////////
//OTHER FUNCTIONS//
///////////////////

double DotProduct(Vector vect1, Vector vect2)
{
	return (vect1.getX() * vect2.getX()) + (vect1.getY() * vect2.getY()) + (vect1.getZ() * vect2.getZ());
}

double AngleBetween(Vector vect1, Vector vect2) //Returns positive angle between 0 and PI radians
{
	double result, dot, deter;
	dot = (vect1.getX() * vect2.getX()) + (vect1.getY() * vect2.getY()); //Dot product of only x and y components
	deter = (vect1.getX() * vect2.getY()) - (vect2.getX() * vect1.getY()); //Determinant using only x and y components
	
	if (dot == 0) //This will make the denominator 0 in arctan function, but dot product of 0 means perpendicular vectors
		return M_PI / 2;
	else if (dot == -1 * vect1.getMagnitude()*vect2.getMagnitude()) {
		return M_PI;
	}
	result = atan(deter / dot); //Using arctan means no domain restrictions, but output must be corrected

	if (dot < 0 && deter / dot > 0) //Fixes how arctan handles angles larger than 90 degrees
		return M_PI - result;
	else if (dot < 0 && deter / dot < 0) //Same as above, but for when the signs of deter and dot are different
		return result + M_PI;
	else
		return fabs(result);
}

double AngleBetween(Coordinate vertex, Coordinate coord1, Coordinate coord2)
{
	Vector vector1(vertex, coord1);
	Vector vector2(vertex, coord2);
	return AngleBetween(vector1, vector2);
}

Vector CrossProduct(Vector vect1, Vector vect2)
{
	Vector product;
	product.setX(vect1.getY() * vect2.getZ() - vect1.getZ() * vect2.getY());
	product.setY(vect1.getZ() * vect2.getX() - vect1.getX() * vect2.getZ());
	product.setZ(vect1.getX() * vect2.getY() - vect1.getY() * vect2.getX());
	return product;
}

Coordinate FindSolution(LinearEq eq1, LinearEq eq2)
{
	Coordinate result;
	if (eq1.getSlope() == eq2.getSlope()) //Either the lines are parallel and have no solution or they are the same line and have infinitely many
	{
		result.x = std::numeric_limits<double>::quiet_NaN();
		result.y = std::numeric_limits<double>::quiet_NaN();
	}
	else
	{
		result.x = (eq2.getYIntercept() - eq1.getYIntercept()) / (eq1.getSlope() - eq2.getSlope());
		result.y = eq1.FindYatX(result.x);
	}
	return result;
}