#include "Delaunay/TriangleMesh.h"
#include<algorithm>
#include<map>

TriangleMesh Delaunay(vector<Point2D>& d){
    //绘制超级三角形
    double SuperTrileft=inf,SuperTriright=-inf,SuperTriup=-inf,SuperTridown=inf;
    auto pointnum=d.size();
    for(int i=0;i<pointnum;i++){
        SuperTrileft=min(SuperTrileft,d[i].x());
        SuperTriright=max(SuperTriright,d[i].x());
        SuperTriup=max(SuperTriup,d[i].y());
        SuperTridown=min(SuperTridown,d[i].y());
    }
    SuperTrileft-=10;
    SuperTriright+=10;
    SuperTridown-=10;
    SuperTriup+=10;
    auto high=SuperTriup-SuperTridown;
    auto width=SuperTriright-SuperTrileft;
    Point2D superp1;
    superp1<<(SuperTrileft+SuperTriright)/2,SuperTriup+high;
    Point2D superp2;
    superp2<<SuperTrileft-width,SuperTridown;
    Point2D superp3;
    superp3<<SuperTrileft+width,SuperTridown;
    //把这几个点拉到点阵里面
    d.push_back(superp1);
    d.push_back(superp2);
    d.push_back(superp3);
    //加入超级三角形
    vector<Vector3i> tri;
    tri.push_back(Vector3i(pointnum,pointnum+1,pointnum+2));
    Triangle2D tmp;
    Vector2i tmpedge;
    Vector3i tmptri;
    for(int i=0;i<pointnum;i++){
        map<Vector2i,bool> dels;//记录下被删去的图形的边
        for(int j=0;j<tri.size();j++){
            tmp.a=d[tri[j].x()];
            tmp.b<<d[tri[j].y()];
            tmp.c<<d[tri[j].z()];
            //出现重复边就删去
            if(pointInCircumcircle(tmp,d[i])){
                tmpedge<<(max(tri[j].x(),tri[j].y()),min(tri[j].x(),tri[j].y()));//这里是为了统一边的序号
                if(dels.count(tmpedge)==0)
                    dels[tmpedge]=true;
                else
                    dels[tmpedge]=false;
                tmpedge<<(max(tri[j].z(),tri[j].y()),min(tri[j].z(),tri[j].y()));
                if(dels.count(tmpedge)==0)
                    dels[tmpedge]=true;
                else
                    dels[tmpedge]=false;
                tmpedge<<(max(tri[j].z(),tri[j].x()),min(tri[j].z(),tri[j].x()));
                if(dels.count(tmpedge)==0)
                    dels[tmpedge]=true;
                else
                    dels[tmpedge]=false;
                tri.erase(tri.begin()+j);
                j--;
            }
        }
        for(auto j:dels){
            if(j.second){
                tmptri<<d[i],d[j.first.x()],d[j.first.y()];
                tri.push_back(tmptri);
            }
        }
    }

    //删除超级三角形
    d.pop_back();
    d.pop_back();
    d.pop_back();
    for(int i=0;i<tri.size();i++){
        bool if_move=(tri[i].x()>=pointnum)&&(tri[i].y()>=pointnum)&&(tri[i].z()>=pointnum);
        if(if_move){

            tri.erase(begin(tri)+i);`
            i--;
        }
    }
    TriangleMesh result;
    result.tri=tri;
    result.d=d;
    return result;
}


Circ getCirc(){

}