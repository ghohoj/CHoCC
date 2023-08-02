//求解一元二次方程
# pragma once
#include <iostream>
#include <cmath>
#include<vector>
using namespace std;
 
vector<double> quadraticequation(double a,double b,double c){
    double x1, x2, discriminant, realPart, imaginaryPart;
    vector<double> result;
    discriminant = b*b - 4*a*c;
    if (discriminant > 0) {
        x1 = (-b + sqrt(discriminant)) / (2*a);
        x2 = (-b - sqrt(discriminant)) / (2*a);
        result.push_back(x1);
        result.push_back(x2);
        return result;
    }

    else if (discriminant == 0) {
        cout << "实根相同,可能存在问题" << endl;
        x1 = (-b + sqrt(discriminant)) / (2*a);
        result.push_back(x1);
        return result;
    }

    else {
        cout<<"这个方程存在巨大的问题";
        return result;
    }
}