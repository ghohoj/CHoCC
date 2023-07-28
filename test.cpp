//测试相切圆（三个不相交）（1）
#include"trianglesgenerate/Apollonius.h"

void test1(){
    vector<Circ2D> ThreeCirc2D;
    Circ2D tmp;
    tmp.r=0.5;
    tmp.center<<0,0;
    ThreeCirc2D.push_back(tmp);
    tmp.center<<1,sqrt(3);
    ThreeCirc2D.push_back(tmp);
    tmp.center<<2,0;
    ThreeCirc2D.push_back(tmp);
    vector<Point2D> result=getCirc(ThreeCirc2D);
    cout<<(result[0]+result[1]+result[2])/3;
}

//测试相切圆（一个不相交两个相交）（1）


//测试三角剖分



//测试第一个方法划分的三角形效果



//测试采样


//测试求解getCircofLineandCirc(const Circ2D& a,const Line2D& b,const Line2D& c)
void test3(){
    Circ2D tmp;
    tmp.r=0.5;
    tmp.center<<0,0;
    Line2D b,c;
    b.dirc<<0,1;
    b.point<<2,0;
    c.dirc<<0,1;
    c.point<<-2,0;
    auto x=getCircofLineandCirc(tmp,b,c);
    cout<<x[0];
    cout<<x[1];
}

//测试反演变换
void test2(){
    Circ2D tmp;
    tmp.r=0.5;
    tmp.center<<0,1;
    Vector2d p;
    p<<0,0;
    auto x=InversetoCirc2D(p,tmp);
    cout<<x;
}

int main(){
    test1();
}