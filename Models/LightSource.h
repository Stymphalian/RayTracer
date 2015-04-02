#ifndef _LIGHT_SOURCE_H_
#define _LIGHT_SOURCE_H_

#include "Primitive.h"

class LightSource : public Primitive {
public:
    // These must be set by the world model manager
    jMat4 local_transform;
    jMat4 parent_transform;
    jVec3 position;


    Primitive* wrapped;
    float intensity;

    // methods
    LightSource(Primitive* wrappee,float intensity);
    virtual ~LightSource();

    // LightSource methods
    jVec3 getDirection(jVec3& hitPoint);
    jVec3 getEmmitance();

    // from Primitive
    virtual void draw(jMat4& transform);
    virtual bool intersects(Ray& ray,HitRecord& rs, jMat4& transform);
    virtual jVec3 getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit);
};

#endif
