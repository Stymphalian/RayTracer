#include <QVector3D>
#include "Utils/linearalgebra.h"
#include "Models/Primitive.h"
#include "Ray.h"

Ray::Ray(){
    this->refractIndex = 1.0f;
}
Ray::Ray(jVec3 origin, jVec3 dir){
    this->origin = origin;
    this->dir = dir;
    this->refractIndex = 1.0f;
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

// return true if there is total internal reflection
// true otherwise, and the desired output ray is placed into
// the outputRay variable.
// output_dir = (n/n_t)*(d - n(d*n)) - n*sqrt(1 - (n^2/n_t^2)(1- (d*n)^2))
bool Ray::refract(jVec3& origin, jVec3& incomingDir,jVec3& normal,
    float inRefractionIndex, float outRefractionIndex,
    Ray* outputRay)
{    
    float dn = incomingDir*normal;
    float ratio = inRefractionIndex/outRefractionIndex;
    float descriminant = 1 - ratio*ratio*(1-dn*dn);
    if( descriminant < 0){
        return true;
    }

    outputRay->origin = origin;

    jVec3 p = ratio*(incomingDir - normal*dn);
    outputRay->dir = p - normal*sqrt(descriminant);
    outputRay->dir.normalize();
    return false;
}
