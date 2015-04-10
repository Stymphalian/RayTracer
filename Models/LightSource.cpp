#include "LightSource.h"
#include "Primitive.h"
#include "PrimitiveTriMesh.h"
#include "RayTracer/HitRecord.h"
#include "Utils/jRand.h"
#include <QtOpenGL>

static GLenum light_enums[] ={
    GL_LIGHT0,
    GL_LIGHT1,
    GL_LIGHT2,
    GL_LIGHT3,
    GL_LIGHT4,
    GL_LIGHT5,
    GL_LIGHT6,
    GL_LIGHT7
};
static bool lights_taken[] = {
    false,
    false,
    false,
    false,
    false,
    false,
    false,
    false
};

LightSource::LightSource(Primitive* wrapped, float intensity) : Primitive(){
    this->wrapped = wrapped;
    this->intensity = intensity;
    for(int i = 0;i < 8 ; ++i){
        if( lights_taken[i] == false){
            lights_taken[i] = true;
            this->light_num = i;
            break;
        }
    }

    _is_area_light_source = false;
}

LightSource::LightSource(const LightSource& other): Primitive(other)
{
    intensity = other.intensity;
    light_num = other.light_num;
    wrapped = other.wrapped->clone();    

    _sample_points = other._sample_points;
    _is_area_light_source = other._is_area_light_source;
}


LightSource::~LightSource(){
    delete wrapped;
    lights_taken[light_num] = false;
    glDisable(light_enums[light_num]);

    _sample_points.clear();
}

jVec3 LightSource::getDirection(jVec3& hitPoint){
    return (getOrigin() - hitPoint);
}

jVec3 LightSource::getEmmitance(){
    return this->intensity*this->wrapped->material.color;
}

void LightSource::draw(jMat4& transform){

    glEnable(light_enums[light_num]);
    jVec3 v2;
    GLfloat vec[4];
    vec[3] = 1.0f;

    glPushMatrix();
    glLoadIdentity();
    jVec3 pos3 = getOrigin()*transform;
    pos3.toOpenGLFormat(vec);
    glLightfv(light_enums[light_num],GL_POSITION,vec);
    glPopMatrix();

    v2 = this->material.ambient.outerProduct(this->material.color);
    v2.toOpenGLFormat(vec);
    this->material.ambient.toOpenGLFormat(vec);
    glLightfv(light_enums[light_num],GL_AMBIENT,vec);

    v2 = this->material.diffuse.outerProduct(this->material.color);
    v2.toOpenGLFormat(vec);
    this->material.diffuse.toOpenGLFormat(vec);
    glLightfv(light_enums[light_num],GL_DIFFUSE,vec);

    v2 = this->material.specular.outerProduct(this->material.color);
    v2.toOpenGLFormat(vec);
    this->material.specular.toOpenGLFormat(vec);
    glLightfv(light_enums[light_num],GL_SPECULAR,vec);

    this->wrapped->material = this->material;
    this->wrapped->draw(transform);
}

bool LightSource::intersects(Ray& ray,HitRecord& rs, jMat4& transform){
    return this->wrapped->intersects(ray,rs,transform);
    //return this->wrapped->intersects(ray,rs,transform);
}
jVec3 LightSource::getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit){
    return this->wrapped->getNormal(hitPoint,transform,hit);
    //return this->wrapped->getNormal(hitPoint,transform,hit);
}

jVec3 LightSource::getOrigin(){
    return this->wrapped->getOrigin();
}
void LightSource::flatten(jMat4& transform){
    this->isFlat = true;
    this->wrapped->flatten(transform);

    if( isAreaLightSource()){
        // flatten the sample points as well
        for(int i = 0; i < (int) _sample_points.size(); ++i){
            _sample_points[i] = _sample_points[i]*transform;
        }
    }
}

LightSource* LightSource::clone() const{
    return new LightSource(*this);
}

bool LightSource::isDirectional(){
    return true;
}


// ----------------
// Area Light Source stuff...
// ----------------
bool LightSource::isAreaLightSource(){
    return _is_area_light_source;
}
void LightSource::setIsAreaLightSource(bool value){
    _is_area_light_source =value;
}

void LightSource::generateSamples(int rows,int cols){
    // HACK! Hard-coded to thinking that the shape is a plane
    // if it is an area light.
    // TODO: Make this general, so that we can use an arbitrary trimesh as a light
    jRand& jrand = jRand::getInstance();
    PrimitiveTriMesh* m = (PrimitiveTriMesh*)this->wrapped;
    jVec3 v;
    jVec3 c = m->vertex_pool[0];
    jVec3 a = (m->vertex_pool[1] - c)/cols;
    jVec3 b = (m->vertex_pool[3] - c)/rows;
    _sample_points.clear();
    for(float row = 0; row < rows; ++row){
        for(float col = 0; col < cols; ++col){
            v = c + (col+jrand())*a + (row+jrand())*b;
            _sample_points.push_back(v);
        }
    }
}

jVec3 LightSource::getSamplePoint(int i){
    return _sample_points[i];
}


