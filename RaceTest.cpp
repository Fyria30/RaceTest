// RaceTest.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <cmath> 
#include <cstdlib>
#include <ctime>
#include <random>
//#include <windows.h>
#include <chrono>
#include "vector.h"

#define PI 3.14159265;
#define TRACK_SIZE 5
#define POINTS_PER_LINE 2
#define ACCURACY 30
#define TRACE_SIZE 500

//used functions
void TraceGeneration(); 
void CarGeneration();
Point MoveAlongLine(Point departure, double lenght, double phi); //Return a point moved in lenght on phi angle
void Algorithm(); // Finds proection
void DrawGraphs(); // Draw the graphs (windows)
double GetRandomDouble(double a, double b);
bool CheckResult(); // Compare initial array of points and proection array

//finally not used functions but can be usefull for test/changing the programm
double GetRandomPhi(double phi);
Point MakeShot(Point departure, double lenght, double phi);
void DisplayPoint(Point p);
bool CheckRange(Point p1, Point p2, Point p3);
Point PointNearLineGeneration(Point p1, Point p2);
void ShowResult();


struct Car {
    Point point;
    double v;
    double phi;
};


Point track[TRACK_SIZE + 1]; //consists of track points
std::vector <Point> car_move; // consists of a real car positions
Car car[POINTS_PER_LINE * TRACK_SIZE][3]; //consists of a car from sensors
Point proection_array[POINTS_PER_LINE * TRACK_SIZE]; // final result of proections

int main()
{
   TraceGeneration();
   CarGeneration();
   Algorithm();

   bool b = CheckResult();
   if (CheckResult())
       std::cout << "Result check : Success";
   else
       std::cout << "Result check : Fail. Accuracy worse than nedded";

   ShowResult();
           
    //time check;
    //auto begin = std::chrono::steady_clock::now();
    //auto end = std::chrono::steady_clock::now();
    //auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    
}

void ShowResult() 
{
    int eps = 1;
    int i = 0;

    for (auto iter = car_move.begin(); iter != car_move.end(); iter++) {
        std::cout << "Initial point: [" << iter->x << ";" << iter->y << "] " << "Result [" << proection_array[i].x << ";" << proection_array[i].y << "]" << std::endl;
        i++;
    }
}

double GetRandomDouble(double a, double b) {
    std::random_device rd;
    std::mt19937 gen;
    gen.seed(rd());

    if (a > b)
        std::swap(a, b);

    std::uniform_real_distribution<> rand(a, b);
    return rand(gen);
}

bool CheckResult() {
    int eps = 1;
    int i = 0;

    for (auto iter = car_move.begin(); iter != car_move.end(); iter++)
        if (abs(iter->x - proection_array[i].x) > eps || abs(iter->y - proection_array[i].y > eps))
            return false;
        else i++;

    return true;
}


Point MakeShot(Point departure, double lenght, double phi) {
    Point p;
    lenght = GetRandomDouble(0, lenght);
    phi = GetRandomPhi(phi) / 180 * PI;
    p.x = round(departure.x + lenght * cos(phi));
    p.y = round(departure.y + lenght * sin(phi));

    return p;
}

//TO DO : make it simpler
Point MoveAlongLine(Point departure, double lenght, double phi) {
    Point p; 
    phi = phi / 180 * PI;
    double step = lenght * cos(phi);

    if (phi >= 90 && phi <= 270)
        step = -step;
    p.x = round(departure.x + step);
    step = lenght * sin(phi);

    if (phi >= 0 && phi <= 180)
        step = -step;
    p.y = round(departure.y + step);
    return p;
}


void DisplayPoint(Point p)
{
    std::cout << "Point x: " << p.x << " y: " << p.y << std::endl;
}

bool CheckRange(Point p1, Point p2, Point p3)
{
    if (p3.x >= p1.x && p3.x <= p2.x || p3.y >= p1.y && p3.y <= p2.y || p3.x <= p1.x && p3.x >= p2.x || p3.y <= p1.y && p3.y >= p2.y)// упростить
        return true;
    else return false;
}


void TraceGeneration()
{
    for (int i = 0; i < sizeof(track) / sizeof(track[0]); i++) {
        track[i].x = GetRandomDouble(60, TRACE_SIZE);
        track[i].y = GetRandomDouble(60, TRACE_SIZE);
    }
}


Point PointNearLineGeneration(Point p1, Point p2)
{
    Point res;
    res.x = GetRandomDouble(p1.x, p2.x);
    res.y = GetRandomDouble(p1.y, p2.y);
    return res;
}

double GetRandomPhi(double phi) {
    double result = -1.0;
    if (phi >= 0 && phi < 90)
        result = GetRandomDouble(0, 89.99);
    else if (phi >= 90 && phi < 180)
        result = GetRandomDouble(90, 179.99);
    else if (phi >= 180 && phi < 270)
        result = GetRandomDouble(180, 269.99);
    else if (phi >= 270 && phi < 360)
        result = GetRandomDouble(270, 359.99);
    else
        std::cout << "Can not create phi" << std::endl;
    return result;
}

