# pragma once
#include "../geo/geo.h"
#include "../display/getindata.h"
#include"../display/Struct3d.h"
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
    out<<"面向量1:"<<spec.vertica[0].x()<<" "<<spec.vertica[0].y()<<" "<<spec.vertica[0].z()<<endl;
    out<<"面向量2:"<<spec.vertica[1].x()<<" "<<spec.vertica[1].y()<<" "<<spec.vertica[1].z()<<endl;
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

ostream & operator<<(ostream & out,const Interface& infa){
    out<<"类型：接口(未转化为3d文件)"<<endl;
    out<<"位置："<<infa.position.x()<<" "<<infa.position.y()<<" "<<infa.position.z()<<endl;
    out<<"柱子的半径："<<infa.r<<endl;
    out<<"接口所在圆的半径："<<infa.R<<endl;
    for(auto i:infa.dir){
        out<<"柱子方向："<<i.x()<<" "<<i.y()<<" "<<i.z()<<endl;
    }
    return out;
}
//输出vector
template<typename T>
ostream & operator<<(ostream & out,const vector<T>& v){
    out<<"类型：容器"<<endl;
    for(auto i:v){
        out<<i<<" "<<endl;
    }
    out<<endl;
    return out;
}



ostream & operator<<(ostream & out,const Struct3d& v){
    out<<"----------------------"<<endl;
    out<<"类型：接口(处理过,未转化为3d文件)"<<endl;

    out<<endl;
    out<<"接口外部包络形状:"<<endl;
    out<<v.sphere;
    
    out<<endl;
    out<<"表面圆形数目:"<<v.circs.size()<<endl;
    out<<"标记大三角形数目:"<<v.tris.size()<<endl;
    out<<"圆标记的编号数目:"<<v.circsnum.size()<<endl;
    out<<"连线需要的点数目:"<<v.ps.size()<<endl;
    out<<endl;
    out<<"表面圆形:"<<endl;
    for(auto i:v.circs){
        out<<i<<endl;
    }
    out<<"标记大三角形:"<<endl;
    for(auto i:v.tris){
        out<<i.x()<<" "<<i.y()<<" "<<i.z()<<endl;
    }
    out<<"圆标记的编号:"<<endl;
    for(auto i:v.circsnum){
        out<<i.x()<<" "<<i.y()<<" "<<i.z()<<endl;
    }
    out<<"连线需要的点:"<<endl;
    for(auto i:v.ps){
        out<<i.x()<<" "<<i.y()<<" "<<i.z()<<endl;
    }
    out<<"----------------------"<<endl;
    return out;
}