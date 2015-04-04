#include <QtOpenGL>
#include <vector>

#include "PrimitiveTriMesh.h"
#include "Primitive.h"
#include "../Utils/linearalgebra.h"
#include "../RayTracer/HitRecord.h"
#include "../RayTracer/Ray.h"
#include "AABB.h"

PrimitiveTriMesh::PrimitiveTriMesh():Primitive(){
    has_bounding_box = true;
}

PrimitiveTriMesh::PrimitiveTriMesh(const PrimitiveTriMesh& other) : Primitive(other){
    vertex_pool = other.vertex_pool;
    texture_pool = other.texture_pool;
    normal_pool = other.normal_pool;
    vertex_indices = other.vertex_indices;
    texture_indices = other.texture_indices;
    normal_indices = other.normal_indices;

    boundingbox = other.boundingbox;
    has_bounding_box = other.has_bounding_box;
}

PrimitiveTriMesh::~PrimitiveTriMesh(){
    vertex_pool.clear();
    texture_pool.clear();
    normal_pool.clear();
    vertex_indices.clear();
    texture_indices.clear();
    normal_indices.clear();
}

bool PrimitiveTriMesh::hasBoundingBox(){
    return has_bounding_box;
}
bool PrimitiveTriMesh::intersectsBoundingBox(Ray& ray){
    return boundingbox.intersects(ray);
}

void  PrimitiveTriMesh::draw(jMat4& transform){
    glColor3f(material.color[0],material.color[1],material.color[2]);

    jVec3 v;
    int size = vertex_indices.size();
    glBegin(GL_TRIANGLES);
    for(int i = 0 ;i < size; ++i){
        v = vertex_pool[vertex_indices[i][0]]*transform;
        glVertex3f(v[0],v[1],v[2]);

        v = vertex_pool[vertex_indices[i][1]]*transform;
        glVertex3f(v[0],v[1],v[2]);

        v = vertex_pool[vertex_indices[i][2]]*transform;
        glVertex3f(v[0],v[1],v[2]);
    }
    glEnd();

    glEnableClientState(GL_VERTEX_ARRAY);
}

// justin guze - https://github.com/jguze/csc305_graphics
bool PrimitiveTriMesh::_intersects(Ray& ray,HitRecord& hit,jMat4& transform, int index)
{
    jVec3 vec_a = vertex_pool[vertex_indices[index][0]];
    jVec3 vec_b = vertex_pool[vertex_indices[index][1]];
    jVec3 vec_c = vertex_pool[vertex_indices[index][2]];

    if(isFlat == false){
        vec_a = vec_a*transform;
        vec_b = vec_b*transform;
        vec_c = vec_c*transform;
    }

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
    if(hasBoundingBox()){
        if( intersectsBoundingBox(ray) == false){
            rs.hit = false;
            return false;
        }
    }

    int size = vertex_indices.size();
    for(int i = 0;i < size; ++i)
    {
        _intersects(ray,rs,transform,i);
    }
    return rs.hit;
}

// retrieve the normal for a particular index
jVec3 PrimitiveTriMesh::_getNormal(jMat4& transform,int hitIndex){
    if(isFlat){
        jVec3 p1_trans = vertex_pool[vertex_indices[hitIndex][0]];
        jVec3 a = vertex_pool[vertex_indices[hitIndex][1]] - p1_trans;
        jVec3 b = vertex_pool[vertex_indices[hitIndex][2]] - p1_trans;
        return (a^b).normalize();
    }else{
        jVec3 p1_trans = vertex_pool[vertex_indices[hitIndex][0]]*transform;
        jVec3 a = (vertex_pool[vertex_indices[hitIndex][1]]*transform) - p1_trans;
        jVec3 b = (vertex_pool[vertex_indices[hitIndex][2]]*transform) - p1_trans;
        return (a^b).normalize();
    }
}

