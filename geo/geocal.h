# pragma once
#include "../geo/geooutput.h"
#include<vector>
#include"../cal/equal.h"

//利用三个点生成平面
Plane getPlanefromPoint(const Point3D& a,const Point3D& b,const Point3D& c){
    Plane result;
    result.point=a;
    result.dirc=(b-a).cross(c-a);
    return result;
}
Plane getPlanefromPoint(const vector<Point3D>& ps){
    return getPlanefromPoint(ps[0],ps[1],ps[2]);
}

//这里的o点要求在向量的反面
Plane getPlanefromPoint(const vector<Point3D>& ps,const Point3D& o){
    Plane tmp=getPlanefromPoint(ps[0],ps[1],ps[2]);
    if((tmp.point-o).dot(tmp.dirc)<0){
        tmp.dirc=-tmp.dirc;
    }
    return tmp;
}

bool IfPlanefromPointRight(const vector<Point3D>& ps,const Point3D& o){
    Plane tmp=getPlanefromPoint(ps[0],ps[1],ps[2]);
    if((tmp.point-o).dot(tmp.dirc)<0){
        return false;
    }
    return true;//不需要交换点
}

//平面截取圆上点，一般有两个点
vector<Point3D> getPonintfromPlanandCirc(Plane p,Circ c){
    vector<Point3D> result;
    return result;
}

//判断两条线段会不会相交(这两条直线已经确认在同一个平面了)
//Todo
// bool LineSegmentIntersect(const Vector3d& a1,const Vector3d& a2,const Vector3d& b1,const Vector3d& b2){
    
// }

//确认点的顺序,使得其连接为一个凸多边形
// void pointSeq(vector<Point3D>& points1,vector<Point3D>& points2){

// }

//找到顶点
Point3D findPoint(const vector<Point3D>& points1,const vector<Point3D>& points2){
    Point3D x;
    return x;
}

//两条射线求角平分线
Vector3d findAngularBisector(const Point3D& point1,const Point3D& point2,const Point3D& top){
    Vector3d temp1=(top-point1).normalized();
    Vector3d temp2=(top-point2).normalized();
    Vector3d result=(temp1+temp2).normalized();
    return result;
}

//3d球上的极坐标转换
Point3DPolar getPoint3DPolar(const Point3D& p,const Point3D& center=Vector3d(0,0,0)){
    Point3DPolar result;
    Vector3d temp=p-center;
    result.r=temp.norm();
    double r_temp=sqrt(temp.x()*temp.x()+temp.y()*temp.y());
    result.theta=acos(temp.z()/r_temp);
    result.phi=atan2(temp.y(),temp.x());
    return result;
}

//3d球上的极坐标转换的逆变换
Point3D InvgetPoint3DPolar(Point3DPolar p,const Point3D& center=Vector3d(0,0,0)){
    Point3D result;
    result<<center.x()+p.r*sin(p.theta)*cos(p.phi),
    center.y()+p.r*sin(p.theta)*sin(p.phi),
    center.z()+p.r*cos(p.theta);
    return result;
}

Vector3d Sphere::getSurPoint(const Vector3d& p){
    auto pp=getPoint3DPolar(p,center);
    pp.r=r;
    return InvgetPoint3DPolar(pp,center);
}

//球面的点投影
Point2D StereographicProjectionofPoint(const Sphere& s,const Point3D& p){
    Point2D result;
    Point3DPolar temp=getPoint3DPolar(p,s.center);
    if(!APequal(temp.r,s.r)){
        cout<<"这个点不再球面上";
        cout<<s;
        cout<<"点的坐标"<<endl<<p<<endl;
        cout<<"点距离球的半径"<<temp.r<<endl;
        return result;
    }
    if(temp.theta<0.001){
        cout<<"请不要使用如此靠近极点的圆";
    }
    double r=2*s.r/tan(temp.theta/2);
    result<<r*cos(temp.phi),r*sin(temp.phi);
    return result;
}

