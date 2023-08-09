//这里是对3.3Plane-rolling boundary evaluation的复现
# pragma once

#include"../geo/geocal.h"
#include"../cal/quadequal.h"
#include"../display/getindata.h"
#include"../display/Struct3d.h"
#include<algorithm>
#include <numeric>
#include<map>
#include<stack>
using namespace std;

//这里是对附录A的复现，求解方程
//|N|=1
//3个cos(\alpha+\alpha_i)=N \cdot N_i
pair<double,Vector3d> solveAppendANoZero(const vector<double>& angle,const vector<Vector3d>& dirs,bool special=false){
    pair<double,Vector3d> result;
    Vector3d A=cos(angle[0])*(dirs[1].cross(dirs[2]))+
    cos(angle[1])*(dirs[2].cross(dirs[0]))+
    cos(angle[2])*(dirs[0].cross(dirs[1]));
    Vector3d B=sin(angle[0])*(dirs[1].cross(dirs[2]))+
    sin(angle[1])*(dirs[2].cross(dirs[0]))+
    sin(angle[2])*(dirs[0].cross(dirs[1]));
    double VV=dirs[0].dot(dirs[1].cross(dirs[2]))*dirs[0].dot(dirs[1].cross(dirs[2]));
    double AA=A.dot(A);
    double BB=B.dot(B);
    double AB=A.dot(B);

    double tanangle=(AB-sqrt(max(double(0),AB*AB-(AA-VV)*(BB-VV))))/(BB-VV);//我们只关心那个小的圆锥，不关系外接的圆锥
    if(special){
        tanangle=(AB+sqrt(max(double(0),AB*AB-(AA-VV)*(BB-VV))))/(BB-VV);
    }
    result.first=atan(tanangle);
    result.second=(cos(result.first)*A-sin(result.first)*B)/(dirs[0].dot(dirs[1].cross(dirs[2])));
    return result;
}



pair<double,Vector3d> solveAppendAZero(const vector<double>& angle,const vector<Vector3d>& dirs,bool special=false){
    pair<double,Vector3d> result;
    Vector3d A=cos(angle[0])*(dirs[1].cross(dirs[2]))+
    cos(angle[1])*(dirs[2].cross(dirs[0]))+
    cos(angle[2])*(dirs[0].cross(dirs[1]));
    Vector3d B=sin(angle[0])*(dirs[1].cross(dirs[2]))+
    sin(angle[1])*(dirs[2].cross(dirs[0]))+
    sin(angle[2])*(dirs[0].cross(dirs[1]));
    double VV=dirs[0].dot(dirs[1].cross(dirs[2]))*dirs[0].dot(dirs[1].cross(dirs[2]));
    double AA=A.dot(A);
    double BB=B.dot(B);
    double AB=A.dot(B);

    double tanangle=(AB-sqrt(max(double(0),AB*AB-(AA-VV)*(BB-VV))))/(BB-VV);//我们只关心那个小的圆锥，不关系外接的圆锥
    if(special){
        tanangle=(AB+sqrt(max(double(0),AB*AB-(AA-VV)*(BB-VV))))/(BB-VV);
    }
    result.first=atan(tanangle);
    

    if(!(APequal(dirs[0],dirs[1])||APequal(dirs[0],-dirs[1]))){
        result.second=dirs[0].cross(dirs[1]).normalized();
    }
    else{
        result.second=dirs[1].cross(dirs[2]).normalized();
    }
    if(special){
        result.second=-result.second;
    }
    return result;
}


// 3.4. Solving apollonius’s problem on a sphere的代码复现
vector<Point3D> solveapollonius(const Sphere& s,const vector<double>& angle,const vector<Vector3d>& dirs,bool special=false){
    vector<Point3D> result;
    if(!(angle.size()==3&&dirs.size()==3)){
        cout<<"注意输入格式，三个角度三个方向";
        return result;
    }
    pair<double,Vector3d> solution;
    Point3D tmp;
    if(abs(dirs[0].dot(dirs[1].cross(dirs[2])))>pre3){
        solution=solveAppendANoZero(angle,dirs,special);
    }
    else{
        solution=solveAppendAZero(angle,dirs,special);
    }
    
    for(int i=0;i<3;i++){
        tmp=s.center+s.r*(sin(angle[i])*solution.second+sin(solution.first)*dirs[i])/sin(solution.first+angle[i]);
        result.push_back(tmp);
    }
    return result;
}

