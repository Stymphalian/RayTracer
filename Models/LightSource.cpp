#include "LightSource.h"
#include "Primitive.h"
#include "RayTracer/HitRecord.h"

LightSource::LightSource(Primitive* wrapped, float intensity) : Primitive(){
    this->wrapped = wrapped;
    this->intensity = intensity;
}

LightSource::LightSource(const LightSource& other): Primitive(other),
    intensity(other.intensity)
{
    wrapped = other.wrapped->clone();
}


LightSource::~LightSource(){
    delete wrapped;
}

jVec3 LightSource::getDirection(jVec3& hitPoint){
    return (getOrigin() - hitPoint);
}

jVec3 LightSource::getEmmitance(){
    return this->intensity*this->wrapped->material.color;
}

void LightSource::draw(jMat4& transform){
    this->wrapped->material = this->material;
    this->wrapped->draw(transform);
}

bool LightSource::intersects(Ray& ray,HitRecord& rs, jMat4& transform){
    return this->wrapped->intersects(ray,rs,transform);
    //return this->wrapped->intersects(ray,rs,transform);
}
jVec3 LightSource::getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit){
    return this->wrapped->getNormal(hitPoint,transform,hit);
    //return this->wrapped->getNormal(hitPoint,transform,hit);
}
jVec3 LightSource::getOrigin(){
    return this->wrapped->getOrigin();
}
void LightSource::flatten(jMat4& transform){
    this->isFlat = true;
    this->wrapped->flatten(transform);
}

LightSource* LightSource::clone() const{
    return new LightSource(*this);
}

