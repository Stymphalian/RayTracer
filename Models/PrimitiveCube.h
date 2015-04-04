#ifndef _PRIMITIVE_CUBE_H_
#define _PRIMITIVE_CUBE_H_

#include <vector>
#include "PrimitiveTriMesh.h"
#include "../RayTracer/HitRecord.h"
#include "../Utils/linearalgebra.h"

class PrimitiveCube : public PrimitiveTriMesh
{
public:
    jVec3 min,max;

    PrimitiveCube(jVec3 min, jVec3 max);
    PrimitiveCube(const PrimitiveCube& other);
    virtual ~PrimitiveCube();

    virtual void flatten(jMat4& transform);
    virtual PrimitiveCube* clone() const;
protected:
    void orderMinMax(jVec3& min,jVec3& max);
};

#endif
