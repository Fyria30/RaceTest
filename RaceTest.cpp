// RaceTest.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include <cmath> 
#include <cstdlib>
#include <ctime>
#include <random>
#include <windows.h>
#include <chrono>
#include "vector.h"

#define PI 3.14159265
#define TRACK_SIZE 20
#define POINTS_PER_LINE 100
#define ACCURACY 50
#define TRACE_SIZE 500
#define EPS 0.1


void TraceGeneration(); 
void CarGeneration();
Point MoveAlongLine(Point departure, double lenght, double phi); //Return a point moved in lenght on phi angle
void Algorithm(); // Finds proection
void DrawGraphs(); // Draw the graphs (windows)
bool CheckResult(); // Compares initial array of points and proection array
bool PointCompare(Point p1, Point p2, double eps);// Compares  two points


struct Car {
    Point point;
    double v;
    double phi;
};

std::vector <Point> track;
std::vector <Point> car_move; // consists of a real car positions
std::vector <Car> car;//consists of a car from sensors
std::vector <Point> proection_array; // final result of proections

int main()
{
   TraceGeneration();
   CarGeneration();
   Algorithm();

   if (CheckResult())
       std::cout << "Result check : Success";
  else
       std::cout << "Result check : Fail. Accuracy worse than nedded";

    DrawGraphs();
           
    //time check;
    //auto begin = std::chrono::steady_clock::now();
    //auto end = std::chrono::steady_clock::now();
    //auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cin.get();
}


bool PointCompare(Point p1, Point p2, double eps) {

    if (abs(p1.x - p2.x) > eps || abs(p1.y - p2.y) > eps)
        return false;
    else
        return true;
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
    int i = 0;

    //Size check 
    if (proection_array.size() != car_move.size()){
        std::cout << "Arrays do not have the same size";
        return false;
    }

    for (int i =0; i < car_move.size();i++)
        if (!PointCompare(car_move[i],proection_array[i], EPS)){
            std::cout << "does not match";
            return false;
        }
            
    return true;
}


//TO DO : make it simpler
Point MoveAlongLine(Point departure, double lenght, double phi) {
    Point p; 
    phi = phi / 180 * PI;
    double step = lenght * cos(phi);

    if (phi >= 90 && phi <= 270)
        step = -step;
    p.x = departure.x + step;
    step = lenght * sin(phi);

    if (phi >= 0 && phi <= 180)
        step = -step;
    p.y = departure.y + step;
    return p;
}

void TraceGeneration()
{
    Point p;
    for (int i = 0; i < TRACK_SIZE+1; i++) {
        p.x = GetRandomDouble(60, TRACE_SIZE);
        p.y = GetRandomDouble(60, TRACE_SIZE);
        track.push_back(p);
    }
}


Point GetProection(Point p1, Point p2, Point p3) 
{
    Point result;
    double lenght_x, lenght_y, x,y;
    double t;
    Vector vec = Vector(p1, p2);
    lenght_x = vec.GetLenght_X();
    lenght_y = vec.GetLenght_Y();
    t = (lenght_x * p3.x + lenght_y * p3.y - lenght_x * p1.x - lenght_y * p1.y) / (pow(lenght_x,2)+pow(lenght_y,2));
    result.x = lenght_x * t + p1.x;
    result.y = lenght_y * t + p1.y;

    return result;

}

void DrawPoint(HDC hDC, Point p, int r, int g, int b) 
{
    SetPixel(hDC, p.x, p.y, RGB(r, g, b));
    SetPixel(hDC, p.x + 1, p.y, RGB(r, g, b));
    SetPixel(hDC, p.x, p.y + 1, RGB(r, g, b));
    SetPixel(hDC, p.x - 1, p.y, RGB(r, g, b));
    SetPixel(hDC, p.x, p.y - 1, RGB(r, g, b));
    SetPixel(hDC, p.x + 1, p.y + 1, RGB(r, g, b));
    SetPixel(hDC, p.x - 1, p.y - 1, RGB(r, g, b));
    SetPixel(hDC, p.x + 1, p.y - 1, RGB(r, g, b));
    SetPixel(hDC, p.x - 1, p.y + 1, RGB(r, g, b));
}

