# pragma once
#include<iostream>
#include<vector>
#include<math.h>
#include<map>
#include<set>
#include"../geo/geocal.h"
#include"../display/Struct3d.h"
using namespace std;

// 这里是CNT之后再划分一次点
// 原文
// The first step is to refine the tessellation. To do so, we first
// insert a vertex at the midpoint of each edge of the CNT of CHoCC
// of each chopped-ball. Then, for each triangle of that CNT, we
// refine it into four new triangles, where one of the new triangles
// is incident on the three inserted vertices. Similarly, for each quad
// (approximating a corridor), we also insert one new vertex at its
// centroid and refine the quad into eight triangles (as shown in
// Fig. 18—left).
// This refinement step does not change the shape of the mesh,
// and hence does not improve its approximation accuracy. To im-
// prove accuracy, a vertex that was inserted on an edge of a
// polygonal approximation of a circle is snapped radially onto its
// closest projection on that circle (see Fig. 18—right). Note that after
// this process, some flat edges may become concave.
// Next, we adjust the positions of the refined and snapped ver-
// tices of the CNT associated with the ball. To do this, our central-
// projection step moves each one of these vertices towards or away
// from the center of the ball, so as to bring it to the boundary of
// the exact lattice. A typical result is shown in Fig. 19—left.

class ACHoCC{
    //FilletSurface中计算的d(P)
    static double FilletSurface(const vector<double>& nums,double b=-1){
        if(b<0){
            double numssum=nums[0];
            for(int i=1;i<nums.size();i++){
                numssum=min(numssum,nums[i]);
            }
            return numssum;
        }else{
            double numssum=0;
            for(auto num:nums){
                numssum+=exp(-b*num);
            }
            return -log(numssum)/b;
        }

    }

    //cone-sphere distance
    static double distance(const Point3D& p,const Sphere& s,const double& r,const Vector3d& dir){
        double x=dir.normalized().dot(p-s.center);
        double rp2cenline=sqrt((p-s.center).squaredNorm()-x*x);
        if(x>0){
            return rp2cenline-r;
        }
        else{
            return (s.center-p).norm()-r;
        }
    }

    //对一个点进行移动
    static void movetolattice(const Struct3d& s,Point3D& p,bool IfAddPoint=false,double binpaper=-1){
        double a=1;
        double b=0;
        double tmp=(a+b)/2;
        vector<double> nums;
        int cirsize=s.circs.size();
        if(IfAddPoint){
            cirsize--;
        }
        while(abs(a-b)>0.02){
            tmp=(a+b)/2;
            nums.clear();
            auto tmppoint=(1-tmp)*s.sphere.center+tmp*p;
            for(int i=0;i<cirsize;i++){
                nums.push_back(distance(tmppoint,s.sphere,s.circs[i].r,s.circs[i].dir));
            }
            double distmp=FilletSurface(nums,binpaper);
            if(distmp>0){
                a=tmp;
            }
            else{
                b=tmp;
            }
        }
        tmp=(a+b)/2;
        p=(1-tmp)*s.sphere.center+tmp*p;
    }

    //记录下coneedge的四个特征点
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

    //对angle排序，x也按照angle的排序而排序
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

    //展示圆面的obj文件
    static void ShowCirFace(Struct3d& s,print3d& result,map<edgeNum,int>& getmid,map<int,int>& circenter,int& pnum,
    set<int,greater<int>>& need_adjust,bool IfAddPoint=false)
    {
        //将这些点归类到各个圆形上面
        vector<vector<int>> x(s.circs.size());
        for(int i=0;i<s.circsnum.size();i++){
            x[s.circsnum[i].x()].push_back(s.tris[i].x());
            x[s.circsnum[i].y()].push_back(s.tris[i].y());
            x[s.circsnum[i].z()].push_back(s.tris[i].z());
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
            if(i==s.circs.size()-1&&IfAddPoint){
                need_adjust.insert(pnum);
            }
            circenter[i]=pnum++;
            
            for(int j=0;j<angles.size();j++){
                int u0=x[i][getmod(j-1,angles.size())];
                int u1=x[i][j];
                int u2=x[i][(j+1)%angles.size()];
                int u3=x[i][(j+2)%angles.size()];
                getmid[edgeNum(u1,u2)]=pnum++;
                //这里有个我排查了很久的bug，当椭圆锥细化到一定程度退化为线段，
                //两个点（例如这两个点是1，2）退化为一个点这就导致了getmid中可能会出现一些点索引是1，3找的到，但换成2，3就找不到了，
                //这里遇到这种先把getmid里面的数值统一一下
                bool repeatposition1=APequal(angles[j],angles[getmod(j-1,angles.size())]);
                bool repeatposition2=APequal(angles[(j+1)%angles.size()],angles[(j+2)%angles.size()]);
                if(repeatposition1){
                    getmid[edgeNum(u0,u2)]=pnum-1;
                }
                if(repeatposition2){
                    getmid[edgeNum(u1,u3)]=pnum-1;
                }
                if(repeatposition1&&repeatposition2){
                    getmid[edgeNum(u0,u3)]=pnum-1;
                }
                //加入圆上多边形的中点
                result.p.push_back((result.p[u1]+result.p[u2])/2);
                int u=getmid[edgeNum(u1,u2)];
                int uc=circenter[i];

                result.f.push_back(Vector3i(uc,u1,u));
                result.f.push_back(Vector3i(uc,u,u2));
            }
        }
    }

