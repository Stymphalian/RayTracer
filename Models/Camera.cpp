#include "Camera.h"

Camera::Camera(){
    pos = jVec3(0,0,0);
    dir = jVec3(0,0,0);
}

Camera::~Camera(){}

void Camera::LookAt(jVec3 eye, jVec3 at, jVec3 up)
{
    // modify the pos
    pos = eye;
    // modify the at
    this->at = at;
    // modify the dir
    dir = (at - eye).normalize();
    // modify the up
    this->up = up;
    // set the focal length as the distance between the at and eye
    // focalLength = (at - eye).length();

    // set the u,v,w basis vectors
    w = -dir;
    u = (up.normalize() ^ w).normalize();
    v = w ^ u;

    // create the matrix
    jMat4 rot(
        u[0],u[1],u[2],0,
        v[0],v[1],v[2],0,
        w[0],w[1],w[2],0,
        0,0,0,1);
    jMat4 trans(
        1,0,0,eye[0],
        0,1,0,eye[1],
        0,0,1,eye[2],
        0,0,0,1 );
    trans = trans.transpose();
    rotMatrix = rot * trans;
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
