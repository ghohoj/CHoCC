# pragma once
#include "../geo/geo.h"
#include<iostream>
using namespace std;

ostream & operator<<(ostream & out,const Triangle& spec){
    out<<"类型：三角形"<<endl;
    out<<"第一个点坐标"<<spec.a.x()<<" "<<spec.a.y()<<" "<<spec.a.z()<<endl;
    out<<"第二个点坐标"<<spec.b.x()<<" "<<spec.b.y()<<" "<<spec.b.z()<<endl;
    out<<"第三个点坐标"<<spec.c.x()<<" "<<spec.c.y()<<" "<<spec.c.z()<<endl;
    return out;
}

ostream & operator<<(ostream & out,const Circ& spec){
    out<<"类型：圆形"<<endl;
    out<<"所在法向量："<<spec.dir.x()<<" "<<spec.dir.y()<<" "<<spec.dir.z()<<endl;
    out<<"半径："<<spec.r<<endl;
    out<<"圆心："<<spec.center.x()<<" "<<spec.center.y()<<" "<<spec.center.z()<<endl;
    return out;
}

ostream & operator<<(ostream & out,const Sphere& spec){
    out<<"类型：球"<<endl;
    out<<"半径："<<spec.r<<endl;
    out<<"圆心："<<spec.center.x()<<" "<<spec.center.y()<<" "<<spec.center.z()<<endl;
    return out;
}

ostream & operator<<(ostream & out,const Circ2D& c){
    out<<"类型：平面上的圆"<<endl;
    out<<"半径："<<c.r<<endl;
    out<<"圆心："<<c.center.x()<<" "<<c.center.y()<<endl;
    return out;
}