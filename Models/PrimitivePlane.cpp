#include "PrimitivePlane.h"
#include "../Utils/linearalgebra.h"
#include "../RayTracer/Ray.h"
#include "../RayTracer/HitRecord.h"

PrimitivePlane::PrimitivePlane(jVec3 p1,jVec3 p2,jVec3 p3, jVec3 p4): PrimitiveTriMesh(){
    vertex_pool.push_back(p1);
    vertex_pool.push_back(p2);
    vertex_pool.push_back(p3);
    vertex_pool.push_back(p4);

    vertex_indices.push_back(jVec3(0,1,3));
    vertex_indices.push_back(jVec3(1,2,3));
    has_bounding_box = false;
}
PrimitivePlane::PrimitivePlane(const PrimitivePlane& other): PrimitiveTriMesh(other){

}
PrimitivePlane::~PrimitivePlane(){}

PrimitivePlane* PrimitivePlane::clone() const{
    return new PrimitivePlane(*this);
}