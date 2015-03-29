#include "PrimitivePlane.h"
#include "../Utils/linearalgebra.h"
#include "../RayTracer/Ray.h"
#include "../RayTracer/HitRecord.h"

PrimitivePlane::PrimitivePlane(jVec3 p1,jVec3 p2,jVec3 p3, jVec3 p4): PrimitiveTriMesh(){
    vertex_pool.push_back(p1);
    vertex_pool.push_back(p2);
    vertex_pool.push_back(p3);
    vertex_pool.push_back(p4);

    indices.push_back(jVec3(0,1,3));
    indices.push_back(jVec3(1,2,3));
}
PrimitivePlane::~PrimitivePlane(){}

// void  PrimitivePlane::draw(jMat4& transform){}
// bool  PrimitivePlane::intersects(Ray& ray,HitRecord& rs, jMat4& transform){}
// jVec3 PrimitivePlane::getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit){}