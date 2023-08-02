//这个文件是一个方法的运行函数所在
//是对3.2. Projection-based boundary evaluation的复现
# pragma once
#include"../geo/geocal.h"



//从方向得到圆
Circ getCircinSphere(const Sphere& s,const const Vector3d& dir,const double& r){
    double h=sqrt(s.r*s.r-r*r);
    Vector3d center=s.center+h*dir;
    Circ result;
    result.r=r;
    result.center=center;
    result.dir=dir.normalized();
    return result;
}


//球上圆的投影
Circ2D StereographicProjectionofCirc(const Sphere& s,const Circ& c){
    Circ2D result;
    auto Points=getPointinCircofSphere(s,c);
    auto p2D1=StereographicProjectionofPoint(s,Points[0]);
    auto p2D2=StereographicProjectionofPoint(s,Points[1]);
    result.center=(p2D1+p2D2)/2;
    result.r=((p2D1-p2D2)/2).norm();
    return result;
}

//平面点逆投影回到球上圆
Point3D InvStereographicProjectionof(const Sphere& s,const Point2D& p){
    auto r=s.r;
    auto phi=atan2(p.y(),p.x());
    Vector2d tmp;
    tmp<<s.center.x(),s.center.y();
    auto theta=2*atan(2*s.r/(p-tmp).norm());
    Point3DPolar pt;
    pt.r=r;
    pt.theta=theta;
    pt.phi=phi;
    Vector3d result=InvgetPoint3DPolar(pt,s.center);
    return result;
}