jVec3 PrimitiveTriMesh::getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit){
    if( hit.hitIndex == -1){
        return jVec3(0,0,0);
    }
    return _getNormal(transform,hit.hitIndex);
}


void PrimitiveTriMesh::fillTriMeshFromObjFile(ObjFileReader::Obj_Model& model){
    this->vertex_pool = model.vertices;
    this->texture_pool= model.textures;
    this->normal_pool = model.normals;
    this->vertex_indices = model.vertex_indices;
    this->texture_indices = model.texture_indices;
    this->normal_indices = model.normal_indices;

    for(int i = 0;i < (int)this->vertex_indices.size(); ++i){
        this->vertex_indices[i][0]= this->vertex_indices[i][0] -1;
        this->vertex_indices[i][1]= this->vertex_indices[i][1] -1;
        this->vertex_indices[i][2]= this->vertex_indices[i][2] -1;
    }

    for(int i = 0;i < (int)this->texture_indices.size(); ++i){
        this->texture_indices[i][0]= this->texture_indices[i][0] -1;
        this->texture_indices[i][1]= this->texture_indices[i][1] -1;
        this->texture_indices[i][2]= this->texture_indices[i][2] -1;
    }

    for(int i = 0;i < (int)this->normal_indices.size(); ++i){
        this->normal_indices[i][0]= this->normal_indices[i][0] -1;
        this->normal_indices[i][1]= this->normal_indices[i][1] -1;
        this->normal_indices[i][2]= this->normal_indices[i][2] -1;
    }

    // calculate the bounding box
}

AABB PrimitiveTriMesh::_getBoundingBox()
{
    AABB b;
    float min_x = this->vertex_pool[0][0];
    float min_y = this->vertex_pool[0][1];
    float min_z = this->vertex_pool[0][2];
    float max_x = this->vertex_pool[0][0];
    float max_y = this->vertex_pool[0][1];
    float max_z = this->vertex_pool[0][2];

    for(int i = 0; i< (int)this->vertex_pool.size(); ++i){
        if( this->vertex_pool[i][0] < min_x){
            min_x = this->vertex_pool[i][0];
        }
        if( this->vertex_pool[i][1] < min_y){
            min_y = this->vertex_pool[i][1];
        }
        if( this->vertex_pool[i][2] < min_y){
            min_z = this->vertex_pool[i][2];
        }

        if( this->vertex_pool[i][0] > max_x){
            max_x = this->vertex_pool[i][0];
        }
        if( this->vertex_pool[i][1] > max_y){
            max_y = this->vertex_pool[i][1];
        }
        if( this->vertex_pool[i][2] > max_y){
            max_z = this->vertex_pool[i][2];
        }
    }

    b.lb = jVec3(min_x,min_y,max_z);
    b.rt = jVec3(max_x,max_y,min_z);
    return b;
}

jVec3 PrimitiveTriMesh::getOrigin(){
    float x = 0;
    float y = 0;
    float z = 0;

    int size = vertex_pool.size();
    for(int i = 0;i < size; ++i){
        x += vertex_pool[i][0];
        y += vertex_pool[i][1];
        z += vertex_pool[i][2];
    }

    // return the mean between all the points.
    return jVec3(x/size, y/size, z/size);
}

void  PrimitiveTriMesh::flatten(jMat4& transform){
    isFlat = true;

    int size = vertex_pool.size();
    for(int i = 0;i < size; ++i)
    {
        vertex_pool[i] = vertex_pool[i]*transform;
    }

    jMat4 inv_trans = transform.inverse().transpose();
    size = normal_pool.size();
    for(int i = 0;i < size; ++i)
    {
        normal_pool[i] = normal_pool[i]*inv_trans;
    }

    boundingbox.lb = boundingbox.lb*transform;
    boundingbox.rt = boundingbox.rt*transform;
}

PrimitiveTriMesh* PrimitiveTriMesh::clone() const{
    return new PrimitiveTriMesh(*this);
}
