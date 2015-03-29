#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <QRgb>
#include "Utils/linearalgebra.h"

class Material{
public:
  Material();
  Material(jVec3 color,jVec3 ambient,jVec3 diffuse, jVec3 specular,
      float shininess,float reflection,float refraction,float refractionIndex);
  virtual ~Material();

  jVec3 color;
  jVec3 ambient;
  jVec3 diffuse;
  jVec3 specular;

  float shininess;
  float reflection;
  // jVec3 reflection
  float refraction;
  float refractionIndex;

  void set(jVec3 color,jVec3 ambient,jVec3 diffuse, jVec3 specular,
    float shininess,float reflection,float refraction,float refractionIndex);
  QRgb colorToQrgb();
};


#endif
