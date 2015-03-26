#ifndef _SCENE_NODE_H_
#define _SCENE_NODE_H_


#include <vector>
#include "Primitive.h"
#include "Utils/linearalgebra.h"
class Ray;
class HitRecord;

class SceneNode{
public:
    SceneNode();
    virtual ~SceneNode();

    // interface
    void draw(jMat4& transform);
    HitRecord queryScene(Ray& ray,float min_dist, float max_dist);
    void addChild(SceneNode* child);
    void clear();

    // variables
    jMat4 localTransform;
    Primitive* sceneObject;

    std::vector<SceneNode*> children;
    SceneNode* parent;
private:
    void queryScene(Ray& ray, HitRecord&  hitRecord, jMat4& transform);
};

#endif
