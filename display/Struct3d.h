# pragma once
#include"../geo/geo.h"
#include"../display/getindata.h"
#include<algorithm>
#include <numeric>
#include<string>
#include<fstream>
#include<vector>
#include <iomanip>
using namespace std;

struct Struct3d
{
    Struct3d(Interface g);
    Struct3d(){}
    void fresh(Interface g);
    void Planerolling();
    //标记三角形点
    vector<Point3D> ps;
    //表面圆形
    vector<Circ> circs;
    //标记大三角形
    vector<Vector3i> tris;
    //圆标记的编号
    vector<Vector3i> circsnum;
    Sphere sphere;
    
    
};


