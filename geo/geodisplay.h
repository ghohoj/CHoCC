//把struct转化为print3d格式的数据，这样可以比较好的打印
#include<iostream>
#include<vector>
#include<math.h>
#include<map>
#include"../geo/geocal.h"
#include"../display/Struct3d.h"
using namespace std;
const int acc=19;//圆一圈采样acc个点

struct print3d
{
    vector<Point3D> p;
    vector<Vector3i> f;
};
struct coneedgepoint
{
    //点所在圆的编号（这里两个数字，顺序一致）
    int pnums[2];
    //点所在圆边上的编号
    int cnum;
};
struct geodisplay
{
    coneedgepoint p[2];
};

//输出a到b之间的整数，注意如果a<b，会循环，例如再0到18循环，a=17，b=3，变成17,18,0,1,2,3
vector<int> getp(int a,int b){
    vector<int> result;
    if(a<b){
        for(int i=a;i<=b;i++){
            result.push_back(i);
        }
    }
    else if(a==b){
        cout<<"出现错误,a不可能等于b";
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

bool compare(int a1,int a2,int b1,int b2){
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
void getedgeofcone(const vector<int>& getp1,const vector<int>& getp2,vector<Vector3i>& result){
    int s1=getp1.size();
    int s2=getp2.size();
    int i=0;
    int j=getp2.size()-1;
    while (i<s1&&j<s2)
    {
        if(compare(i,s1,s2-j-1,s2)){
            i++;
            result.push_back(Vector3i(getp1[i],getp1[i-1],getp2[j]));
        }
        else
        {
            j--;
            result.push_back(Vector3i(getp1[i],getp2[j],getp2[j+1]));
        }
    }
}

//这段的目的是完整的展示一个接口的形状，并不适合作为每个接口的计算方程，那样复杂度太高
void ShowGeo(Struct3d& s,print3d& result){
    for(auto cir:s.circs){
        //这里是计算与圆的法向量垂直的两个向量
        cir.getvertica();
        //写入圆上的取点19个,再写入中心放在第20个点
        for(int i=0;i<acc;i++){
            result.p.push_back(cir.center+cir.vertica[0]*sin(2*M_PI/acc*i)+cir.vertica[1]*cos(2*M_PI/acc*i));
            result.p.push_back(cir.center);
        }
    }
    //加入圆形的面
    for(int i=0;i<s.circs.size();i++){
        //边界采样19个点则生成18个三角形
        for(int j=0;j<acc-1;j++){
            result.f.push_back(Vector3i(i*(acc+1)+j,i*(acc+1)+j+1,(i+1)*(acc+1)));
        }
    }
    //加入大三角形
    for(int i=0;i<s.circsnum.size();i++){
        //这三个点的圆位置序号  s.circsnum[i].x();
        //三个点坐标在ps中的编号 s.tris[i].x()
        //这三个点的坐标  s.ps[s.tris[i].x()];
        //这三个点映射到的圆的第几个点（这里采取就近选择）  (s.circs[s.circsnum[i].x()]).geti(s.ps[s.tris[i].x()],acc)
        // 在加上圆本身的坐标偏移  (s.circs[s.circsnum[i].x()]).geti(s.ps[s.tris[i].x()],acc)+s.circs[s.circsnum[i].x()]*(acc+1)
        auto tmpVector3i=Vector3i(
            (s.circs[s.circsnum[i].x()]).geti(s.ps[s.tris[i].x()],acc)+s.circsnum[i].x()*(acc+1),
            (s.circs[s.circsnum[i].y()]).geti(s.ps[s.tris[i].y()],acc)+s.circsnum[i].y()*(acc+1),
            (s.circs[s.circsnum[i].z()]).geti(s.ps[s.tris[i].z()],acc)+s.circsnum[i].z()*(acc+1)
        );

        // 前面加入circsnum的时候已经反转过方向
        // 这里是调整一下面的方向问题，使得法向量朝外面
        // vector<Point3D> tmpvectorPoint3D;
        // tmpvectorPoint3D.push_back(s.ps[tmpVector3i.x()]);
        // tmpvectorPoint3D.push_back(s.ps[tmpVector3i.y()]);
        // tmpvectorPoint3D.push_back(s.ps[tmpVector3i.z()]);
        // if(!IfPlanefromPointRight(tmpvectorPoint3D,s.center)){
        //     tmpVector3i<<tmpVector3i.x(),tmpVector3i.z(),tmpVector3i.y();
        // }
        result.f.push_back(tmpVector3i);
    }
    //找到每个椭圆锥的四个点
    map<edgeNum,vector<int>> ms;
    for(int i=0;i<s.circsnum.size();i++){
        ms[edgeNum(s.circsnum[i].x(),s.circsnum[i].y())].push_back(
            (s.circs[s.circsnum[i].x()]).geti(s.ps[s.tris[i].x()],acc)+s.circsnum[i].x()*(acc+1)
        );
        ms[edgeNum(s.circsnum[i].x(),s.circsnum[i].y())].push_back(
            (s.circs[s.circsnum[i].y()]).geti(s.ps[s.tris[i].y()],acc)+s.circsnum[i].y()*(acc+1)
        );
        ms[edgeNum(s.circsnum[i].y(),s.circsnum[i].z())].push_back(
            (s.circs[s.circsnum[i].y()]).geti(s.ps[s.tris[i].y()],acc)+s.circsnum[i].y()*(acc+1)
        );
        ms[edgeNum(s.circsnum[i].y(),s.circsnum[i].z())].push_back(
            (s.circs[s.circsnum[i].z()]).geti(s.ps[s.tris[i].z()],acc)+s.circsnum[i].z()*(acc+1)
        );
        ms[edgeNum(s.circsnum[i].x(),s.circsnum[i].z())].push_back(
            (s.circs[s.circsnum[i].z()]).geti(s.ps[s.tris[i].z()],acc)+s.circsnum[i].z()*(acc+1)
        );
        ms[edgeNum(s.circsnum[i].x(),s.circsnum[i].z())].push_back(
            (s.circs[s.circsnum[i].x()]).geti(s.ps[s.tris[i].x()],acc)+s.circsnum[i].x()*(acc+1)
        );
    }
    //加入椭圆锥面，
    //这样整个图形就都被切割为了三角形，将椭圆锥切割为三角形
    for(auto m:ms){
        vector<int> ep1=getp(m.second[0],m.second[2]);
        vector<int> ep2=getp(m.second[1],m.second[3]);
        getedgeofcone(ep1,ep2,result.f);
    }
}


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

void ShowCNT(Struct3d& s,print3d& result){

}

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

void ShowACHoCC(Struct3d& s,print3d& result){

}


//