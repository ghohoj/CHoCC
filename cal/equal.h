# pragma once
#include<iostream>
#include<Eigen/Dense>
using namespace std;
using namespace Eigen;
//确定两个浮点数差不多一样
const double pre1=10e-3;
const double pre2=10e-6;
const double pre3=10e-9;
//最大值
const double inf=10e9;
bool APequal(const double& a,const double& b,double pre=pre2){
    if(abs(a-b)<pre){
        return true;
    }
    else{
        return false;
    }
}

bool APequal(const Vector3d& a,const Vector3d& b,double pre=pre2){
    if((a-b).norm()<pre){
        return true;
    }
    else{
        return false;
    }
}

int getmod(int a,int b){
    int result=a%b;
    if(result<0){
        return result+b;
    }
    else{
        return result;
    }
}

bool compare(int a1,int a2,int b1,int b2){
    a1++;
    b1++;
    if(a1*b2<a2*b1){
        return true;
    }
    else{
        return false;
    }
}