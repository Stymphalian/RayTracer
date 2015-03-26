#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <QRgb>
#include "Utils/linearalgebra.h"

class Material{
public:
  Material();
  virtual ~Material();

  jVec3 color;
  float reflection;
  float refraction;
  float refractionIndex;
  float diffuse;
  float specular;

  QRgb colorToQrgb();
};


#endif
