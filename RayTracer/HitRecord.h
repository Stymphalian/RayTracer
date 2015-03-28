#ifndef _HIT_RECORD_H_
#define _HIT_RECORD_H_

#include "Utils/linearalgebra.h"
class Primitive;

class HitRecord{
public:
    HitRecord();
    virtual ~HitRecord();

    // use for recursive queries in the ray trace,
    bool hit;
    double dist;
    double min_dist;
    double max_dist;


    Primitive* hitObject;
    jMat4 transform;
    //jVec3 normal;
};


#endif