    //输入三角形的点
    static void getinitpoint(Struct3d& s,print3d& result,set<int,greater<int>>& need_adjust,bool IfAddPoint=false){
        for(int i=0;i<s.ps.size();i++){
            result.p.push_back(s.ps[i]);
        }
        if(IfAddPoint){
            need_adjust.insert(s.ps.size()-3);
            need_adjust.insert(s.ps.size()-2);
            need_adjust.insert(s.ps.size()-1);
        }
    }
    
    //加入三角形的中间点
    static void AddTriEdgeCenter(Struct3d& s,print3d& result,map<edgeNum,int>& getmid,int& pnum){
        for(int i=0;i<s.tris.size();i++){
            result.p.push_back(
                (s.ps[s.tris[i].x()]+s.ps[s.tris[i].y()])/2
                );
            getmid[edgeNum(s.tris[i].x(),s.tris[i].y())]=pnum++;
            result.p.push_back(
                (s.ps[s.tris[i].y()]+s.ps[s.tris[i].z()])/2
                );
            getmid[edgeNum(s.tris[i].y(),s.tris[i].z())]=pnum++;
            result.p.push_back(
                (s.ps[s.tris[i].z()]+s.ps[s.tris[i].x()])/2
                );
            getmid[edgeNum(s.tris[i].z(),s.tris[i].x())]=pnum++;
        }
    }
    
    //加入coneedge的中心点
    static void Addconeedgecenter(Struct3d& s,print3d& result,map<edgeNum,vector<int>>& ms,map<FaceNum,int>& facenum,int& pnum){
        for(auto m:ms){
            result.p.push_back(
                (s.ps[m.second[0]]+s.ps[m.second[1]]+s.ps[m.second[2]]+s.ps[m.second[3]])/4
            );
            facenum[m.second]=pnum++;
        }
    }
    
    //三角形的obj输入
    static void ShowTri(Struct3d& s,print3d& result,map<edgeNum,int>& getmid){
        for(int i=0;i<s.tris.size();i++){
            assert(getmid.count(edgeNum(s.tris[i].x(),s.tris[i].y()))!=0);
            assert(getmid.count(edgeNum(s.tris[i].z(),s.tris[i].x()))!=0);
            assert(getmid.count(edgeNum(s.tris[i].y(),s.tris[i].z()))!=0);
            int xy=getmid[edgeNum(s.tris[i].x(),s.tris[i].y())];
            int xz=getmid[edgeNum(s.tris[i].z(),s.tris[i].x())];
            int yz=getmid[edgeNum(s.tris[i].y(),s.tris[i].z())];
            int x=s.tris[i].x();
            int y=s.tris[i].y();
            int z=s.tris[i].z();
            result.f.push_back(Vector3i(x,xy,xz));
            result.f.push_back(Vector3i(y,yz,xy));
            result.f.push_back(Vector3i(z,xz,yz));
            result.f.push_back(Vector3i(yz,xz,xy));
        }
    }

    //展示coneedge的obj文件
    static void ShowConeedge(Struct3d& s,print3d& result,map<FaceNum,int>& facenum,map<edgeNum,int>& getmid
        ,map<edgeNum,vector<int>> ms){
        for(auto m:ms){
            int center=facenum[m.second];
            int mid01=getmid[edgeNum(m.second[0],m.second[1])];
            int mid12=getmid[edgeNum(m.second[1],m.second[2])];
            int mid23=getmid[edgeNum(m.second[2],m.second[3])];
            int mid30=getmid[edgeNum(m.second[3],m.second[0])];

            assert(getmid.count(edgeNum(m.second[0],m.second[1]))!=0);
            assert(getmid.count(edgeNum(m.second[1],m.second[2]))!=0);
            assert(getmid.count(edgeNum(m.second[2],m.second[3]))!=0);
            assert(getmid.count(edgeNum(m.second[3],m.second[0]))!=0);

            result.f.push_back(Vector3i(m.second[0],mid01,center));
            result.f.push_back(Vector3i(center,mid01,m.second[1]));

            result.f.push_back(Vector3i(m.second[1],mid12,center));
            result.f.push_back(Vector3i(center,mid12,m.second[2]));

            result.f.push_back(Vector3i(m.second[2],mid23,center));
            result.f.push_back(Vector3i(center,mid23,m.second[3]));

            result.f.push_back(Vector3i(m.second[3],mid30,center));
            result.f.push_back(Vector3i(center,mid30,m.second[0]));
            
        }
    }

