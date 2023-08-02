//这个是对一个关节的复现

# pragma once
#include<Eigen/Dense>
#include<vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
using namespace std;
using namespace Eigen;
struct Interface
{
    vector<Vector3d> dir;//加入的时候一定要先初始化好
    double r;//圆柱的半径
    Vector3d position=Vector3d(0,0,0);
    double R;//需要的最小圆的半径，这里我们使用最小值的2倍
};

//关于这个，文章原话是
// The key idea is to associate an inflated sphere S+ of radius r + ,
// with each ball. S+ has the same center as the ball. r + is computed
// as the smallest radius for which the beams incident on the ball do
// not interfere outside of S+ . To compute r + , we consider all pairs
// of incident beams and for each pair compute a candidate radius
// using line–line intersection between the profiles of both beams
// in the plane that contains their axes. Radius r + is the maximum
// of these candidates. For simplicity, we set r + to be slightly larger
// than this maximum, so as to ensure that the caps (or disks) are
// disjoint.

void getR(Interface& g){
    double anglecos=1;
    for(int i=0;i<g.dir.size();i++){
        for(int j=i+1;j<g.dir.size();j++){
            anglecos=min(g.dir[i].dot(g.dir[j]),anglecos);
        }
    }
    double angle=acos(anglecos);
    double R=g.r/tan(angle/2);
    g.R=2*R;
}

