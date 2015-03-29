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

// justin guze - https://github.com/jguze/csc305_graphics
bool PrimitiveTriMesh::_intersects(Ray& ray,HitRecord& hit,jMat4& transform, int index)
{
    jVec3 vec_a = (vertex_pool[indices[index][0]]*transform);
    jVec3 vec_b = (vertex_pool[indices[index][1]]*transform);
    jVec3 vec_c = (vertex_pool[indices[index][2]]*transform);

    double t, gamma, beta;
    double a = vec_a[0] - vec_b[0];
    double b = vec_a[1] - vec_b[1];
    double c = vec_a[2] - vec_b[2];
    double d = vec_a[0] - vec_c[0];
    double e = vec_a[1] - vec_c[1];
    double f = vec_a[2] - vec_c[2];
    double j = vec_a[0] - ray.origin[0];
    double k = vec_a[1] - ray.origin[1];
    double l = vec_a[2] - ray.origin[2];

    double ei_hf = (e * ray.dir[2]) -  (f * ray.dir[1]);
    double gf_di = (f * ray.dir[0]) - (d * ray.dir[2]);
    double dh_eg = (d * ray.dir[1]) - (e * ray.dir[0]);

    double M = (a * ei_hf) + (b * gf_di) + (c * dh_eg);

    double ak_jb = (a * k) - (j * b);
    double jc_al = (j * c) - (a * l);
    double bl_kc = (b * l) - (k * c);

    t = - ((f * ak_jb) + (e * jc_al) + (d * bl_kc)) / M;

    if (t < hit.min_dist || t > hit.max_dist)
    {
        return false;
    }

    gamma = ((ray.dir[2] * ak_jb) + (ray.dir[1] * jc_al) + (ray.dir[0] * bl_kc)) / M;

    if (gamma < 0 || gamma > 1)
    {
        return false;
    }

    beta = ((j * ei_hf) + (k * gf_di) + (l * dh_eg)) / M;
    if (beta < 0 || beta > (1 - gamma))
    {
        return false;
    }


    hit.hit = true;
    hit.dist = t;
    hit.max_dist = t;
    hit.hitIndex = index;
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