    //CNT+插入中间点
    static void WritePoint(Struct3d& s,print3d& result,set<int,greater<int>>& need_adjust,bool IfAddPoint=false){
        map<edgeNum,vector<int>> ms;//标记椭圆锥上的四个特征点
        ShowEdge(ms,s);
        map<edgeNum,int> getmid;//标记边的中点
        map<int,int> circenter;//标记圆中多边形的中点
        map<FaceNum,int> facenum;//圆形中点

        //加入CNT中已经有的点
        getinitpoint(s,result,need_adjust,IfAddPoint);
        int pnum=result.p.size();
        //加入三角形中点
        AddTriEdgeCenter(s,result,getmid,pnum);
        //加入三角形面
        ShowTri(s,result,getmid);
        // //加入椭圆锥的中心点并记录到facenum
        Addconeedgecenter(s,result,ms,facenum,pnum);
        // //圆面中心点,圆面的连接
        ShowCirFace(s,result,getmid,circenter,pnum,need_adjust,IfAddPoint);
        // //连接椭圆锥
        ShowConeedge(s,result,facenum,getmid,ms);

        for(auto i:getmid){
            need_adjust.insert(i.second);
        }
        for(auto i:facenum){
            need_adjust.insert(i.second);
        }
    }

    //对点的位置进行调整
    static void adjustpoint(Struct3d& s,print3d& result,set<int, greater<int>>& need_adjust,bool IfAddPoint=false,double binpaper=-1){
        for(auto i:need_adjust){
            //投影到圆心
            auto tmpp=result.p[i];
            tmpp=s.sphere.getSurPoint(tmpp);
            //投影到面上面
            movetolattice(s,tmpp,IfAddPoint,binpaper);
            result.p[i]=tmpp;
        }
    }
    
    static int IfAddPoint(Struct3d& s){
        vector<Point3D> p;
        int i;
        for(i=0;i<s.tris.size();i++){
            p.clear();
            p.push_back(s.ps[s.tris[i].x()]);
            p.push_back(s.ps[s.tris[i].y()]);
            p.push_back(s.ps[s.tris[i].z()]);
            if(!IfPlanefromPointRight(p,s.sphere.center)){
                break;
            }
        }
        if(i!=s.tris.size()){
            return i;
        }
        return -1;
    }
    static void pre_deal(Struct3d& s,int num){
        //计算圆形与中心
        Vector3d tmpp;
        tmpp<<(s.ps[s.tris[num].x()]+s.ps[s.tris[num].y()]+s.ps[s.tris[num].z()])/3;
        tmpp=s.sphere.center-(tmpp-s.sphere.center);
        tmpp=s.sphere.getSurPoint(tmpp);
        Circ c(s.sphere.center+s.sphere.r*(tmpp-s.sphere.center).normalized(),(tmpp-s.sphere.center).normalized(),0.00001);
        s.circs.push_back(c);
        s.ps.push_back(
            c.center+((s.ps[s.tris[num].y()]-c.center)+(s.ps[s.tris[num].z()]-c.center)).normalized()*c.r
            );
        s.ps.push_back(
            c.center+((s.ps[s.tris[num].x()]-c.center)+(s.ps[s.tris[num].z()]-c.center)).normalized()*c.r
        );
        s.ps.push_back(
            c.center+((s.ps[s.tris[num].x()]-c.center)+(s.ps[s.tris[num].y()]-c.center)).normalized()*c.r
        );
        //加入三角形
        Vector3i tmp;
        tmp<<s.ps.size()-3,s.tris[num].y(),s.tris[num].z();
        s.tris.push_back(tmp);
        tmp<<s.tris[num].x(),s.ps.size()-2,s.tris[num].z();
        s.tris.push_back(tmp);
        tmp<<s.tris[num].x(),s.tris[num].y(),s.ps.size()-1;
        s.tris.push_back(tmp);
        s.tris.erase(s.tris.begin()+num);
        //加入圆形坐标
        tmp<<s.circs.size()-1,s.circsnum[num].y(),s.circsnum[num].z();
        s.circsnum.push_back(tmp);
        tmp<<s.circsnum[num].x(),s.circs.size()-1,s.circsnum[num].z();
        s.circsnum.push_back(tmp);
        tmp<<s.circsnum[num].x(),s.circsnum[num].y(),s.circs.size()-1;
        s.circsnum.push_back(tmp);
        s.circsnum.erase(s.circsnum.begin()+num);
    }

public:
    static void ShowACHoCC(Struct3d& s,print3d& result,double binpaper=-1){
        //写入点与面
        int choice=IfAddPoint(s);
        set<int, greater<int>> need_adjust;
        if(choice==-1){
            WritePoint(s,result,need_adjust);
            //调整点的位置
            adjustpoint(s,result,need_adjust,false,binpaper=binpaper);
        }
        else{
            pre_deal(s,choice);
            WritePoint(s,result,need_adjust,true);
            //调整点的位置
            adjustpoint(s,result,need_adjust,true,binpaper);
        }
    }
};