//使用球上面的两个圆生成圆锥，这里有比较大的问题，应该三是椭圆锥
// InfCone CIRC2andSPHERE(const Sphere &s,const Circ &x,const Circ &y){
//     //检测这2个圆是不是在球上
//     double d1=(s.center-x.center).norm();
//     double approx_r1=d1*d1+x.r*x.r;
//     double d2=(s.center-y.center).norm();
//     double approx_r2=d2*d2+y.r*y.r;
//     InfCone result;
//     if(!(APequal(approx_r1,s.r)&&APequal(approx_r2,s.r))){
//         cout<<"这个圆不在球上面";
//         cout<<s;
//         cout<<x;
//         cout<<y;
//         return result;
//     }
//     //计算平面向量与平面上的一个点
//     Plane p=getPlanefromPoint(s.center,x.center,y.center);
//     //计算四个点的位置（面与圆交）
//     vector<Point3D> points1=getPonintfromPlanandCirc(p,x);
//     vector<Point3D> points2=getPonintfromPlanandCirc(p,y);
//     //交换四个点的顺序
//     pointSeq(points1,points2);
//     //计算圆锥的顶点在哪里
//     Point3D top=findPoint(points1,points2);
//     //计算方向与半角度
//     Vector3d temp1=points1[0]-points2[0];
//     Vector3d temp2=points1[1]-points2[1];
//     double angle=acos(temp1.dot(temp2)/(temp1.norm()*temp2.norm()));
//     Vector3d dir=findAngularBisector(points1[0],points2[0],top);
//     result.top=top;
//     result.dir=dir;
//     result.angle=angle;
//     return result;
// }