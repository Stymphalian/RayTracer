#include <QVector3D>
#include "Utils/linearalgebra.h"
#include "Models/Primitive.h"
#include "Ray.h"

Ray::Ray(){}
Ray::Ray(jVec3 origin, jVec3 dir){
    this->origin = origin;
    this->dir = dir;
}

void Ray::calcRay(double renderWidth, double renderHeight, double col, double row,
                    jVec3& o, double focalLength)
{
    origin = o;
    double r = renderWidth / 2;
    double l = -r;
    double t = renderHeight / 2;
    double b = -t;
    double subu = l + (r - l) * (col + 0.5) / renderWidth;
    double subv = b + (t - b) * (row + 0.5) / renderHeight;
    dir = focalLength*-w + subu * u + subv * v;
}


void Ray::calcUVW(jVec3& gaze, jVec3& up){
    w = (-gaze).normalize();
    u = (up^w).normalize();
    v = (w^u).normalize();
}

void Ray::calcUVW(jVec3 gaze, jVec3 up){
    w = (-gaze).normalize();
    u = (up^w).normalize();
    v = (w^u).normalize();
}

jVec3 Ray::getPoint(double dist){
    return (origin + dist*dir);
}
