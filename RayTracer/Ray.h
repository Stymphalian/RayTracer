#ifndef _RAY_H_
#define _RAY_H_

#include "Utils/linearalgebra.h"

class Ray {
public:
    jVec3 origin;
    jVec3 dir;
    jVec3 u;
    jVec3 v;
    jVec3 w;

    Ray();
    Ray(jVec3 origin, jVec3 dir);

    void calcRay(double renderWidth, double renderHeight,
            double col, double row, jVec3& o, double focalLength);

    void calcUVW(jVec3& gaze, jVec3& up);
    void calcUVW(jVec3 gaze, jVec3 up);
    Ray reflect(jVec3& origin,jVec3& normal);
    Ray refract(jVec3& origin,jVec3& dir,jVec3& normal,float inRefractIndex,float outRefractIndex);

    jVec3 getPoint(double dist);
};

#endif
