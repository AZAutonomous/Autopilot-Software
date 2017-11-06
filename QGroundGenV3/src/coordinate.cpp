#define _USE_MATH_DEFINES
#include "coordinate.h"
#include <cmath>
#include <limits>

////////////////////////
//COORDINATE FUNCTIONS//
////////////////////////

Coordinate::Coordinate() {}

Coordinate::Coordinate(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Coordinate Coordinate::operator+(Vector& displacement_vector)
{
	return Coordinate(x + displacement_vector.getX(), y + displacement_vector.getY(), z + displacement_vector.getZ());
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

Vector::Vector(double magnitude, double angle) //Does not create a 3-space vector
{
	this->_x = magnitude * cos(angle);
	this->_y = magnitude * sin(angle);
	this->_z = 0;
	this->_magnitude = magnitude;
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

double Vector::getDirection()
{
	if (_x > 0)
	{
		return atan(_y / _x);
	}
	else if (_x < 0)
	{
		return atan(_y / _x) + M_PI;
	}
	else if (_x == 0)
	{
		if (_y > 0)
			return M_PI / 2;
		else if (_y < 0)
			return -M_PI / 2;
		else if (_y == 0)
			return std::numeric_limits<double>::quiet_NaN();
	}
}

/////////////////////////////
//LINEAR EQUATION FUNCTIONS//
/////////////////////////////

Line::Line()
{
	this->_slope = 0.0;
	this->_y_intercept = 0.0;
}

Line::Line(Coordinate coord1, Coordinate coord2)
{
	this->_slope = CalculateSlope(coord1, coord2);
	this->_y_intercept = coord1.y - (_slope * coord1.x);
}

Line::Line(Coordinate point, double slope)
{
	this->_slope = slope;
	this->_y_intercept = point.y - (_slope * point.x);
}

double Line::getSlope() { return this->_slope; }
double Line::getYIntercept() { return this->_y_intercept; }

double Line::CalculateSlope(Coordinate coord1, Coordinate coord2)
{
	if (coord1.x - coord2.x == 0) //This makes the slope undefined
		return std::numeric_limits<double>::quiet_NaN();
	else
		return (coord1.y - coord2.y) / (coord1.x - coord2.x);
}

double Line::FindYatX(double x)
{
	return (_slope * x) + _y_intercept;
}

double Line::FindXatY(double y)
{
	if (_slope == 0)
		return 0.0;
	else
		return (y - _y_intercept) / _slope;
}

////////////////////
//CIRCLE FUNCTIONS//
////////////////////

Circle::Circle(Coordinate center = Coordinate(), double radius = 1.0)
{
	this->_center = center;
	this->_radius = radius;
}

Coordinate Circle::getCenter() { return this->_center; }
double Circle::getRadius() { return this->_radius; }

void Circle::setCenter(Coordinate center) { this->_center = center; }
void Circle::setRadius(double radius) { this->_radius = radius; }

double Circle::FindSlope(Coordinate point)
{
	if (point.y == _center.y)
		return std::numeric_limits<double>::quiet_NaN();
	else
		return -(point.x - _center.x) / (point.y - _center.y);
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

	result = atan(deter / dot); //Using arctan means no domain restrictions, but output must be corrected

	if (deter != 0)
	{
		if (dot > 0)
			return fabs(result);
		else if (deter < 0)
			return M_PI - result;
		else if (deter > 0)
			return M_PI + result;
	}
	else if (dot < 0)
	{
		return M_PI;
	}
	else if (dot > 0)
	{
		return 0.0;
	}

	return 0.0;
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

Coordinate FindSolution(Line line1, Line line2)
{
	Coordinate result;
	if (line1.getSlope() == line2.getSlope()) //Either the lines are parallel and have no solution or they are the same line and have infinitely many
	{
		result.x = std::numeric_limits<double>::quiet_NaN();
		result.y = std::numeric_limits<double>::quiet_NaN();
	}
	else
	{
		result.x = (line2.getYIntercept() - line1.getYIntercept()) / (line1.getSlope() - line2.getSlope());
		result.y = line1.FindYatX(result.x);
	}
	return result;
}

std::vector<Coordinate> FindSolutions(Circle circ, Line line)
{
	std::vector<Coordinate> solutions;
	Coordinate solution1, solution2;
	double x1, x2;
	double a = pow(line.getSlope(), 2) + 1;
	double b = (2 * line.getYIntercept() * line.getSlope()) - (2 * line.getSlope() * circ.getCenter().y) - (2 * circ.getCenter().x);
	double c = pow(line.getYIntercept(), 2) - (2 * line.getYIntercept() * circ.getCenter().y) + pow(circ.getCenter().y, 2) + pow(circ.getCenter().x, 2) - pow(circ.getRadius(), 2);

	if (pow(b, 2) - 4 * a * c < 0) //Complex Roots
	{
		//For now, do nothing
	}
	else if (pow(b, 2) - 4 * a * c > 0) //Two Real Roots
	{
		x1 = ((-1 * b) + sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
		x2 = ((-1 * b) - sqrt(pow(b, 2) - 4 * a * c)) / (2 * a);
		solution1 = Coordinate(x1, line.FindYatX(x1), 0);
		solution2 = Coordinate(x2, line.FindYatX(x2), 0);
		solutions.push_back(solution1);
		solutions.push_back(solution2);
	}
	else //Single Repeated Root (tangent point)
	{
		x1 = (-1 * b) / (2 * a);
		solution1 = Coordinate(x1, line.FindYatX(x1), 0);
		solutions.push_back(solution1);
	}

	return solutions;
}

std::vector<Coordinate> FindTangentPoints(Circle circle, Coordinate point)
{
	std::vector<Coordinate> points;
	Coordinate tangent_point1, tangent_point2;
	Vector point_to_circle(point, circle.getCenter());
	double angle, angle_shift, tangent_magnitude; //The angle of the point_to_circle vector, how much to rotate it to create a tangent, and the magnitude of the tangent
	if (point_to_circle.getMagnitude() > circle.getRadius()) //If the point is outside of the circle
	{
		angle = point_to_circle.getDirection();
		angle_shift = asin(circle.getRadius() / point_to_circle.getMagnitude()); //Angle at vertex point of a triangle defined by the tangency point, "point", and circle center
		tangent_magnitude = sqrt(pow(point_to_circle.getMagnitude(), 2) - pow(circle.getRadius(), 2));
		Vector tangent1(tangent_magnitude, angle + angle_shift); //Create a tangent that is angle_shift ahead of the point_to_circle vector
		Vector tangent2(tangent_magnitude, angle - angle_shift); //Create a tangent that is angle_shift behind the point_to_circle vector
		tangent_point1 = Coordinate(point.x + tangent1.getX(), point.y + tangent1.getY(), 0); //Create points by adding the displacement vectors to the original point
		tangent_point2 = Coordinate(point.x + tangent2.getX(), point.y + tangent2.getY(), 0);
	}
	else if (point_to_circle.getMagnitude() == circle.getRadius()) //"point" is a point of tangency
	{
		points.push_back(point);
	}
	
	return points;
}