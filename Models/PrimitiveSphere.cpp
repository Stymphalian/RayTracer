#include <QtOpenGL>
#if defined(Q_WS_MAC)
    #include <OpenGL/glu.h>
#else
    #include <GL/glu.h>
#endif

#include "Utils/linearalgebra.h"
#include "PrimitiveSphere.h"
#include "RayTracer/Ray.h"
#include "RayTracer/HitRecord.h"


PrimitiveSphere::PrimitiveSphere(jVec3 pos, double radius) : Primitive()
{
    this->pos = pos;
    this->radius = radius;

    int num_lats = 20;
    int num_longs = 20;
    float x,y,z,r;
    const double pi = 3.14159265358979323;
    const float long_divs = pi/num_longs;
    const float lat_divs = 2*pi/num_lats;

    for(int longs = 0; longs  < num_longs; ++longs){
        y = cos(long_divs*longs)*radius;
        r = sin(long_divs*longs)*radius;

        for(int lat = 0; lat < num_lats; ++lat){
            x = cos(lat_divs*lat)*r;
            z = sin(lat_divs*lat)*r;
            vertices.push_back( jVec3(x,y,z));
        }
    }

}

PrimitiveSphere::~PrimitiveSphere(){}

void PrimitiveSphere::draw(jMat4& transform){
    glBegin(GL_POINTS);
    glColor3f(1.0,0.0,0.0);
     jVec3 v;
     for(int i = 0;i < vertices.size(); ++i)
     {
         v = vertices[i]*transform;
         glVertex3f(v[0],v[1],v[2]);
     }
    glEnd();
}

bool PrimitiveSphere::intersects(Ray& ray, HitRecord& hitRecord, jMat4& transform)
{
    jVec3 transformed_origin = this->pos*transform;

    // origin - center
    jVec3 omc = ray.origin - transformed_origin;

    double A = ray.dir*ray.dir;
    double B = (2*ray.dir)*(omc);
    double C = (omc*omc) - radius*radius;

    // calculate the descriminant and make sure that it has a solution
    double descriminant = B*B - 4*A*C;
    if( descriminant < EPSILON){
        hitRecord.hit = false;
        return false;
    }

    // calculate the two possible solutions
    descriminant = sqrt(descriminant);
    double dist1 = (-B + descriminant)/(2*A);
    double dist2 = (-B - descriminant)/(2*A);
    double dist = 0;
    if( dist2 < dist1){
        dist = dist2;
    }else{
        dist = dist1;
    }

    // make sure the dist is in range.
    if( dist > hitRecord.min_dist && dist < hitRecord.max_dist){
        hitRecord.hit = true;
        hitRecord.dist = dist;

        hitRecord.max_dist = hitRecord.dist;
        return true;
    }

    return false;
}

// normal to a sphere is the exactly the difference vector between
// the intersection point and the origin of the sphere.
jVec3 PrimitiveSphere::getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit){
    jVec3 trans_pos = pos*transform;
    return (hitPoint - trans_pos)/radius;
}
