#include <QtOpenGL>
#include "PrimitiveTriangle.h"
#include "PrimitiveTriMesh.h"
#include "../RayTracer/Ray.h"
#include "../RayTracer/HitRecord.h"
#include "../Utils/linearalgebra.h"

PrimitiveTriangle::PrimitiveTriangle(){
    jVec3 zero(0,0,0);
    setPoints(zero,zero,zero);
    has_bounding_box = false;
}
PrimitiveTriangle::PrimitiveTriangle(jVec3 p1,jVec3 p2, jVec3 p3):PrimitiveTriMesh(){
    setPoints(p1,p2,p3);
    has_bounding_box = false;
}
PrimitiveTriangle::~PrimitiveTriangle(){

}

// void  PrimitiveTriangle::draw(jMat4& transform){
//     glColor3f(material.color[0],material.color[1],material.color[2]);
//     glBegin(GL_TRIANGLES);
//     jVec3 v;
//     for(int i = 0;i < 3; ++i){
//         v = points[i]*transform;
//         glVertex3f(v[0],v[1],v[2]);
//     }
//     glEnd();
// }

// // Reference: http://www.graphics.cornell.edu/pubs/1997/MT97.pdf
// bool  PrimitiveTriangle::intersects(Ray& ray,HitRecord& rs, jMat4& transform){
//     //rs.hit = false;

//     // calculate the translated points
//     jVec3 a_trans = points[0]*transform;
//     jVec3 b_trans = points[1]*transform;
//     jVec3 c_trans = points[2]*transform;

//     // calculate the variables
//     jVec3 D = ray.dir;
//     jVec3 E1 = b_trans - a_trans;
//     jVec3 E2 = c_trans - a_trans;
//     jVec3 T = ray.origin - a_trans;

//     jVec3 Q = T^E1;
//     jVec3 P = D^E2;
//     float det = P*E1;

//     if( very_close(det,0.0,EPSILON)){
//         return false;
//     }

//     float inv_det = 1.0f/det;

//     // calculate the u
//     float u = P*T*inv_det;
//     if( u < 0 || very_close(u,0.0f,EPSILON)){
//         return false;
//     }

//     // calculate the v
//     float v = Q*D*inv_det;
//     if( v < 0.0f || very_close(v,0.0f,EPSILON)){
//         return false;
//     }

//     // u + v is too big
//     if( u+v > 1.0f || very_close(u+v,1.0f + EPSILON,EPSILON) ){
//         return false;
//     }

//     // calucalute the distance.
//     float t = Q*E2*inv_det;
//     if( t < rs.min_dist || t > rs.max_dist){
//         // out of range
//         return false;
//     }

//     rs.hit = true;
//     rs.dist = t;
//     rs.max_dist = t;
//     return true;
// }

// jVec3 PrimitiveTriangle::getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit){
//     jVec3 p1_trans = points[0]*transform;
//     jVec3 a = (points[1]*transform) - p1_trans;
//     jVec3 b = (points[2]*transform) - p1_trans;
//     return (a^b).normalize();
// }

void PrimitiveTriangle::setPoints(jVec3& p1, jVec3& p2, jVec3& p3){
    vertex_pool.clear();
    vertex_indices.clear();

    vertex_pool.push_back(p1);
    vertex_pool.push_back(p2);
    vertex_pool.push_back(p3);

    vertex_indices.push_back(jVec3(0,1,2));
}
