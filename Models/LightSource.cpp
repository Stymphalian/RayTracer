#include "LightSource.h"
#include "Primitive.h"
#include "RayTracer/HitRecord.h"
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

    // isDirectional = true;
}

LightSource::LightSource(const LightSource& other): Primitive(other),
    intensity(other.intensity),
    light_num(other.light_num)
{
    wrapped = other.wrapped->clone();
    light_num = other.light_num;
    intensity = other.intensity;
    // isDirectional = other.isDirectional;
}


LightSource::~LightSource(){
    delete wrapped;
    lights_taken[light_num] = false;
    glDisable(light_enums[light_num]);
}

jVec3 LightSource::getDirection(jVec3& hitPoint){
    return (getOrigin() - hitPoint);
}

jVec3 LightSource::getEmmitance(){
    return this->intensity*this->wrapped->material.color;
}

void LightSource::draw(jMat4& transform){

    glEnable(light_enums[light_num]);
    GLfloat vec[4];
    vec[3] = 1.0f;

    glPushMatrix();
    glLoadIdentity();
    jVec3 pos3 = getOrigin()*transform;
    pos3.toOpenGLFormat(vec);
    glLightfv(light_enums[light_num],GL_POSITION,vec);
    glPopMatrix();

    this->material.ambient.toOpenGLFormat(vec);
    glLightfv(light_enums[light_num],GL_AMBIENT,vec);

    this->material.diffuse.toOpenGLFormat(vec);
    glLightfv(light_enums[light_num],GL_DIFFUSE,vec);

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
}

LightSource* LightSource::clone() const{
    return new LightSource(*this);
}

bool LightSource::isDirectional(){
    return true;
}

