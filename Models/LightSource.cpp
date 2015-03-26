#include "LightSource.h"

LightSource::LightSource(){
    position = jVec3(0,0,0);
    shape = NULL;
    intensity = 1.0f;
}

LightSource::~LightSource(){
    delete shape;
}

jVec3 LightSource::getDirection(jVec3& hitPoint){
    return (position - hitPoint);
}

bool  LightSource::intersects(Ray& ray,HitRecord& hitRecord, jMat4& transform){
    if(shape != NULL){
        return shape->intersects(ray,hitRecord, transform);
    }else{
        return false;
    }
}
