#pragma once

#include"../geo/geocal.h"
#include"../cal/quadequal.h"
#include<algorithm>
#include <numeric>
#include<string>
#include<fstream>
#include <iomanip>
using namespace std;

struct PartCone
{

};
struct Struct3d
{
    vector<Point3D> ps;//标记点，属于
    vector<Vector3i> tris;//标记大三角形
    vector<PartCone> conePart;//椭圆锥
    vector<Circ> circs;
    vector<Vector3i> objprint;
};
void dealCirc(){

}
void totris(Struct3d& result){
    //对圆形
    for(Circ ci:result.circs){
        dealCirc();
    }
    //处理椭圆锥

}

//写入一个点的字符串
void pointstring(const Point3D& x,fstream& f){
    f<<"v "<<x.x()<<" "<<x.y()<<" "<<x.y()<<" "<<x.z()<<setprecision(5)<<endl;
}
//写入一个面的字符串
//ToDo这里有个天坑就是怎么做好这个面的方向，后面要检查这个事情
void pointstring(const Vector3i& x,fstream& f){
    f<<"f "<<x.x()<<" "<<x.y()<<" "<<x.z()<<endl;
}

//最后的写入文件
void printobj(const Struct3d& result,string u){
    fstream f;
	f.open(u+".obj",ios::out);
    //打印点
    for(auto p:result.ps){
        pointstring(p,f);
    }
    //连线部分
    for(auto l:result.tris){
        pointstring(l,f);
    }
    f.close();
}