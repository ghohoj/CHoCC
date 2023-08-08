# pragma once
#include<iostream>
#include<vector>
#include<math.h>
#include<map>
#include"../geo/geocal.h"
#include"../display/Struct3d.h"
using namespace std;


// 这里是CNT的初始化方式
// 原文
// The first approach that we have considered starts by sampling
// all of the circular edges of the selected portion of the ACHoCC.
// In this version, the sampling of the two circular edges of each
// chopped-beam may be coordinated so that the beam will be
// tessellated by a quad-loop. But the samplings of the circular edges
// of a chopped-ball are not coordinated. Then, for each solid, it
// computes the convex hull of the sample points on all the circular
// edges of that solid. This is trivial for the beams, but requires
// computing the convex hull of sn cospherical points for a ball with
// n incident beams, assuming an average of s samples per circle.
// This approach has higher computational cost and more faces on
// the polyhedral approximation of the CHoCC of a ball than the
// approach proposed next.


class CNT{

    static void ShowTriDirt(Struct3d& s,print3d& result){
        int pnum=result.p.size();
        for(int i=0;i<s.ps.size();i++){
            result.p.push_back(s.ps[i]);
        }
        for(int i=0;i<s.tris.size();i++){
            result.f.push_back(s.tris[i]);
        }
        
    }


    static void mysort(vector<int>& x,vector<double>& angles){
        for(int i=0;i<x.size();i++){
            for(int j=i+1;j<x.size();j++){
                if(angles[i]<angles[j]){
                    swap(angles[i],angles[j]);
                    swap(x[i],x[j]);
                }
            }
        }
    }

    static void ShowCirFace(Struct3d& s,print3d& result,map<double, vector<int>>& tmp)
    {
        int pnum=result.p.size();
        
        int j=0;
        //将这些点归类到各个圆形上面
        vector<vector<int>> x(s.circs.size());
        for(int i=0;i<s.circsnum.size();i++){
            x[s.circsnum[i].x()].push_back(j++);
            x[s.circsnum[i].y()].push_back(j++);
            x[s.circsnum[i].z()].push_back(j++);
        }
        //生成多边形
        vector<double> angles;
        for(int i=0;i<s.circs.size();i++){
            angles.clear();
            //多边形的中心点
            Vector3d centerpoint(0,0,0);
            for(int j=0;j<x[i].size();j++){
                auto tmp=s.circs[i].getangle(s.ps[x[i][j]]);
                angles.push_back(tmp);
                centerpoint+=s.ps[x[i][j]]/x[i].size();
            }
            mysort(x[i],angles);
            //加入多边形的中心点
            result.p.push_back(centerpoint);
            for(j=0;j<angles.size();j++){
                result.f.push_back(Vector3i(i,x[i][j],x[i][(j+1)%angles.size()])+Vector3i(pnum,0,0));
                tmp[Veckey(s.circs[i].dir)].push_back(x[i][j]);
            }
        }
    }

    static void ShowEdge(map<edgeNum,vector<int>>& ms,Struct3d& s){
        for(int i=0;i<s.circsnum.size();i++){
            ms[edgeNum(s.circsnum[i].x(),s.circsnum[i].y())].push_back(
                s.tris[i].x()
            );
            ms[edgeNum(s.circsnum[i].x(),s.circsnum[i].y())].push_back(
                s.tris[i].y()
            );
            ms[edgeNum(s.circsnum[i].y(),s.circsnum[i].z())].push_back(
                s.tris[i].y()
            );
            ms[edgeNum(s.circsnum[i].y(),s.circsnum[i].z())].push_back(
                s.tris[i].z()
            );
            ms[edgeNum(s.circsnum[i].x(),s.circsnum[i].z())].push_back(
                s.tris[i].z()
            );
            ms[edgeNum(s.circsnum[i].x(),s.circsnum[i].z())].push_back(
                s.tris[i].x()
            );
        }
    }
public:
    static map<double, vector<int>> ShowCNT(Struct3d& s,print3d& result){
        map<double, vector<int>> tmp;
        //加入大三角形
        ShowTriDirt(s,result);
        //加入最简单的多边形的接口
        ShowCirFace(s,result,tmp);
        //加入边线
        map<edgeNum,vector<int>> ms;
        ShowEdge(ms,s);
        for(auto m:ms){
            result.f.push_back(Vector3i(m.second[3],m.second[0],m.second[2]));
            result.f.push_back(Vector3i(m.second[2],m.second[0],m.second[1]));
        }
        return tmp;
    }

};