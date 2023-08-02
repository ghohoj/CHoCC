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
    Sphere(double r,Point3D center):r(r),center(center){}
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
    Plane(Vector3d dirc=Vector3d(0,0,0),Point3D point=Vector3d(0,0,0)):dirc(dirc),point(point){}
};

struct Line2D
{
    Vector2d dirc;
    Point2D point;
};

struct Circ
{
    Point3D center;
    Vector3d dir;
    double r;
    vector<int> passpoint;//这个是用来标记需要经过的点
    vector<int> passface;//这个是用来标记需要经过的面
    Vector3d vertica[2];
    void getvertica(){
        Matrix<double,1,3> equation(dir);
        FullPivLU<Matrix<double,1,3>> solve(equation);
        vertica[0]=(Matrix<double,3,2>(solve.kernel()).col(0)).normalized();
        vertica[1]=dir.cross(vertica[0]).normalized();
    }
    Circ(Point3D center,Vector3d dir,double r):center(center),dir(dir),r(r){
        getvertica();
    }
    Circ(){}
    int geti(Point3D p,const int& num){
        double angle=atan2((p-center).dot(vertica[1]),(p-center).dot(vertica[0]));
        return int(round(angle/(2*M_PI)*num))%num;
    }
};

struct edgeNum:pair<int,int>
{
    //下面的定义是为了方便map做索引
    edgeNum(int i,int j){
        this->first=max(i,j);
        this->second=min(i,j);
    }
    bool operator<(edgeNum a){
        if(this->first<a.first){
            return true;
        }
        else if(this->first==a.first&&this->second<a.second){
            return true;
        }
        else{
            return false;
        }
    }
};