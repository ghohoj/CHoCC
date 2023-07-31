//这里是对3.3Plane-rolling boundary evaluation的复现
#pragma once

#include"../geo/geocal.h"
#include"../cal/quadequal.h"
#include"../display/getindata.h"
#include<algorithm>
#include <numeric>
#include<map>
#include<stack>
using namespace std;

//这里是对附录A的复现，求解方程
//|N|=1
//3个cos(\alpha+\alpha_i)=N \cdot N_i
pair<double,Vector3d> solveAppendA(const vector<double>& angle,const vector<Vector3d>& dirs){
    pair<double,Vector3d> result;
    Vector3d A=(cos(angle[0])*(dirs[1].cross(dirs[2]))+
    cos(angle[1])*(dirs[2].cross(dirs[0]))+
    cos(angle[2])*(dirs[0].cross(dirs[1])))/dirs[0].dot(dirs[1].cross(dirs[2]));
    Vector3d B=(sin(angle[0])*(dirs[1].cross(dirs[2]))+
    sin(angle[1])*(dirs[2].cross(dirs[0]))+
    sin(angle[2])*(dirs[0].cross(dirs[1])))/dirs[0].dot(dirs[1].cross(dirs[2]));

    double AA=A.dot(A);
    double BB=B.dot(B);
    double AB=A.dot(B);
    double tanangle=(AB-sqrt(AB*AB-(AA-1)*(BB-1)))/(BB-1);//我们只关心那个小的圆锥，不关系外接的圆锥
    result.first=atan(tanangle);
    result.second=cos(result.first)*A-sin(result.first)*B;
    return result;
}

// 3.4. Solving apollonius’s problem on a sphere的代码复现
vector<Point3D> solveapollonius(const Sphere& s,const vector<double>& angle,const vector<Vector3d>& dirs){
    vector<Point3D> result;
    if(!(angle.size()==3&&dirs.size()==3)){
        cout<<"注意输入格式，三个角度三个方向";
        return result;
    }
    Point3D tmp;
    auto solution=solveAppendA(angle,dirs);
    for(int i=0;i<3;i++){
        tmp=s.center+s.r*(sin(angle[i])*solution.second+sin(solution.first)*dirs[i])/sin(solution.first+angle[i]);
        result.push_back(tmp);
    }
    return result;
}

//3.3中的Plane-rolling算法
void Planerolling(getdata& g){
    getR(g);
    auto sphere=Sphere(g.R,Point3D(0,0,0));
    //找第一个平面
    //     The initialization step picks a random circle, C i . Let Π i denote
    // the plane that contains it. Then, we consider all possible pairs
    // from the remaining circles, C j and C k , forming a triplet of circles
    // with C i . On each such triplet of circles, we can rest two support-
    // ing planes, which we denote by Π ijk (Fig. 5). We describe in the
    // next section a closed-form solution to compute these supporting
    // planes. We pick the pair (C j , C k ) that minimizes the angle between
    // the normals of Π i and of Π ijk . Intuitively, we want to minimize
    // how much Π i must ‘‘roll’’ before it rests on three circles. The three
    // points on which Π i rests are the vertices of the initial triangle,
    // which we add to the boundary B. We also add the three edges of
    // the triangle to a list of exposed borders.
    Vector3i tmpVector3i;
    double minangle=100;//记录最小的角度
    double tmpangle;//记录暂时的角度
    vector<Point3D> tmpdirs;//记录圆柱方向
    vector<double> tmpangles;//记录圆柱夹角
    vector<Point3D> tmpsolution;
    vector<Point3D> solution;
    Plane tmpplane;
    tmpangles.push_back(asin(g.r/g.R));
    tmpangles.push_back(asin(g.r/g.R));
    tmpangles.push_back(asin(g.r/g.R));
    for(int i=1;i<g.dir.size();i++){
        for(int j=i+1;j<g.dir.size();j++){
            tmpdirs.clear();
            tmpdirs.push_back(g.dir[0]);
            tmpdirs.push_back(g.dir[i]);
            tmpdirs.push_back(g.dir[j]);
            tmpsolution=solveapollonius(sphere,tmpangles,tmpdirs);
            tmpplane=getPlanefromPoint(tmpsolution);
            tmpangle=angleBetweenPlane(Plane(g.dir[0],Vector3d(0,0,0)),tmpplane);
            if(tmpangle<minangle){
                tmpVector3i<<0,i,j;
                solution=tmpsolution;
            }
        }
    }
    vector<Vector3d> points;
    points.insert(points.end(), solution.begin(), solution.end());
    
    map<edgeNum,edgeNum> connect;//第一个是指代圆的编号，第二个指代点的编号
    map<edgeNum,bool> repeat;//检测是不是有重复,false代表没有重复，true已经重复过了
    stack<pair<edgeNum,Vector3d>> edge_info;//第一次进入这条边，记录下
    //初始化边
    edge_info.push(make_pair(edgeNum(tmpVector3i.x(),tmpVector3i.y()),tmpplane.dirc));
    edge_info.push(make_pair(edgeNum(tmpVector3i.y(),tmpVector3i.z()),tmpplane.dirc));
    edge_info.push(make_pair(edgeNum(tmpVector3i.x(),tmpVector3i.z()),tmpplane.dirc));
    repeat[edgeNum(tmpVector3i.x(),tmpVector3i.y())]=false;
    repeat[edgeNum(tmpVector3i.y(),tmpVector3i.z())]=false;
    repeat[edgeNum(tmpVector3i.x(),tmpVector3i.z())]=false;
    //现在开始贪婪的寻找边
    //     Then, as long as the list of exposed borders is not empty,
    // we pop one of the exposed borders from the list, say between
    // circles C i and C j , use Π ijk , the plane of the triangle adjacent to
    // that border, and find a circle C l such that the angle between the
    // normals of plane Π ijk and the supporting plane Π jil is minimum.
    // Then, we add the triangle corresponding to plane Π jil to the
    // boundary B, and check whether one or both of its two edges that
    // touch C l match edges already in the list of exposed borders. If an
    // edge is already in the list, we remove it. Otherwise, we add it.
    while(!edge_info.empty()){
        auto edge_and_dirt=edge_info.top();
        edge_info.pop();
        auto edge=edge_and_dirt.first;
        auto dirt=edge_and_dirt.second;
        repeat[edge]=true;
        int ;
        for(int i=0;i<g.dir.size();i++){
            if(i==edge.first||i==edge.second||repeat[edgeNum(i,edge.first)]){
                continue;
            }

        }
    }


    //从repeat中取出所有的边
}