#ifndef _AABB_H_
#define _AABB_H_

#include "../RayTracer/Ray.h"
#include  "../Utils/linearalgebra.h"

class AABB {
public:
    jVec3 lb;
    jVec3 rt;

    AABB();
    AABB(jVec3 min,jVec3 max);
    virtual ~AABB();

    virtual bool intersects(Ray& ray);
    virtual void draw(jMat4& transform);
};

#endif