void DrawGraphs() 
{
    POINT op;
    HWND hWnd = GetConsoleWindow();
    HDC hDC = GetDC(hWnd);
    HPEN cP = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
    SelectObject(hDC, cP);
    MoveToEx(hDC, track[0].x, track[0].y, &op);
   
    for (int i = 0; i < track.size(); i++)
    {
        if (i == 0)
            DrawPoint(hDC, track[i], 255, 255, 255);
        else
            DrawPoint(hDC, track[i], 0, 0, 255);
        LineTo(hDC, track[i].x, track[i].y);
    }

    cP = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    SelectObject(hDC, cP);
    for (int i = 0; i < proection_array.size(); i++)
    {
        DrawPoint(hDC, car[i].point, 255, 0, 0);
        MoveToEx(hDC, car[i].point.x, car[i].point.y, &op);
        LineTo(hDC, proection_array[i].x, proection_array[i].y);
    }

    cP = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    SelectObject(hDC, cP);

    for (auto iter = car_move.begin(); iter != car_move.end(); iter++)
       DrawPoint(hDC, *iter, 0, 255, 0);
    

    ReleaseDC(hWnd, hDC);
}

void Algorithm() 
{
    int count = 0;
    double angle1, angle2;
    Vector current_vec, next_vec;
    Point p1, p2;


    for (int i = 0; i < car.size(); i++) // Разобраться с размерностью массивов
    {
       angle1, angle2 = 360;
 
       if (count + 2 < track.size())
       {
       next_vec = Vector(track[count + 1], track[count + 2]);
       angle2 = abs(car[i].phi - next_vec.GetAngleToOrtoi());
       }

       current_vec = Vector(track[count], track[count + 1]);
       p1 = GetProection(track[count], track[count + 1], car[i].point);
       angle1 = abs(car[i].phi - current_vec.GetAngleToOrtoi());
      
       if (angle2 < angle1 || !current_vec.CheckPoint(p1, EPS))
           count++;

           proection_array.push_back(GetProection(track[count], track[count + 1], car[i].point));


    }
}


//TO DO: durty, make it clearer
void CarGeneration() {
    double step, part_lenght;
    Vector vec;
    Point p;

    Point uniform_points[POINTS_PER_LINE + 2];
    for (int i = 0; i < track.size() - 1; i++) {

        step = Vector(track[i], track[i+1]).GetVectorLenght() / (POINTS_PER_LINE + 1);
        part_lenght = Vector(track[i], track[i + 1]).GetVectorLenght() / POINTS_PER_LINE;
        uniform_points[0] = track[i];
        uniform_points[POINTS_PER_LINE + 1] = track[i + 1];
        vec = Vector(uniform_points[0], uniform_points[POINTS_PER_LINE + 1]);
       
        //Values without random
        for (int j = 1; j < POINTS_PER_LINE + 1; j++){
            uniform_points[j] = MoveAlongLine(track[i], GetRandomDouble(step * j - step, step * j), vec.GetAngleToOrtoi());
        }

        vec = Vector(uniform_points[0], uniform_points[POINTS_PER_LINE + 1]);
        for (int j = 0; j < sizeof(uniform_points) / sizeof(uniform_points[0]) - 2; j++) {
            Point init_point;
                       
            //Values with random on the line -> That's proection of points on the line
            init_point  = MoveAlongLine(uniform_points[j], GetRandomDouble(0, part_lenght), vec.GetAngleToOrtoi());
           
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

            Car c;
            c.point = MoveAlongLine(init_point, GetRandomDouble(0, ACCURACY), line_angle + a);
            c.phi = vec.GetAngleToOrtoi();
            car.push_back(c);
         
        // Values for the check later
        car_move.push_back(init_point);
        }
        

    }
}
