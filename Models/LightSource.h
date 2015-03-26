#ifndef _LIGHT_SOURCE_H_
#define _LIGHT_SOURCE_H_

#include "Primitive.h"

class LightSource{
public:
    // variables
    jVec3 position;
    Primitive* shape;
    float intensity;

    // methods
    LightSource();
    virtual ~LightSource();

    jVec3 getDirection(jVec3& hitPoint);
    bool intersects(Ray& ray,HitRecord& hitRecord, jMat4& transform);
};

#endif