//TO DO: not checked yet
void Generation() {

    double step;
    //Point track[2];
    //track[0] = Point{0,0};
    //track[1] = Point{100,100};
    //track[2] = Point{ 300,300};

    Point next_point[POINTS_PER_LINE + 2];
    Vector vec;
    for (int i = 0; i < sizeof(track) / sizeof(track[0]) - 1; i++) {
        step = Vector(track[i], track[i + 1]).GetVectorLenght() / (POINTS_PER_LINE + 1);
        next_point[0] = track[i];
        next_point[POINTS_PER_LINE + 1] = track[i + 1];

        for (int j = 1; j < POINTS_PER_LINE + 1; j++) {
            vec = Vector(next_point[0], next_point[POINTS_PER_LINE + 1]);
            next_point[j] = MoveAlongLine(track[i], step * j, vec.GetAngleToOrtoi());
        }
           

        for (int j = 0; j < POINTS_PER_LINE; j++) {
            vec = Vector(track[i], track[i+1]);
            car[i * POINTS_PER_LINE + j][0].point = PointNearLineGeneration(next_point[j], next_point[j + 2]);
            car[i * POINTS_PER_LINE + j][0].phi = vec.GetAngleToOrtoi();

            //WithoutRandom
            //car[i * POINTS_PER_LINE + j][0].point = MoveAlongLine(next_point[j],GetVectorLenght(next_point[j],next_point[j+1]), GetAngleBetweenVectors(GetVectorbyPoints(next_point[0], next_point[POINTS_PER_LINE]), ortoi));
        }
    }

}

double GetDistance(Point p1, Point p2, Point p3) 
{
    return abs((p2.y - p1.y) * p3.x - (p2.x - p1.x) * p3.y + p2.x * p1.y - p2.y * p1.x) / sqrt(pow(p2.y - p1.y, 2.0) + pow(p2.x - p1.x, 2.0));
}


Point GetProection(Point p1, Point p2, Point p3) 
{
    Point result;
    int lenght_x, lenght_y, x,y;
    double t;
    Vector vec = Vector(p1, p2);
    lenght_x = vec.GetLenght_X();
    lenght_y = vec.GetLenght_Y();
    t = (lenght_x * p3.x + lenght_y * p3.y - lenght_x * p1.x - lenght_y * p1.y) / (pow(lenght_x,2)+pow(lenght_y,2));
    result.x = round(lenght_x * t + p1.x);
    result.y = round(lenght_y * t + p1.y);

    return result;

}


void Algorithm() 
{
    int count = 0;
    double distance1, distance2, angle1, angle2;
    Vector vec;

    for (int i = 0; i < sizeof(car) / sizeof(car[0]); i++)
    {
       distance1, distance2 = -1;

       vec = Vector(track[count + 1], track[count + 2]);
       distance2 = GetDistance(track[count + 1], track[count + 2], car[i]->point);
       angle2 = abs(car[i]->phi - vec.GetAngleToOrtoi());

       vec = Vector(track[count], track[count + 1]);
       distance1 = GetDistance(track[count], track[count + 1], car[i]->point);
       angle1 = abs(car[i]->phi - vec.GetAngleToOrtoi());
 
       //make a desicion
       if (distance2 == -1 && distance1 == -1)
           std::cout << "Can not find a line!";
       else if (distance1 == -1)
           count++;
       else if (distance2 == -1);
       else if (angle2 < angle1)
           count++;

       proection_array[i] = GetProection(track[count], track[count + 1], car[i]->point);

    }
}

//TO DO: durty, make it clearer
void CarGeneration() {
    double step, part_lenght;
    Vector vec;

    Point uniform_points[POINTS_PER_LINE + 2];
    for (int i = 0; i < sizeof(track) / sizeof(track[0]) - 1; i++) {

        step = Vector(track[i], track[i+1]).GetVectorLenght() / (POINTS_PER_LINE + 1);
        part_lenght = Vector(track[i], track[i + 1]).GetVectorLenght() / POINTS_PER_LINE;
        uniform_points[0] = track[i];
        uniform_points[POINTS_PER_LINE + 1] = track[i + 1];
        vec = Vector(uniform_points[0], uniform_points[POINTS_PER_LINE + 1]);
        //Values without random
        for (int j = 1; j < POINTS_PER_LINE + 1; j++)//нужно разбить цикл на несколько или очень красиво все оформить
            uniform_points[j] = MoveAlongLine(track[i], GetRandomDouble(step * j - step, step * j), vec.GetAngleToOrtoi());

        vec = Vector(uniform_points[0], uniform_points[POINTS_PER_LINE + 1]);
        for (int j = 0; j < sizeof(uniform_points) / sizeof(uniform_points[0]) - 2; j++) {
            //Values with random on the line -> That's proection of points on the line
            car[i * POINTS_PER_LINE + j][0].point = MoveAlongLine(uniform_points[j], GetRandomDouble(0, part_lenght), vec.GetAngleToOrtoi());
            // Values for the check later
            car_move.push_back(car[i * POINTS_PER_LINE + j][0].point);

            //Basing on points, making random perpendicular to the line points; It can be done much easier.
            vec = Vector(track[i], track[i + 1]);
            double a = vec.GetAngleToOrtoi();
            while (a > 90)
                a -= 90;

            int line_angle = 90;
            double angle = vec.GetAngleToOrtoi();
            if (angle >= 90 && angle <= 180 || angle >= 270 && angle <= 360)
                line_angle = 0;

            if (GetRandomDouble(0, 1) > 0.5)
                line_angle = line_angle + 180;

            car[i * POINTS_PER_LINE + j][0].point = MoveAlongLine(car[i * POINTS_PER_LINE + j][0].point, GetRandomDouble(0, ACCURACY), line_angle + a);
            car[i * POINTS_PER_LINE + j][0].phi = vec.GetAngleToOrtoi();
        }

    }
}
