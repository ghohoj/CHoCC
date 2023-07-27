//解决三个的圆的相切圆计算问题


#pragma once
#include"geo/geocal.h"
#include<algorithm>
#include <numeric>
using namespace std;
const double R=100;

//判断是不是在圆上面
bool PointinCirc(Point2D p,Circ2D c){
    if(APequal((c.center-p).norm(),c.r,pre1)){
        return true;
    }
    else{
        return false;
    }
}
//反演到点
Point2D InverseP2P(Point2D p,Point2D pt){
    Point2D result;
    auto tmp=pt-p;
    result=R*R/tmp.norm()*tmp.normalized()+p;
    return result;
}

//反演到直线
Line2D InversetoLine2D(Point2D p,Circ2D c,bool norcheck=true){
    Line2D result;
    if(!PointinCirc(p,c)&&norcheck){
        cout<<"这里不太适合反演为圆";
        return result;
    }
    Matrix2d myRotate;
    myRotate<<0,1,-1,0;
    result.dirc=myRotate*(c.center-p);
    result.point=R*R/(2*c.r)*(c.center-p).normalized()+p;
    return result;
}

//反演到圆形
Circ2D InversetoCirc2D(Point2D p,Circ2D c){
    auto p1=InverseP2P(p,c.center+c.r*(p-c.center).normalized());
    auto p2=InverseP2P(p,c.center-c.r*(p-c.center).normalized());
    Circ2D result;
    result.center=(p1+p2)/2;
    result.r=(p2-p1).norm()/2;
    return result;
}

//判断三个圆的距离关系,返回的result表示第几个三角形被孤立了
int  sortDirofCirc(vector<Circ2D>& ThreeCirc2D){
    int result;
    vector<int> vIndex(3); //序号
    iota(vIndex.begin(),vIndex.end(),0);//递增赋值
    
    vector<double> vData(3);
    vData[0]=(ThreeCirc2D[1].center-ThreeCirc2D[2].center).norm()-ThreeCirc2D[2].r-ThreeCirc2D[1].r;
    vData[1]=(ThreeCirc2D[0].center-ThreeCirc2D[2].center).norm()-ThreeCirc2D[0].r-ThreeCirc2D[2].r;
    vData[2]=(ThreeCirc2D[0].center-ThreeCirc2D[1].center).norm()-ThreeCirc2D[0].r-ThreeCirc2D[1].r;
    sort(vIndex.begin(),vIndex.end(),[&vData](int a,int b){ return vData[a]>vData[b];});
    return result;
}



//找到两个平行线段和一个圆的相切圆位置
Circ2D getCircofLineandCirc(const Circ2D& a,const Line2D& b,const Line2D& c){
    Circ2D result;
    if(!(APequal(b.dirc.x(),c.dirc.x(),pre1)&&APequal(b.dirc.y(),c.dirc.y(),pre1))){
        cout<<"这两个线不平行啊";
        return result;
    }
    Line2D centerline;
    centerline.point=(b.point+c.point)/2;
    centerline.dirc=(b.dirc+c.dirc)/2;
    auto d=LineandLine(b,c);
    //下面开始求解圆的中心在那里，核心是解出一个一元二次方程
}

//三个不相交的圆求外切圆,外接小圆的3个切点
vector<Point2D> getCirc(vector<Circ2D>& ThreeCirc2D){
    vector<Point2D> result;
    //整理三个圆形的顺序，使最前面一个Inverse后变成圆，剩下两个变成直线
    auto tmpint=sortDirofCirc(ThreeCirc2D);
    auto tmp=ThreeCirc2D[0];
    if(tmpint!=0){
        ThreeCirc2D[0]=ThreeCirc2D[tmpint];
        ThreeCirc2D[tmpint]=tmp;
    }

    //变换中心
    auto dirc2c=(ThreeCirc2D[2].center-ThreeCirc2D[1].center);
    auto p=ThreeCirc2D[1].center+dirc2c.normalized()*(ThreeCirc2D[1].r+(dirc2c.norm()-ThreeCirc2D[1].r-ThreeCirc2D[2].r)/2);

    auto Line1_inverse=InversetoLine2D(p,ThreeCirc2D[1],false);
    auto Line2_inverse=InversetoLine2D(p,ThreeCirc2D[2],false);

    tmp.r=tmp.r+(dirc2c.norm()-ThreeCirc2D[1].r-ThreeCirc2D[2].r)/2;
    Circ2D Circ_inverse=InversetoCirc2D(p,tmp);
    Circ2D resultCirc=getCircofLineandCirc(Circ_inverse,Line1_inverse,Line2_inverse);
    Point2D resultcenter=resultCirc.center;
    Point2D tmpp;
    Point2D tmpp=getPointonCirc(resultcenter,ThreeCirc2D[0]);
    result.push_back(tmpp);
    Point2D tmpp=getPointonCirc(resultcenter,ThreeCirc2D[1]);
    result.push_back(tmpp);
    Point2D tmpp=getPointonCirc(resultcenter,ThreeCirc2D[2]);
    result.push_back(tmpp);
    return result;
}