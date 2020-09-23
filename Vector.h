#pragma once
struct Point {
	int x;
	int y;
};

class Vector
{
private:
	int lenght_x;
	int lenght_y;
	Point a;
	Point b;

public:
	Vector(Point p1, Point p2);
	Vector();
	Vector DevisionVectorByNumber(int number);
	Vector DevisionVectorByNumber(Vector vec, int number);
	Vector GetOrti();
	Point GetPointA();
	Point GetPointB();
	int GetLenght_X();
	int GetLenght_Y();
	double GetAngleBetweenVectors(Vector vec1, Vector vec2);
	double GetAngleBetweenVectors(Vector vec);
	double GetAngleToOrtoi();
	double GetVectorLenght(Point p1, Point p2);
	double GetVectorLenght();
	double ScalarProduct(Vector v1);
	bool CheckPoint(Point p, double eps);

	

};




