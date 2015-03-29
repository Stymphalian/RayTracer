#ifndef _PRIMITIVE_TRI_MESH_H_
#define _PRIMITIVE_TRI_MESH_H_

#include <vector>
#include "Primitive.h"
#include "../Utils/linearalgebra.h"
#include "../RayTracer/HitRecord.h"

class PrimitiveTriMesh : public Primitive
{
public:
    std::vector<jVec3> vertex_pool;
    std::vector<jVec3> indices;

    PrimitiveTriMesh();
    virtual ~PrimitiveTriMesh();

    virtual void draw(jMat4& transform);
    virtual bool intersects(Ray& ray,HitRecord& rs, jMat4& transform);
    virtual jVec3 getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit);

protected:
    bool  _intersects(Ray& ray,HitRecord& rs, jMat4& transform,int index);
    jVec3 _getNormal(jMat4& transform,int hitIndex);
};

#endif
