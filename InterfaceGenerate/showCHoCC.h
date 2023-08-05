# pragma once
#include<iostream>
#include<vector>
#include<math.h>
#include<map>
#include"../geo/geocal.h"
#include"../display/Struct3d.h"
using namespace std;


class CHoCC{

    //输出a到b之间的整数，注意如果a<b，会循环，例如再0到18循环，a=17，b=3，变成17,18,0,1,2,3
    static vector<int> getp(int a,int b){
        vector<int> result;
        if(a<b){
            for(int i=a;i<=b;i++){
                result.push_back(i);
            }
        }
        else if(a==b){
            result.push_back(a);
        }
        else{
            for(int i=a;i<=((a/(acc+1)+1)*(acc+1)-2);i++){
                result.push_back(i);
            }
            for(int i=(a/(acc+1))*(acc+1);i<=b;i++){
                result.push_back(i);
            }
        }
        return result;
    }

    static bool compare(int a1,int a2,int b1,int b2){
        a1++;
        b1++;
        if(a1*b2<a2*b1){
            return true;
        }
        else{
            return false;
        }
    }

    //这里的采取的方法是，直接连接不同弧线之间的点，如果弧线a与弧线b的点数目不一样，那么我们取点少的那组，然后按照比例连接另外一组
    //例如弧线a有1，2，3，4，弧线b有4，5，我们连接a1b4,a2b4,a2b5,a3b5,a4b5,当ai/a.size()较小就下个点变换a，反之变换b5
    //实现这个的函数是getedgeofcone
    static void getedgeofcone(const vector<int>& getp1,const vector<int>& getp2,vector<Vector3i>& result){
        int s1=getp1.size();
        int s2=getp2.size();
        int i=0;
        int j=getp2.size()-1;
        while (i<s1-1&&j>0)
        {
            if(compare(i,s1,s2-j-1,s2)){
                i++;
                result.push_back(Vector3i(getp1[i],getp1[i-1],getp2[j]));
            }
            else{
                j--;
                result.push_back(Vector3i(getp1[i],getp2[j],getp2[j+1]));
            }
        }
        if(j==0){
            while (i<s1-1)
            {
                i++;
                result.push_back(Vector3i(getp1[i],getp1[i-1],getp2[j]));
            }
        }
        else{
            while (j>0)
            {
                j--;
                result.push_back(Vector3i(getp1[i],getp2[j],getp2[j+1]));
            }
        }
    }

    //画圆形
    static void ShowCirc(Circ &cir,print3d& result){
        int psize=result.p.size();
        cir.getvertica();
        //加点
        for(int i=0;i<acc;i++){
            result.p.push_back(cir.center+cir.vertica[0]*cir.r*sin(2*M_PI/acc*i)+cir.vertica[1]*cir.r*cos(2*M_PI/acc*i));
        }
        result.p.push_back(cir.center);
        //加面
        for(int j=0;j<acc;j++){
            result.f.push_back(Vector3i(psize+(j)%acc,psize+(j+1)%acc,psize+acc));
        }
    }
    //画三角形
    static void ShowTriUseCircPoint(Struct3d& s,print3d& result,int i){
        //这三个点的圆位置序号  s.circsnum[i].x();
        //三个点坐标在ps中的编号 s.tris[i].x()
        //这三个点的坐标  s.ps[s.tris[i].x()];
        //这三个点映射到的圆的第几个点（这里采取就近选择）  (s.circs[s.circsnum[i].x()]).geti(s.ps[s.tris[i].x()],acc)
        // 在加上圆本身的坐标偏移  (s.circs[s.circsnum[i].x()]).geti(s.ps[s.tris[i].x()],acc)+s.circs[s.circsnum[i].x()]*(acc+1)
        auto tmpVector3i=Vector3i(
            (s.circs[s.circsnum[i].x()]).geti(s.ps[s.tris[i].x()])+s.circsnum[i].x()*(acc+1),
            (s.circs[s.circsnum[i].y()]).geti(s.ps[s.tris[i].y()])+s.circsnum[i].y()*(acc+1),
            (s.circs[s.circsnum[i].z()]).geti(s.ps[s.tris[i].z()])+s.circsnum[i].z()*(acc+1)
        );
        result.f.push_back(tmpVector3i);
    }

    static void getConeedge(map<edgeNum,vector<int>>& ms,Struct3d& s){
        for(int i=0;i<s.circsnum.size();i++){
            ms[edgeNum(s.circsnum[i].x(),s.circsnum[i].y())].push_back(
                (s.circs[s.circsnum[i].x()]).geti(s.ps[s.tris[i].x()])+s.circsnum[i].x()*(acc+1)
            );
            ms[edgeNum(s.circsnum[i].x(),s.circsnum[i].y())].push_back(
                (s.circs[s.circsnum[i].y()]).geti(s.ps[s.tris[i].y()])+s.circsnum[i].y()*(acc+1)
            );
            ms[edgeNum(s.circsnum[i].y(),s.circsnum[i].z())].push_back(
                (s.circs[s.circsnum[i].y()]).geti(s.ps[s.tris[i].y()])+s.circsnum[i].y()*(acc+1)
            );
            ms[edgeNum(s.circsnum[i].y(),s.circsnum[i].z())].push_back(
                (s.circs[s.circsnum[i].z()]).geti(s.ps[s.tris[i].z()])+s.circsnum[i].z()*(acc+1)
            );
            ms[edgeNum(s.circsnum[i].x(),s.circsnum[i].z())].push_back(
                (s.circs[s.circsnum[i].z()]).geti(s.ps[s.tris[i].z()])+s.circsnum[i].z()*(acc+1)
            );
            ms[edgeNum(s.circsnum[i].x(),s.circsnum[i].z())].push_back(
                (s.circs[s.circsnum[i].x()]).geti(s.ps[s.tris[i].x()])+s.circsnum[i].x()*(acc+1)
            );
        }
    }
public:
    //这段的目的是完整的展示一个接口的形状，并不适合作为每个接口的计算方程，那样复杂度太高
    static void ShowCHoCC(Struct3d& s,print3d& result){
        //加入圆柱的接口
        for(auto cir:s.circs){
            ShowCirc(cir,result);
        }
        //加入大三角形
        for(int i=0;i<s.circsnum.size();i++){
            ShowTriUseCircPoint(s,result,i);
        }
        // 找到每个椭圆锥的四个点
        map<edgeNum,vector<int>> ms;
        getConeedge(ms,s);
        //加入椭圆锥面
        for(auto m:ms){
            vector<int> ep1=getp(m.second[0],m.second[3]);
            vector<int> ep2=getp(m.second[2],m.second[1]);
            getedgeofcone(ep1,ep2,result.f);
        }
    }

};



