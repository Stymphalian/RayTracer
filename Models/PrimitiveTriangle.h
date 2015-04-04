#ifndef _PRIMITIVE_TRIANGLE_H_
#define _PRIMITIVE_TRIANGLE_H_

#include "Primitive.h"
#include "PrimitiveTriMesh.h"
#include "../Utils/linearalgebra.h"

class PrimitiveTriangle : public PrimitiveTriMesh
{
public:
    PrimitiveTriangle();
    PrimitiveTriangle(jVec3 p1,jVec3 p2, jVec3 p3);
    PrimitiveTriangle(const PrimitiveTriangle& other);
    virtual ~PrimitiveTriangle();

private:
    void setPoints(jVec3& p1, jVec3& p2, jVec3& p3);
};

#endif