//计算球上圆最上面和最下面的两个点
vector<Point3D> getPointinCircofSphere(const Sphere& s,const Circ& c){
    vector<Point3D> result;
    if(APequal(c.dir.x(),0,pre2)&&APequal(c.dir.y(),0,pre2)){
        result.push_back(s.center+Vector3d(1,0,0)*c.r);
        result.push_back(s.center+Vector3d(-1,0,0)*c.r);
        return result;
    }
    Vector3d zdir;
    zdir<<0,0,1;
    Vector3d x=(c.dir.cross(zdir)).cross(c.dir).normalized();
    if(x.z()<0){
        x=-x;
    }
    result.push_back(c.center+x*c.r);//上面的点
    result.push_back(c.center-x*c.r);//下面的点
    return result;
}
//三角形面积
double getTrianglearea(const Triangle2D& t){
    auto t1=t.a-t.b;
    auto t2=t.c-t.b;
    auto area=abs(t1.x()*t2.y()-t2.x()*t1.y())/2;
    return area;
}

// 判断一个点是不是在三角形里面
bool inTriangle(const Point2D& p,const Triangle2D& t){
    auto area=getTrianglearea(t);
    Triangle2D tmp=t;
    tmp.a=p;
    auto area1=getTrianglearea(tmp);
    tmp.a=t.a;
    tmp.b=p;
    auto area2=getTrianglearea(tmp);
    tmp.b=t.b;
    tmp.c=p;
    auto area3=getTrianglearea(tmp);
    if(APequal(area,area1+area2+area3,pre1)){
        return true;
    }
    return false;
}


//判断一点是否在外接圆内（包括在外接圆上
bool pointInCircumcircle(const Triangle2D& t,const Point2D& p) {
    // 计算三角形的三边长度
    double x1=t.a.x(),y1=t.a.y(),x2=t.b.x(),y2=t.b.y(),x3=t.c.x(),y3=t.c.y();
    double x=p.x(),y=p.y();
    double a = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
    double b = std::sqrt(std::pow(x3 - x2, 2) + std::pow(y3 - y2, 2));
    double c = std::sqrt(std::pow(x1 - x3, 2) + std::pow(y1 - y3, 2));
    
    // 计算三角形的半周长和面积
    double s = (a + b + c) / 2.0;
    double area = std::sqrt(s * (s - a) * (s - b) * (s - c));
    
    // 计算外接圆的半径和圆心坐标
    double r = a * b * c / (4.0 * area);
    double a1 = 2 * (x2 - x1), a2 = 2 * (y2 - y1), a3 = x2 * x2 + y2 * y2 - x1 * x1 - y1 * y1,
           b1 = 2 * (x3 - x2), b2 = 2 * (y3 - y2), b3 = x3 * x3 + y3 * y3 - x2 * x2 - y2 * y2;

    double xc = -(a2 * b3 - a3 * b2) / (a1 * b2 - a2 * b1);
    double yc = (a1 * b3 - a3 * b1) / (a1 * b2 - a2 * b1);
    // std::cout << r<<" " << xc<<" " << yc<<endl;
    // 判断点是否在外接圆上
    double d = std::sqrt(std::pow(x - xc, 2) + std::pow(y - yc, 2));
    return d <= r;
}
//判断一个点是不是在圆中
bool inCirc(const Point2D& p,const Circ2D& c){
    if((c.center-p).norm()<c.r){
        return true;
    }
    else{
        return false;
    }
}

//计算一个点映射到一个圆上的那个点
Point2D getPointonCirc(const Point2D& p,const Circ2D& c){
    auto d=(p-c.center).normalized();
    auto result=c.center+d*c.r;
    return result;
}


//平行线的距离
double LineandLine(Line2D l1,Line2D l2){
    auto dir=l1.dirc.normalized();
    auto result=(l1.point-l2.point).dot(dir);
    result=sqrt((l1.point-l2.point).squaredNorm()-result*result);
    return result;
}


//面之间的角度
double angleBetweenPlane(Plane x,Plane y){
    double anglecos=x.dirc.normalized().dot(y.dirc.normalized());
    anglecos=max(double(-1),anglecos);
    anglecos=min(double(1),anglecos);
    return acos(anglecos);
}
//面之间的角度
double angleBetweenPlane(Vector3d dir,Plane y){
    double anglecos=dir.normalized().dot(y.dirc.normalized());
    anglecos=max(double(-1),anglecos);
    anglecos=min(double(1),anglecos);
    return acos(anglecos);
}