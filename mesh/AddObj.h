#pragma once

#include"../geo/geodisplay.h"

void addObj(print3d& result,print3d& x){
    int sizenum=result.p.size();
    Vector3i tmpsize(sizenum,sizenum,sizenum);
    result.p.insert(result.p.begin(),x.p.begin(),x.p.end());
    for(auto i:x.f){
        result.f.push_back(i+tmpsize);
    }
}