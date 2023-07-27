# pragma once

#include<iostream>
#include<Eigen/Dense>
using namespace std;
using namespace Eigen;
//确定两个浮点数差不多一样
const double pre1=10e-3;
const double pre1=10e-6;
const double pre2=10e-9;
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

