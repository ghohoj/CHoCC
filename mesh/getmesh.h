//这里我们尝试生存一个简单的网格。我们只生成一个正方形的网格，即
//n*n*n的网格
#include"../display/getindata.h"

const int n=6;
const double r=10;
const double LenBetweenCirc=100;


//生成网格
void getmesh(){
    Interface result[n][n][n];
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            for(int k=0;k<n;k++){
                result[i][j][k].r=r;
                result[i][j][k].position=Vector3d(i*LenBetweenCirc,j*LenBetweenCirc,k*LenBetweenCirc);
                getR(result[i][j][k]);
                //下面是读取文件的dir
                if(i!=0){
                    result[i][j][k].dir.push_back(Vector3d(-1,0,0));
                }
                if(i!=n-1){
                    result[i][j][k].dir.push_back(Vector3d(1,0,0));
                }
                if(j!=0){
                    result[i][j][k].dir.push_back(Vector3d(0,-1,0));
                }
                if(j!=n-1){
                    result[i][j][k].dir.push_back(Vector3d(0,1,0));
                }
                if(k!=0){
                    result[i][j][k].dir.push_back(Vector3d(0,0,-1));
                }
                if(k!=n-1){
                    result[i][j][k].dir.push_back(Vector3d(0,0,1));
                }
            }
        }
    }
}