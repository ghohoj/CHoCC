#include"generate/Apollonius.h"
#include"Delaunay/Delauat.h"
#include"display/ReadandWrite.h"
#include"display/Struct3d.h"
#include"generate/PlaneRollingBoundary.h"

//测试相切圆（三个不相交）（1）
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
//测试第一个方法划分的三角形效果
void test4(){
    vector<Point2D> list_tmp;
    Point2D tmp;
    tmp<<0,0;
    list_tmp.push_back(tmp);
    tmp<<sqrt(3)/2,0;
    list_tmp.push_back(tmp);
    tmp<<-sqrt(3)/2,0;
    list_tmp.push_back(tmp);
    tmp<<0,1.0/2;
    list_tmp.push_back(tmp);
    tmp<<0,-1.0/2;
    list_tmp.push_back(tmp);
    auto x=Delaunay(list_tmp);
    cout<<x;
}
//测试写入写出
void test5(){
    auto q=readTxt("./data/testshape1.txt");
    cout<<q;
    q=readTxt("./data/testshape2.txt");
    cout<<q;
    print3d result;
    result.p.push_back(Vector3d(0,0,0));
    result.p.push_back(Vector3d(1,0,0));
    result.p.push_back(Vector3d(0,1,0));
    result.p.push_back(Vector3d(0,0,1));

    result.f.push_back(Vector3i(0,1,3));
    result.f.push_back(Vector3i(0,2,1));
    result.f.push_back(Vector3i(3,2,0));
    result.f.push_back(Vector3i(3,1,2));
    printobj(result,"./data/testshape3");
}


//测试ShowGeo的所有前置的函数
void test6(){
    cout<<getp(1,18);
    cout<<getp(20,39);
    vector<Vector3i> tmpV;
    getedgeofcone(getp(1,18),getp(20,38),tmpV);
    print3d result;
    for(int i=0;i<acc;i++){
        result.p.push_back(Vector3d(0,i,0));
    }
    result.p.push_back(Vector3d(1,0,0));
    for(int i=0;i<acc;i++){
        result.p.push_back(Vector3d(0,acc-i-1,1));
    }
    result.p.push_back(Vector3d(1,0,1));
    result.f=tmpV;
    printobj(result,"./data/testshape4");
}


//测试采样（圆形）
void test7(){
    print3d result;
    Circ cir=Circ();
    cout<<cir;
    ShowCirc(cir,result);
    printobj(result,"./data/testshape5");
}

//测试Plane-rolling
void test8(){
    auto q=readTxt("./data/testshape2.txt");
    cout<<q;
    Struct3d x(q);
    cout<<x;
}






int main(){
    test8();
}