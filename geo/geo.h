# pragma once
#include<iostream>
#include<ostream>
#include<Eigen/Dense>
using namespace std;
using namespace Eigen;

typedef Vector3d Point3D;
typedef Vector2d Point2D;

struct Circ2D
{
    Point2D center;
    double r;
};

struct Point3DPolar 
{
    double r;
    double theta;
    double phi;
};

struct Sphere
{
    double r;
    Point3D center;
};

struct Cone
{
    Point3D top;
    Vector3d dir;
    double h1,h2;
    double r;
};

struct InfCone
{
    Point3D top;
    Vector3d dir;
    double angle;
};


struct Triangle
{
    Point3D a;
    Point3D b;
    Point3D c;
};

struct Triangle2D
{
    Point2D a;
    Point2D b;
    Point2D c;
};

struct Circ
{
    Point3D center;
    Vector3d dir;
    double r;
};



struct Plane
{
    Vector3d dirc;
    Point3D point;
};

struct Line2D
{
    Vector2d dirc;
    Point2D point;
};