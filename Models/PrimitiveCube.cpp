#include "PrimitiveCube.h"
#include "PrimitiveTriMesh.h"
#include "../RayTracer/HitRecord.h"
#include "../Utils/linearalgebra.h"


PrimitiveCube::PrimitiveCube(jVec3 min, jVec3 max): PrimitiveTriMesh(){
    orderMinMax(min,max);
    vertex_pool.push_back(jVec3(min[0],min[1],min[2]) );
    vertex_pool.push_back(jVec3(min[0],max[1],min[2]) );
    vertex_pool.push_back(jVec3(max[0],max[1],min[2]) );
    vertex_pool.push_back(jVec3(max[0],min[1],min[2]) );
    vertex_pool.push_back(jVec3(min[0],min[1],max[2]) );
    vertex_pool.push_back(jVec3(min[0],max[1],max[2]) );
    vertex_pool.push_back(jVec3(max[0],max[1],max[2]) );
    vertex_pool.push_back(jVec3(max[0],min[1],max[2]) );

    vertex_indices.push_back(jVec3(0,3,2));
    vertex_indices.push_back(jVec3(2,1,0));
    vertex_indices.push_back(jVec3(2,3,7));
    vertex_indices.push_back(jVec3(7,6,2));
    vertex_indices.push_back(jVec3(0,4,7));
    vertex_indices.push_back(jVec3(7,3,0));
    vertex_indices.push_back(jVec3(1,2,6));
    vertex_indices.push_back(jVec3(6,5,1));
    vertex_indices.push_back(jVec3(4,5,6));
    vertex_indices.push_back(jVec3(6,7,4));
    vertex_indices.push_back(jVec3(0,1,5));
    vertex_indices.push_back(jVec3(5,4,0));

    has_bounding_box = false;
}
PrimitiveCube::PrimitiveCube(const PrimitiveCube& other): PrimitiveTriMesh(other){
    min = other.min;
    max = other.max;
}

PrimitiveCube::~PrimitiveCube(){}

void PrimitiveCube::orderMinMax(jVec3& min,jVec3& max)
{
    jVec3 _min = min;
    jVec3 _max = max;

    jVec3 dir = (min - max).normalize();
    if( dir[0] < 0 && dir[1] < 0 && dir[2] >= 0){
        this->min = _min;
        this->max = _max;
    }else{
        this->min = _max;
        this->max = _min;
    }
}

void PrimitiveCube::flatten(jMat4& transform) {
    PrimitiveTriMesh::flatten(transform);
    min = min*transform;
    max = max*transform;
}
