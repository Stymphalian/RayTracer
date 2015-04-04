#include <QtOpenGL>
#include "PrimitiveTriangle.h"
#include "PrimitiveTriMesh.h"
#include "../RayTracer/Ray.h"
#include "../RayTracer/HitRecord.h"
#include "../Utils/linearalgebra.h"

PrimitiveTriangle::PrimitiveTriangle(){
    jVec3 zero(0,0,0);
    setPoints(zero,zero,zero);
    has_bounding_box = false;
}
PrimitiveTriangle::PrimitiveTriangle(jVec3 p1,jVec3 p2, jVec3 p3):PrimitiveTriMesh(){
    setPoints(p1,p2,p3);
    has_bounding_box = false;
}
PrimitiveTriangle::PrimitiveTriangle(const PrimitiveTriangle& other): PrimitiveTriMesh(other)
{}

PrimitiveTriangle::~PrimitiveTriangle(){

}


void PrimitiveTriangle::setPoints(jVec3& p1, jVec3& p2, jVec3& p3){
    vertex_pool.clear();
    vertex_indices.clear();

    vertex_pool.push_back(p1);
    vertex_pool.push_back(p2);
    vertex_pool.push_back(p3);

    vertex_indices.push_back(jVec3(0,1,2));
}

PrimitiveTriangle* PrimitiveTriangle::clone() const{
    return new PrimitiveTriangle(*this);
}