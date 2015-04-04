#include "AABB.h"
#include "../Utils/linearalgebra.h"
#include "../RayTracer/Ray.h"

AABB::AABB(){
    lb = jVec3(0,0,0);
    rt = jVec3(0,0,0);
}

AABB::AABB(jVec3 min,jVec3 max){
    this->lb = min;
    this->rt = max;
}

AABB::~AABB(){}

//Reference :http://gamedev.stackexchange.com/questions/18436/most-efficient-aabb-vs-ray-collision-algorithms
bool AABB::intersects(Ray& r){
    float t;
    jVec3 dirfrac;
    // r.dir is unit direction vector of ray
    dirfrac[0] = 1.0f / r.dir[0];
    dirfrac[1] = 1.0f / r.dir[1];
    dirfrac[2] = 1.0f / r.dir[2];
    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    // r.org is origin of ray
    float t1 = (lb[0] - r.origin[0])*dirfrac[0];
    float t2 = (rt[0] - r.origin[0])*dirfrac[0];
    float t3 = (lb[1] - r.origin[1])*dirfrac[1];
    float t4 = (rt[1] - r.origin[1])*dirfrac[1];
    float t5 = (lb[2] - r.origin[2])*dirfrac[2];
    float t6 = (rt[2] - r.origin[2])*dirfrac[2];

    float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
    float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
    if (tmax < 0)
    {
        t = tmax;
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
        t = tmax;
        return false;
    }

    t = tmin;
    return true;
}
