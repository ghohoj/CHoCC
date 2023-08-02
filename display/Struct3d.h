#pragma once

#include"../geo/geocal.h"
#include"../cal/quadequal.h"
#include"../display/getindata.h"
#include"../geo/geodisplay.h"

#include<algorithm>
#include <numeric>
#include<string>
#include<fstream>
#include <iomanip>
using namespace std;

struct Struct3d
{
    //标记三角形点
    vector<Point3D> ps;
    //表面圆形
    vector<Circ> circs;
    //接口的中心
    Point3D center;
    //标记大三角形
    vector<Vector3i> tris;
    //圆标记的编号
    vector<Vector3i> circsnum;
    
    
    Struct3d(Interface g){
        circs.resize(g.dir.size());
        //初始化那几个圆
        for(int i=0;i<g.dir.size();i++){
            circs[i].dir=g.dir[i];
            circs[i].center=g.position+sqrt(g.R*g.R-g.r*g.r)*g.dir[i];
            center=g.position;
            circs[i].r=g.r;
        }
    }

};


