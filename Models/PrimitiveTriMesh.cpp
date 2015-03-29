#include <QtOpenGL>
#include <vector>

#include "PrimitiveTriMesh.h"
#include "Primitive.h"
#include "../Utils/linearalgebra.h"
#include "../RayTracer/HitRecord.h"
#include "../RayTracer/Ray.h"


PrimitiveTriMesh::PrimitiveTriMesh():Primitive(){}
PrimitiveTriMesh::~PrimitiveTriMesh(){
    vertex_pool.clear();
    indices.clear();
}

void  PrimitiveTriMesh::draw(jMat4& transform){
    glColor3f(material.color[0],material.color[1],material.color[2]);

    jVec3 v;
    int size = indices.size();
    glBegin(GL_TRIANGLES);
    for(int i = 0 ;i < size; ++i){
        v = vertex_pool[indices[i][0]]*transform;
        glVertex3f(v[0],v[1],v[2]);

        v = vertex_pool[indices[i][1]]*transform;
        glVertex3f(v[0],v[1],v[2]);

        v = vertex_pool[indices[i][2]]*transform;
        glVertex3f(v[0],v[1],v[2]);
    }
    glEnd();
}


bool  PrimitiveTriMesh::_intersects(Ray& ray,HitRecord& rs, jMat4& transform,int index)
{
    // calculate the translated points
    jVec3 a_trans = vertex_pool[indices[index][0]]*transform;
    jVec3 b_trans = vertex_pool[indices[index][1]]*transform;
    jVec3 c_trans = vertex_pool[indices[index][2]]*transform;

    // calculate the variables
    jVec3 D = ray.dir;
    jVec3 E1 = b_trans - a_trans;
    jVec3 E2 = c_trans - a_trans;
    jVec3 T = ray.origin - a_trans;

    jVec3 Q = T^E1;
    jVec3 P = D^E2;
    float det = P*E1;

    if( very_close(det,0.0,EPSILON)){
        return false;
    }

    float inv_det = 1.0f/det;

    // calculate the u
    float u = P*T*inv_det;
    if( u < 0 || very_close(u,0.0f,EPSILON)){
        return false;
    }

    // calculate the v
    float v = Q*D*inv_det;
    if( v < 0.0f || very_close(v,0.0f,EPSILON)){
        return false;
    }

    // u + v is too big
    if( u+v > 1.0f || very_close(u+v,1.0f + EPSILON,EPSILON) ){
        return false;
    }

    // calucalute the distance.
    float t = Q*E2*inv_det;
    if( t < rs.min_dist || t > rs.max_dist){
        // out of range
        return false;
    }

    rs.hit = true;
    rs.dist = t;
    rs.max_dist = t;
    rs.hitIndex = index;
    return true;
}

bool  PrimitiveTriMesh::intersects(Ray& ray,HitRecord& rs, jMat4& transform){
    // check bounding volume.

    int size = indices.size();
    for(int i = 0;i < size; ++i)
    {
        _intersects(ray,rs,transform,i);
    }
    return rs.hit;
}


// retrieve the normal for a particular index
jVec3 PrimitiveTriMesh::_getNormal(jMat4& transform,int hitIndex){
    jVec3 p1_trans = vertex_pool[indices[hitIndex][0]]*transform;
    jVec3 a = (vertex_pool[indices[hitIndex][1]]*transform) - p1_trans;
    jVec3 b = (vertex_pool[indices[hitIndex][2]]*transform) - p1_trans;
    return (a^b).normalize();
}

jVec3 PrimitiveTriMesh::getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit){
    if( hit.hitIndex == -1){
        return jVec3(0,0,0);
    }
    return _getNormal(transform,hit.hitIndex);
}
