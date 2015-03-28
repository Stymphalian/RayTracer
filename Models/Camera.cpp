#include <QDebug>
#include "Camera.h"

Camera::Camera(){
    pos = jVec3(0,0,0);
    dir = jVec3(0,0,0);
}

Camera::~Camera(){}

void Camera::LookAt(jVec3 eye, jVec3 at, jVec3 up_dir)
{
    // modify the pos
    this->pos = eye;
    // modify the at
    this->at = at;
    // modify the up
    this->up_dir = up_dir;
    // modify the dir
    this->dir = (at - eye).normalize();
    // set the focal length as the distance between the at and eye
    //focalLength = 1500;


    forward = -(at-eye).normalize();
    left = (up_dir^forward).normalize();
    up = (forward^left).normalize();

    jMat4 translate(1,0,0,-eye[0],
                    0,1,0,-eye[1],
                    0,0,1,-eye[2],
                    0,0,0,1);
    jMat4 rot(left[0],left[1],left[2],0,
            up[0],up[1],up[2],0,
            forward[0],forward[1],forward[2],0,
            0,0,0,1);
    rotMatrix = (rot*translate).transpose();
}

void Camera::LookAt(
        float eyex, float eyey, float eyez,
        float atx, float aty, float atz,
        float upx, float upy, float upz)
{
    jVec3 eye(eyex,eyey,eyez);
    jVec3 at(atx,aty,atz);
    jVec3 up(upx,upy,upz);
    LookAt(eye,at,up);
}

void Camera::LookAt()
{
    LookAt(this->pos,this->at,jVec3(0,1,0));
}