Struct3d::Struct3d(Interface g){
    circs.resize(g.dir.size());
    //初始化圆
    for(int i=0;i<g.dir.size();i++){
        circs[i]=Circ(g.position+sqrt(g.R*g.R-g.r*g.r)*g.dir[i],g.dir[i],g.r);
    }
    sphere=Sphere(g.R,g.position);
    Planerolling();
}
void Struct3d::fresh(Interface g){
    ps.clear();
    circs.clear();
    tris.clear();
    circsnum.clear();
    //重新初始化
    circs.resize(g.dir.size());
    //初始化圆
    for(int i=0;i<g.dir.size();i++){
        circs[i]=Circ(g.position+sqrt(g.R*g.R-g.r*g.r)*g.dir[i],g.dir[i],g.r);
    }
    sphere=Sphere(g.R,g.position);
    Planerolling();
}


//3.3中的Plane-rolling算法
void Struct3d::Planerolling(){
    // 找第一个平面
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
    //记录最小的角度,暂时的角度
    double minangle=100,tmpangle;
    //记录圆柱方向
    vector<Point3D> tmpdirs;
    //记录圆柱夹角,由于半径固定，这个值也固定
    vector<double> tmpangles;
    //连接三个点的坐标位置
    vector<Point3D> tmpsolution,solution;
    //记录三个点暂时生成的平面
    Plane tmpplane,plane;
    
    for(int i=1;i<circs.size();i++){
        for(int j=i+1;j<circs.size();j++){
            tmpdirs.clear();
            tmpdirs.push_back(circs[0].dir);
            tmpdirs.push_back(circs[i].dir);
            tmpdirs.push_back(circs[j].dir);
            tmpangles.clear();
            tmpangles.push_back(asin(circs[0].r/sphere.r));
            tmpangles.push_back(asin(circs[i].r/sphere.r));
            tmpangles.push_back(asin(circs[j].r/sphere.r));

            tmpsolution=solveapollonius(sphere,tmpangles,tmpdirs);
            tmpplane=getPlanefromPoint(tmpsolution,circs[0].center);
            tmpangle=angleBetweenPlane(circs[0].dir,tmpplane);
            if(tmpangle<minangle){
                minangle=tmpangle;
                tmpVector3i<<0,i,j;
                solution=tmpsolution;
                plane=tmpplane;
            }
        }
    }

    if(!IfPlanefromPointRight(solution,circs[tmpVector3i.x()].center)){
        auto toswap=tmpVector3i;
        tmpVector3i<<toswap.x(),toswap.z(),toswap.y();
        swap(solution[1],solution[2]);
    }
    circsnum.push_back(tmpVector3i);
    int points_nums=0;//记录点的编号
    //初始化点
    ps.insert(ps.end(), solution.begin(), solution.end());
    //初始化三角形
    tris.push_back(Vector3i(points_nums,points_nums+1,points_nums+2));

    
    //第一个是指代圆的编号，第二个指代点的编号
    map<edgeNum,edgeNum> connect;
    //检测是不是有重复,false代表没有重复，true已经重复过了(避免边重复，遇到)
    map<edgeNum,bool> repeat;
    //避免出现重复的三角形(例如第一个三角形012，第二个三角形寻找的时候避免再出现012)
    map<edgeNum,int> repeattri;
    //第一次进入这条边，记录下边两边点的序号和前面那个三角形的方向和另外一个点的序号
    stack<pair<int,int>> edge_info;
    //初始化边
    edge_info.push(make_pair(tmpVector3i.x(),tmpVector3i.y()));
    edge_info.push(make_pair(tmpVector3i.y(),tmpVector3i.z()));
    edge_info.push(make_pair(tmpVector3i.z(),tmpVector3i.x()));
    repeat[edgeNum(tmpVector3i.x(),tmpVector3i.y())]=false;
    repeat[edgeNum(tmpVector3i.y(),tmpVector3i.z())]=false;
    repeat[edgeNum(tmpVector3i.x(),tmpVector3i.z())]=false;
    repeattri[edgeNum(tmpVector3i.x(),tmpVector3i.z())]=tmpVector3i.y();
    repeattri[edgeNum(tmpVector3i.y(),tmpVector3i.z())]=tmpVector3i.x();
    repeattri[edgeNum(tmpVector3i.x(),tmpVector3i.y())]=tmpVector3i.z();
    map<edgeNum,Vector3d> face;
    face[edgeNum(tmpVector3i.x(),tmpVector3i.z())]=plane.dirc;
    face[edgeNum(tmpVector3i.y(),tmpVector3i.z())]=plane.dirc;
    face[edgeNum(tmpVector3i.x(),tmpVector3i.y())]=plane.dirc;

    points_nums+=3;
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
        //取出边和他的其他信息
        auto edgeint=edge_info.top();
        auto edgekey=edgeNum(edgeint.first,edgeint.second);
        edge_info.pop();


        if(repeat[edgekey]){
            continue;
        }
        //记录这个边已经重复过了，后面不需要再重复
        repeat[edgekey]=true;

        auto dirt=face[edgekey];
        
        minangle=100;
        for(int i=0;i<circs.size();i++){
            if(i==edgekey.first||i==edgekey.second||
            (repeat.count(edgeNum(i,edgekey.first))&&repeat[edgeNum(i,edgekey.first)])||
            (repeat.count(edgeNum(i,edgekey.second))&&repeat[edgeNum(i,edgekey.second)])){
                continue;
            }
            tmpdirs.clear();
            tmpdirs.push_back(circs[i].dir);
            tmpdirs.push_back(circs[edgeint.second].dir);
            tmpdirs.push_back(circs[edgeint.first].dir);
            tmpangles.clear();
            tmpangles.push_back(asin(circs[i].r/sphere.r));
            tmpangles.push_back(asin(circs[edgeint.second].r/sphere.r));
            tmpangles.push_back(asin(circs[edgeint.first].r/sphere.r));

            tmpsolution=solveapollonius(sphere,tmpangles,tmpdirs);
            tmpplane=getPlanefromPoint(tmpsolution);
            tmpangle=angleBetweenPlane(dirt,tmpplane);
            if(tmpangle<minangle){
                //记录点
                minangle=tmpangle;
                tmpVector3i<<i,edgeint.second,edgeint.first;
                solution=tmpsolution;
                plane=tmpplane;
            }
        }
        //修正，有可能是外接圆锥
        if(!IfPlanefromPointRight(plane,circs[tmpVector3i.x()].center)){
            tmpdirs.clear();
            tmpdirs.push_back(circs[tmpVector3i.x()].dir);
            tmpdirs.push_back(circs[tmpVector3i.y()].dir);
            tmpdirs.push_back(circs[tmpVector3i.z()].dir);
            solution=solveapollonius(sphere,tmpangles,tmpdirs,true);
            plane=getPlanefromPoint(tmpsolution);
        }


        circsnum.push_back(tmpVector3i);
        if(repeat.count(edgeNum(edgekey.first,tmpVector3i.x()))==0){
            repeat[edgeNum(edgekey.first,tmpVector3i.x())]=false;
        }
        else{
            repeat[edgeNum(edgekey.first,tmpVector3i.x())]=true;
        }
        if(repeat.count(edgeNum(edgekey.second,tmpVector3i.x()))==0){
            repeat[edgeNum(edgekey.second,tmpVector3i.x())]=false;
        }
        else{
            repeat[edgeNum(edgekey.second,tmpVector3i.x())]=true;
        }

        edge_info.push(make_pair(edgeint.first,tmpVector3i.x()));
        edge_info.push(make_pair(tmpVector3i.x(),edgeint.second));
        repeattri[edgeNum(edgekey.first,tmpVector3i.x())]=edgekey.second;
        repeattri[edgeNum(edgekey.second,tmpVector3i.x())]=edgekey.first;
        face[edgeNum(edgekey.first,tmpVector3i.x())]=plane.dirc;
        face[edgeNum(edgekey.second,tmpVector3i.x())]=plane.dirc;

        //初始化点
        ps.insert(ps.end(), solution.begin(), solution.end());
        
        //初始化三角形
        tris.push_back(Vector3i(points_nums,points_nums+1,points_nums+2));
        //初始化圆圈需要经过的点编号
        points_nums+=3;
    }
}