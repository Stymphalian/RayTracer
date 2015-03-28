#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Utils/linearalgebra.h"

class Camera{
public:
    Camera();
    virtual ~Camera();

    // changes the  pos,dir u,v,w vectors
    void LookAt(jVec3 eye, jVec3 at, jVec3 up);
    void LookAt(
        float eyex, float eyey, float eyez,
        float atx, float aty, float atz,
        float upx, float upy, float upz);
    void LookAt();


    jVec3 pos;
    jVec3 at;
    jVec3 up_dir;
    jVec3 dir;
    float focalLength;

    jVec3 forward;
    jVec3 left;
    jVec3 up;
    jMat4 rotMatrix;
};


#endif
