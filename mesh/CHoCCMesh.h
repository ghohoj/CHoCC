//这里我们尝试生存一个简单的网格。我们只生成一个正方形的网格，即
//n*n*n的网格
#pragma once
#include"../display/getindata.h"
#include"../geo/geodisplay.h"
#include"../mesh/AddObj.h"
#include"../display/ReadandWrite.h"
#include"../display/Struct3d.h"
#include"../generate/PlaneRollingBoundary.h"



class CHoCCMesh{
    //生成网格
public:
    Struct3d meshoutput[MESHN][MESHN][MESHN];
    map<double, vector<pair<int, int>>> mesh[MESHN][MESHN][MESHN];
    int startnum[MESHN][MESHN][MESHN];
    void getmesh(){
        Interface meshinput[MESHN][MESHN][MESHN];
        for(int i=0;i<MESHN;i++){
            for(int j=0;j<MESHN;j++){
                for(int k=0;k<MESHN;k++){
                    meshinput[i][j][k].r=MESHR;
                    meshinput[i][j][k].position=Vector3d(i*LenBetweenCirc,j*LenBetweenCirc,k*LenBetweenCirc);
                    //下面是读取文件的dir
                    if(i!=0){
                        meshinput[i][j][k].dir.push_back(Vector3d(-1,0,0));
                    }
                    if(i!=MESHN-1){
                        meshinput[i][j][k].dir.push_back(Vector3d(1,0,0));
                    }
                    if(j!=0){
                        meshinput[i][j][k].dir.push_back(Vector3d(0,-1,0));
                    }
                    if(j!=MESHN-1){
                        meshinput[i][j][k].dir.push_back(Vector3d(0,1,0));
                    }
                    if(k!=0){
                        meshinput[i][j][k].dir.push_back(Vector3d(0,0,-1));
                    }
                    if(k!=MESHN-1){
                        meshinput[i][j][k].dir.push_back(Vector3d(0,0,1));
                    }
                    getR(meshinput[i][j][k]);
                    meshoutput[i][j][k].fresh(meshinput[i][j][k]);
                }
            }
        }
    }

    void getmeshresult(print3d& result){
        //加点
        for(int i=0;i<MESHN;i++){
            for(int j=0;j<MESHN;j++){
                for(int k=0;k<MESHN;k++){
                    print3d tmp;
                    mesh[i][j][k]=CHoCC::ShowCHoCC(meshoutput[i][j][k],tmp);
                    startnum[i][j][k]=result.p.size();
                    addObj(result,tmp);
                }
            }
        }
        //加面
        Vector3d tmpVec(1,0,0);
        for(int i=0;i<MESHN-1;i++){
            for(int j=0;j<MESHN;j++){
                for(int k=0;k<MESHN;k++){
                    //一个顺时针一个反过来，连接即可
                    connect(mesh[i][j][k][Veckey(tmpVec)][0],
                    mesh[i+1][j][k][Veckey(-tmpVec)][0],
                    result,tmpVec,startnum[i][j][k],startnum[i+1][j][k]);
                }
            }
        }
        tmpVec<<0,1,0;
        for(int i=0;i<MESHN;i++){
            for(int j=0;j<MESHN-1;j++){
                for(int k=0;k<MESHN;k++){
                    connect(mesh[i][j][k][Veckey(tmpVec)][0],
                    mesh[i][j+1][k][Veckey(-tmpVec)][0],
                    result,tmpVec,startnum[i][j][k],startnum[i][j+1][k]);
                }
            }
        }
        tmpVec<<0,0,1;
        for(int i=0;i<MESHN;i++){
            for(int j=0;j<MESHN;j++){
                for(int k=0;k<MESHN-1;k++){
                    connect(mesh[i][j][k][Veckey(tmpVec)][0],
                    mesh[i][j][k+1][Veckey(-tmpVec)][0],
                    result,tmpVec,startnum[i][j][k],startnum[i][j][k+1]);
                }
            }
        }
    }

    void connect(pair<int,int> x,pair<int,int> y,print3d& result,Vector3d tmpVec,int m1,int m2){
        Matrix<double,1,3> equation(tmpVec);
        FullPivLU<Matrix<double,1,3>> solve(equation);
        auto vertica0=(Matrix<double,3,2>(solve.kernel()).col(0)).normalized();
        auto vertica1=tmpVec.cross(vertica0).normalized();
        double angle,minangle=100;
        int l1,l2;
        x.first=x.first+m1;
        x.second=x.second+m1;
        y.first=y.first+m2;
        y.second=y.second+m2;
        Vector3d s;
        for(int i=x.first;i<x.second;i++){
            s=result.p[i]-result.p[x.second];
            angle=atan2(s.dot(vertica1),s.dot(vertica0));
            if(angle<minangle){
                minangle=angle;
                l1=i-x.first;
            }
        }
        minangle=100;
        for(int i=y.first;i<y.second;i++){
            s=result.p[i]-result.p[y.second];
            angle=atan2(s.dot(vertica1),s.dot(vertica0));
            if(angle<minangle){
                minangle=angle;
                l2=i-y.first;
            }
        }

        int s1=x.second-x.first;
        int s2=y.second-y.first;
        int i=0;
        int j=0;
        while (i<s1&&j>-s2)
        {
            if(compare(i,s1,-j,s2)){
                i++;
                result.f.push_back(Vector3i(x.first+getmod(i+l1,s1),x.first+getmod(i+l1-1,s1),y.first+getmod(l2+j,s2)));
            }
            else{
                j--;
                result.f.push_back(Vector3i(x.first+getmod(i+l1,s1),y.first+getmod(l2+j,s2),y.first+getmod(l2+(j+1),s2)));
            }
        }
        if(j==-s2){
            while (i<s1)
            {
                i++;
                result.f.push_back(Vector3i(x.first+getmod(i+l1,s1),x.first+getmod(i+l1-1,s1),y.first+getmod(l2+j,s2)));
            }
        }
        else{
            while (j>-s2)
            {
                j--;
                result.f.push_back(Vector3i(x.first+getmod(i+l1,s1),y.first+getmod(l2+j,s2),y.first+getmod(l2+(j+1),s2)));
            }
        }
    }
};

