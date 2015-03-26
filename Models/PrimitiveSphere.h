#ifndef _PRIMITIVE_SPHERE_H_
#define _PRIMITIVE_SPHERE_H_

#include <vector>
#include "Primitive.h"
#include "RayTracer/HitRecord.h"
#include "Utils/linearalgebra.h"

// sphere object
class PrimitiveSphere : public Primitive {
public:
  // sphere code
  jVec3 pos;
  double radius;

  PrimitiveSphere(jVec3 pos, double radius);
  virtual ~PrimitiveSphere();

  virtual void draw(jMat4& transform);
  virtual bool intersects(Ray& ray,HitRecord& rs, jMat4& transform);
  virtual jVec3 getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit);
  // virtual jVec3 getNormal(jVec3& v);
protected:
  std::vector<jVec3> vertices;
  //std::vector<GLfloat> transformed_verts;

};

#endif
