#ifndef _PRIMITIVE_TRI_MESH_H_
#define _PRIMITIVE_TRI_MESH_H_

#include <vector>
#include "Primitive.h"
#include "../Utils/linearalgebra.h"
#include "../RayTracer/HitRecord.h"
#include "../Utils/ObjFileReader.h"
#include "AABB.h"

class PrimitiveTriMesh : public Primitive
{
public:
    std::vector<jVec3> vertex_pool;
    std::vector<jVec3> texture_pool;
    std::vector<jVec3> normal_pool;
    std::vector<jVec3> vertex_indices;
    std::vector<jVec3> texture_indices;
    std::vector<jVec3> normal_indices;
    AABB boundingbox;
    bool has_bounding_box;

    PrimitiveTriMesh();
    PrimitiveTriMesh(const PrimitiveTriMesh& other);
    virtual ~PrimitiveTriMesh();

    virtual void draw(jMat4& transform);
    virtual bool intersects(Ray& ray,HitRecord& rs, jMat4& transform);
    virtual jVec3 getNormal(jVec3& hitPoint,jMat4& transform,HitRecord hit);
    // virtual jVec3 getSamplePoint(int index, int rows,int cols,int depth);
    virtual jVec3 getOrigin();
    virtual void flatten(jMat4& transform);
    virtual PrimitiveTriMesh* clone() const;


    virtual bool hasBoundingBox();
    virtual bool intersectsBoundingBox(Ray& ray);

    void fillTriMeshFromObjFile(ObjFileReader::Obj_Model& model);

protected:
    bool  _intersects(Ray& ray,HitRecord& rs, jMat4& transform,int index);
    jVec3 _getNormal(jMat4& transform,int hitIndex);
    AABB _getBoundingBox();
};

#endif
