#ifndef _PRIMITIVE_SPHERE_H_
#define _PRIMITIVE_SPHERE_H_

#include <vector>
#include "Primitive.h"
#include "PrimitiveTriMesh.h"
#include "RayTracer/HitRecord.h"
#include "Utils/linearalgebra.h"

// sphere object
class PrimitiveSphere : public PrimitiveTriMesh {
public:
  jVec3 pos;
  double radius;
  jVec3 radius_vector;
  int num_lats;
  int num_longs;

  PrimitiveSphere(jVec3 pos, double radius);
  PrimitiveSphere(const PrimitiveSphere& other);
  virtual ~PrimitiveSphere();

  virtual bool intersects(Ray& ray,HitRecord& rs, jMat4& transform);
  virtual jVec3 getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit);
  virtual jVec3 getOrigin();
  virtual void flatten(jMat4& transform);
  virtual PrimitiveSphere* clone() const;
};

#endif
