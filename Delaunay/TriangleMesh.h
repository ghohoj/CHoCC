// # pragma once
// #include<iostream>
// #include<vector>
// #include"../geo/geocal.h"
// using namespace std;
// struct TriangleMesh
// {
//     vector<Point2D> d;
//     vector<Vector3i> tri;
// };

// ostream & operator<<(ostream & out,const TriangleMesh& spec){
//     out<<"类型：网格"<<endl;
//     out<<"下面是点的坐标"<<endl;
//     for(auto i:spec.d){
//         out<<i.x()<<" "<<i.y()<<endl;
//     }
//     out<<"下面是点连接成的三角形"<<endl;
//     for(auto i:spec.tri){
//         out<<i.x()<<" "<<i.y()<<" "<<i.z()<<endl;
//     }
//     return out;
// }