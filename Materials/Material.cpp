#include <QRgb>
#include "Utils/linearalgebra.h"
#include "Material.h"

Material::Material(){
    color= jVec3(0,0,0);
    diffuse = 0.1;
    specular = 0.1;
    reflection = 0.1;
    refraction = 0.1;
    refractionIndex = 0.1;
}
Material::~Material(){}

QRgb Material::colorToQrgb(){
    return qRgb(color[0],color[1],color[2]);
}
