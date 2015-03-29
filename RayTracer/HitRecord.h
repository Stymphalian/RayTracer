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


    // Any primtive which intersects with the ray should set the followin properties
    // hit = true
    // dist = dist from the ray it hist
    // min_dist
    // max_dist = update the max_dist to where it was hit
    // hitObject = this;
    // hitNormal = the surface normal in which it hit
    // hitIndex = the index into the index array of the hit triangle
    // transform = the current transform which was used when calculating the intersection

    Primitive* hitObject;
    // BREAKS ENCAPSULATION!
    jVec3 hitNormal;
    int hitIndex; // this is the index into the indices buffer of the tri_mesh
                    // in which the hit intersected with. Only used by the
                    // PrmitivieTriMesh class in order to quickly

    jMat4 transform;
    //jVec3 normal;
};


#endif
