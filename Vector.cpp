#include "Vector.h"
#include <cmath>
#define PI 3.14159265

Vector::Vector(Point p1, Point p2) 
{
	a = p1;
	b = p2;
	lenght_x = p2.x - p1.x;
	lenght_y = p2.y - p1.y;
}

Vector::Vector()
{
	a = Point{0,0};
	b = Point{0,0};
	lenght_x = 0;
	lenght_y = 0;
}

Vector Vector::GetOrti()
{
	Point a{0,0};
	Point b{1,-0};
	return Vector(a,b);
}

Point Vector::GetPointA()
{
	return a;
}

Point Vector::GetPointB()
{
	return b;
}
int Vector::GetLenght_X()
{
	return lenght_x;
}

int Vector::GetLenght_Y()
{
	return lenght_y;
}

Vector Vector::DevisionVectorByNumber(int number) 
{
	lenght_x /= number;
	lenght_y /= number;
	return *this;

}

Vector Vector::DevisionVectorByNumber(Vector vec, int number) {
	return DevisionVectorByNumber(number);
}

double  Vector::GetAngleBetweenVectors(Vector vec)
{
	double t = (lenght_x * vec.GetLenght_X() + lenght_y * vec.GetLenght_Y()) / (sqrt((double)pow(lenght_x, 2) + pow(lenght_y, 2)) * sqrt((double)pow(vec.GetLenght_X(), 2) + pow(vec.GetLenght_Y(), 2)));
	if (t < -1) t = -1;
	else if (t > 1) t = 1;
	double r = acos(t) * 180 / PI;
	if (lenght_y < vec.GetLenght_Y())
		return r;
	else
		return 360 - r;
}

double Vector::GetAngleBetweenVectors(Vector v1, Vector v2) 
{
	return v1.GetAngleBetweenVectors(v2);
}

double Vector::GetAngleToOrtoi() {
	Vector orti{ Point{0,0}, Point{1,0} };
	return this->GetAngleBetweenVectors(orti);
}


double Vector::GetVectorLenght(Point p1, Point p2)
{
	return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

double Vector::GetVectorLenght()
{
	return sqrt(pow(lenght_x, 2) + pow(lenght_y, 2));
}

double Vector::ScalarProduct(Vector vec) 
{   
	return this->GetVectorLenght() * vec.GetVectorLenght() * cos(this->GetAngleBetweenVectors(vec)/180 * PI);
}

bool Vector::CheckPoint(Point p, double eps) 
{
	Vector vec = Vector(this->GetPointA(), p);
	double angle = sin(this->GetAngleBetweenVectors(vec)/180*PI);

	if ((p.x >= this->GetPointA().x && p.x <= this->GetPointB().x || p.x <= this->GetPointA().x && p.x >= this->GetPointB().x) && abs(angle) < eps)
		return true;
	else return false;

}





