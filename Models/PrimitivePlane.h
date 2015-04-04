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
    PrimitivePlane(const PrimitivePlane& other);
    virtual ~PrimitivePlane();

    virtual PrimitivePlane* clone() const;
};

#endif
