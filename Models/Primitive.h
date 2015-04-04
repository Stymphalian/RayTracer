#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

#include <QRgb>
#include "Utils/linearalgebra.h"
#include "Materials/Material.h"
#include "RayTracer/HitRecord.h"
class Ray;


// should be used in float comparison calculations
#define EPSILON 0.001f

//  base class for all scene objects
class Primitive {
public:
  Material material;
  bool isLight;

  Primitive();
  virtual ~Primitive();

  virtual void  draw(jMat4& transform) = 0;
  virtual bool  intersects(Ray& ray,HitRecord& rs, jMat4& transform) = 0;
  virtual jVec3 getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit) = 0;

  virtual bool hasBoundingBox();
  virtual bool intersectsBoundingBox(Ray& ray);

protected:
    bool very_close(float v,float v2, float e);
};



#endif
