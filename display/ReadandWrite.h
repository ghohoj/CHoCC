//写入一个点的字符串
#include<algorithm>
#include <numeric>
#include<string>
#include<fstream>
#include <iomanip>
#include"../geo/geodisplay.h"
#include"../display/getindata.h"

void pointstring(const Point3D& x,fstream& f){
    f<<"v "<<x.x()<<" "<<x.y()<<" "<<x.y()<<" "<<x.z()<<setprecision(5)<<endl;
}
//写入一个面的字符串
//ToDo这里有个天坑就是怎么做好这个面的方向，后面要检查这个事情
void pointstring(const Vector3i& x,fstream& f){
    f<<"f "<<x.x()<<" "<<x.y()<<" "<<x.z()<<endl;
}

//最后的写入文件
void printobj(const print3d& result,string u){
    fstream f;
	f.open(u+".obj",ios::out);
    //打印点
    for(auto p:result.p){
        pointstring(p,f);
    }
    //连线部分
    for(auto l:result.f){
        pointstring(l,f);
    }
    f.close();
}

Interface readTxt(string file)
{
    ifstream infile; 
    infile.open(file.data());   //将文件流对象与文件连接起来 
    assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
    string s;
    Interface tmp;
    int tmpnum;//柱子的方向有几个
    double x,y,z;
    infile>>tmpnum;
    infile>>tmp.r;
    for(int i=0;i<tmpnum;i++){
        infile>>x>>y>>z;
        tmp.dir.push_back(Vector3d(x,y,z).normalized());
    }
    infile.close();             //关闭文件输入流 
    return tmp;
}
