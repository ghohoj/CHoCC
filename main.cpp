// 这个文件用于展示结果

#include<iostream>
#include"generate/Apollonius.h"
#include"Delaunay/Delauat.h"
#include"display/ReadandWrite.h"
#include"display/Struct3d.h"
#include"mesh/CHoCCMesh.h"
#include"generate/PlaneRollingBoundary.h"
#include"mesh/CNTMesh.h"
using namespace std;

//下面两个函数运行可以得到
void CHoCCInterface(string dir_address,string result_address){
    auto q=readTxt(dir_address);
    cout<<q;
    Struct3d x(q);
    cout<<x;
    print3d result;
    CHoCC::ShowCHoCC(x,result);
    printobj(result,result_address);
}
void ACHoCCInterface(string dir_address,string result_address){
    auto q=readTxt(dir_address);
    cout<<q;
    Struct3d x(q);
    cout<<x;
    print3d result;
    ACHoCC::ShowACHoCC(x,result);
    printobj(result,result_address);
}
void CNTInterface(string dir_address,string result_address){
    auto q=readTxt(dir_address);
    cout<<q;
    Struct3d x(q);
    cout<<x;
    print3d result;
    CNT::ShowCNT(x,result);
    printobj(result,result_address);
}

void getCHoCCMesh(string result_address){
    CHoCCMesh tmp;
    print3d result;
    tmp.getmesh();
    tmp.getmeshresult(result);
    printobj(result,result_address);
}

void getCNTMesh(string result_address){
    CNTMesh tmp;
    print3d result;
    tmp.getmesh();
    tmp.getmeshresult(result);
    printobj(result,result_address);
}

int main()
{
    //要运行哪个函数请将下面的函数注释去掉

    // CHoCCInterface("./data/testshape3.txt","./result/CHoCCInterface");
    // CNTInterface("./data/testshape3.txt","./result/CNTInterface");
    // ACHoCCInterface("./data/testshape3.txt","./result/ACHoCCInterface");
    // getCHoCCMesh("./result/CHoCCMesh");
    // getCNTMesh("./result/CNTMesh");
}