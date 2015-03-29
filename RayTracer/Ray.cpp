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

Ray Ray::reflect(jVec3& origin,jVec3& normal){
    Ray r;
    r.origin = origin;
    r.dir = (dir - 2*(dir*normal)*normal).normalize();
    return r;
}

Ray Ray::refract(jVec3& origin,jVec3& dir,jVec3& normal,float inRefractIndex,float outRefractIndex){
    Ray r;
    r.origin = origin;
    float dn = dir*normal;
    float ratio = inRefractIndex/outRefractIndex;
    jVec3 temp = dir - normal*(dir*normal)*ratio;
    r.dir = temp - normal*sqrt(1 - (ratio*ratio*( 1 - (dn*dn))));
    r.dir.normalize();
    return r;
}
