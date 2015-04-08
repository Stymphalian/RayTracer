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
    this->radius_vector = jVec3(radius,0,0);
    this->num_lats = 20;
    this->num_longs = 20;

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

            jVec3 v(x,y,z);
            vertices.push_back(v);
        }
    }

    // ignore the last.
    for(int lat = 0; lat  < num_lats -1; ++lat){
        for(int longs = 0; longs < num_longs ; ++longs){
            int next_long = (longs + 1) % num_longs;
            int next_lat = lat + 1;

            // make two triangle whcih represent a quad face of the sphere.
            int v1 = lat*num_longs + longs;
            int v2 = next_lat*num_longs + longs;
            int v3 = lat*num_longs + next_long;
            jVec3 v(v1,v2,v3);
            vertex_indices.push_back(v);

            v1 = next_lat*num_longs + longs;
            v2 = next_lat*num_longs + next_long;
            v3 = lat*num_longs + next_long;
            v.setValues(v1,v2,v3);
            vertex_indices.push_back(v);
        }
    }
}

PrimitiveSphere::PrimitiveSphere(const PrimitiveSphere& other) : Primitive(other){
    pos = other.pos;
    radius = other.radius;
    radius_vector = other.radius_vector;
    vertices = other.vertices;
    vertex_indices = other.vertex_indices;
    num_lats = other.num_lats;
    num_longs = other.num_longs;
}

PrimitiveSphere::~PrimitiveSphere(){}

void PrimitiveSphere::draw(jMat4& transform){

    glColor3f(material.color[0],material.color[1],material.color[2]);

    jVec3 v;
    int size = vertex_indices.size();
    glBegin(GL_TRIANGLES);
    for(int i = 0 ;i < size; ++i){
        v = vertices[vertex_indices[i][0]]*transform;
        glVertex3f(v[0],v[1],v[2]);

        v = vertices[vertex_indices[i][1]]*transform;
        glVertex3f(v[0],v[1],v[2]);

        v = vertices[vertex_indices[i][2]]*transform;
        glVertex3f(v[0],v[1],v[2]);
    }
    glEnd();

    // glBegin(GL_POINTS);
    // glColor3f(material.color[0],material.color[1],material.color[2]);
    // jVec3 v;
    // for(int i = 0;i < (int) vertices.size(); ++i)
    // {
    //     v = vertices[i]*transform;
    //     //qDebug("%f %f %f --> %f %f %f",vertices[i][0],vertices[i][1],vertices[i][2],v[0],v[1],v[2]);
    //     glVertex3f(v[0],v[1],v[2]);
    // }
    // glEnd();

   //  qDebug("---matrix begin----");
   // GLfloat matrix[16];
   // transform.toOpenGLMat(matrix);
   // for(int i = 0;i < 4; ++i){
   //     qDebug("%f %f %f %f",matrix[i*4],matrix[i*4 +1],matrix[i*4 + 2],matrix[i*4 +3]);
   // }
   // qDebug("");
   // qDebug("----");
}

bool PrimitiveSphere::intersects(Ray& ray, HitRecord& hitRecord, jMat4& transform)
{
    jVec3 transformed_origin = (isFlat) ? this->pos : this->pos*transform;
    jVec3 transformed_radius = (isFlat) ? this->radius_vector : this->radius_vector*transform;
    float radius = (transformed_radius - transformed_origin).length();

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
    jVec3 trans_pos             = (isFlat) ? pos : pos*transform;
    jVec3 transformed_radius    = (isFlat) ? radius_vector : radius_vector*transform;
    float radius = (transformed_radius - trans_pos).length();
    return (hitPoint - trans_pos)/radius;
}

jVec3 PrimitiveSphere::getOrigin(){
    return pos;
}
void  PrimitiveSphere::flatten(jMat4& transform){
    isFlat = true;

    pos = pos*transform;
    radius_vector = radius_vector*transform;
    radius = (radius_vector - pos).length();

    for(int i = 0;i < (int)vertices.size(); ++i)
    {
        vertices[i] = vertices[i]*transform;
    }
}

PrimitiveSphere* PrimitiveSphere::clone() const{
    return new PrimitiveSphere(*this);
}
