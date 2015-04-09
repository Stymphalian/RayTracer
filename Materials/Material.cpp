#include <QRgb>
#include "Utils/linearalgebra.h"
#include "Material.h"

Material::Material(jVec3 color,jVec3 ambient,jVec3 diffuse, jVec3 specular,
    float shininess,float reflection,
    float refraction,float refractionIndex,jVec3 refractionAttenuation)
{
    set(color,ambient,diffuse,specular,shininess,reflection,
        refraction,refractionIndex,refractionAttenuation);
}

Material::Material(){
    color= jVec3(0,0,0);
    ambient = jVec3(0,0,0);
    diffuse = jVec3(0,0,0);
    specular = jVec3(0,0,0);

    shininess = 0.0;
    reflection = 0.0;
    refraction = 0.0;
    refractionIndex = 0.0;
    refractionAttenuation = jVec3(0,0,0);
}
Material::~Material(){}

QRgb Material::colorToQrgb(){
    return qRgb(color[0],color[1],color[2]);
}

void Material::set(jVec3 color,jVec3 ambient,jVec3 diffuse, jVec3 specular,
    float shininess,float reflection,
    float refraction,float refractionIndex,jVec3 refractionAttenuation)
{
    this->color = color;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->shininess = shininess;
    this->reflection = reflection;
    this->refraction = refraction;
    this->refractionIndex = refractionIndex;
    this->refractionAttenuation = refractionAttenuation;
}

bool Material::isTransparent(){
    return (refraction > 0.0f);
}