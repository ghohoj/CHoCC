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
//这个测试里面的函数已经被放到类的private了
// void test6(){
//     int acc=5;
//     cout<<getp(0,acc-1);
//     cout<<getp(acc+1,2*acc);
//     vector<Vector3i> tmpV;
//     getedgeofcone(getp(0,acc-1),getp(acc+1,2*acc),tmpV);
//     print3d result;
//     for(int i=0;i<acc;i++){
//         result.p.push_back(Vector3d(0,i,0));
//     }
//     result.p.push_back(Vector3d(1,0,0));
//     for(int i=0;i<acc;i++){
//         result.p.push_back(Vector3d(0,acc-i-1,1));
//     }
//     result.p.push_back(Vector3d(1,0,1));
//     result.f=tmpV;
//     printobj(result,"./data/testshape4");
// }
//测试采样（圆形）
//这个测试里面的函数已经被放到类的private了
// void test7(){
//     print3d result;
//     Circ cir=Circ();
//     cout<<cir;
//     ShowCirc(cir,result);
//     printobj(result,"./data/testshape5");
// }

//测试Plane-rolling
void test8(){
    auto q=readTxt("./data/testshape2.txt");
    cout<<q;
    Struct3d x(q);
    cout<<x;

    q=readTxt("./data/testshape1.txt");
    cout<<q;
    Struct3d y(q);
    cout<<y;
}

//apollonius的测试
void test9(){
    Sphere s;
    s.r=10;
    vector<double> angle;
    angle.push_back(asin(1/s.r));
    angle.push_back(asin(1/s.r));
    angle.push_back(asin(1/s.r));
    vector<Vector3d> dirs;
    dirs.push_back(Vector3d(1,0,0));
    dirs.push_back(Vector3d(0,1,0));
    dirs.push_back(Vector3d(0,0,1));
    cout<<solveapollonius(s,angle,dirs);
}
//测试使用testshape2展示一下接口

void test10(){
    auto q=readTxt("./data/testshape2.txt");
    cout<<q;
    Struct3d x(q);
    cout<<x;
    print3d result;
    CHoCC::ShowCHoCC(x,result);
    printobj(result,"./data/testshape6");
}
//测试圆的geti
void test11(){
    Circ c;
    c.getvertica();
    cout<<c;
    cout<<c.getangle(Vector3d(0.5,0.5,0));
    cout<<c.geti(Vector3d(0.5,0.5,0),8)<<endl;
}
//测试使用testshape1展示一下接口
void test12(){
    auto q=readTxt("./data/testshape1.txt");
    cout<<q;
    Struct3d x(q);
    cout<<x;
    print3d result;
    CHoCC::ShowCHoCC(x,result);
    printobj(result,"./data/testshape7");
}
//CNT的3d文件展示
void test13(){
    auto q=readTxt("./data/testshape1.txt");
    cout<<q;
    Struct3d x(q);
    cout<<x;
    print3d result;
    CNT::ShowCNT(x,result);
    printobj(result,"./data/testshape8");
}
//测试ACHoCC
void test14(){
    auto q=readTxt("./data/testshape1.txt");
    cout<<q;
    Struct3d x(q);
    cout<<x;
    print3d result;
    ACHoCC::ShowACHoCC(x,result);
    printobj(result,"./data/testshape9");
}

void test15(){
    auto s=Sphere(10);
    auto tmp=s.getSurPoint(Vector3d(1,1,1));
    cout<<tmp;
}

int main(){
    test14();
}