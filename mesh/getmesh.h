//这里我们尝试生存一个简单的网格。我们只生成一个正方形的网格，即
//n*n*n的网格
#pragma once
#include"../display/getindata.h"
#include"../geo/geodisplay.h"
#include"../mesh/AddObj.h"
#include"../display/ReadandWrite.h"

const int n=3;
const double r=10;
const double LenBetweenCirc=100;


class Mesh{
    //生成网格
public:
    Interface meshinput[n][n][n];
    void getmesh(){
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                for(int k=0;k<n;k++){
                    meshinput[i][j][k].r=r;
                    meshinput[i][j][k].position=Vector3d(i*LenBetweenCirc,j*LenBetweenCirc,k*LenBetweenCirc);
                    //下面是读取文件的dir
                    if(i!=0){
                        meshinput[i][j][k].dir.push_back(Vector3d(-1,0,0));
                    }
                    if(i!=n-1){
                        meshinput[i][j][k].dir.push_back(Vector3d(1,0,0));
                    }
                    if(j!=0){
                        meshinput[i][j][k].dir.push_back(Vector3d(0,-1,0));
                    }
                    if(j!=n-1){
                        meshinput[i][j][k].dir.push_back(Vector3d(0,1,0));
                    }
                    if(k!=0){
                        meshinput[i][j][k].dir.push_back(Vector3d(0,0,-1));
                    }
                    if(k!=n-1){
                        meshinput[i][j][k].dir.push_back(Vector3d(0,0,1));
                    }
                    getR(meshinput[i][j][k]);
                }
            }
        }
    }

    void getmeshresult(print3d& result){
        //加点
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                for(int k=0;k<n;k++){
                    Struct3d z(meshinput[i][j][k]);
                    print3d tmp;
                    CHoCC::ShowCHoCC(z,tmp);
                    addObj(result,tmp);
                }
            }
        }
        //加面
        for(int i=0;i<n;i++){
            for(int j=0;j<n;j++){
                for(int k=0;k<n;k++){
                    
                }
            }
        }
    }

};

