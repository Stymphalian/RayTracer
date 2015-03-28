#ifndef _PRIMITIVE_TRIANGLE_H_
#define _PRIMITIVE_TRIANGLE_H_

#include "Primitive.h"
#include "../Utils/linearalgebra.h"

class PrimitiveTriangle : public Primitive
{
public:
    jVec3 points[3];
    jVec3 normal;

    PrimitiveTriangle();
    PrimitiveTriangle(jVec3 p1,jVec3 p2, jVec3 p3);
    virtual ~PrimitiveTriangle();

    virtual void draw(jMat4& transform);
    virtual bool intersects(Ray& ray,HitRecord& rs, jMat4& transform) ;
    virtual jVec3 getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit) ;

    void setPoints(jVec3& p1, jVec3& p2, jVec3& p3);    
};

#endif
