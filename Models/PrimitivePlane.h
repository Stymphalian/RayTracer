#ifndef _PRIMITIVE_PLANE_H_
#define _PRIMITIVE_PLANE_H_

#include "PrimitiveTriMesh.h"
#include "../Utils/linearalgebra.h"
#include "../RayTracer/Ray.h"
#include "../RayTracer/HitRecord.h"

class PrimitivePlane : public PrimitiveTriMesh
{
public:
    PrimitivePlane(jVec3 a,jVec3 b,jVec3 c,jVec3 d);
    virtual ~PrimitivePlane();

    // virtual void  draw(jMat4& transform);
    // virtual bool  intersects(Ray& ray,HitRecord& rs, jMat4& transform);
    // virtual jVec3 getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit);
};

#endif
