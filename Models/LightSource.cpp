#include "LightSource.h"
#include "RayTracer/HitRecord.h"

LightSource::LightSource(Primitive* wrapped, float intensity) : Primitive(){
    this->wrapped = wrapped;
    this->intensity = intensity;

    this->local_transform.toidentity();
    this->parent_transform.toidentity();
    this->position.setValues(0,0,0);

    // this->local_transform= local_transform;
    // this->parent_transform= parent_transform;
    // this->position = jVec3(0,0,0)*(local_transform*parent_transform);
}

LightSource::~LightSource(){
    delete wrapped;
}

jVec3 LightSource::getDirection(jVec3& hitPoint){
    return (position - hitPoint);
}

void LightSource::draw(jMat4& transform){
    this->wrapped->material = this->material;
    this->wrapped->draw(transform);
}
bool LightSource::intersects(Ray& ray,HitRecord& rs, jMat4& transform){
    jMat4 candTransform = transform*parent_transform;
    return this->wrapped->intersects(ray,rs,candTransform);
    //return this->wrapped->intersects(ray,rs,transform);
}
jVec3 LightSource::getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit){
    jMat4 candTransform = transform*parent_transform;
    return this->wrapped->getNormal(hitPoint,candTransform,hit);
    //return this->wrapped->getNormal(hitPoint,transform,hit);
}

