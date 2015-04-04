#include "SceneNode.h"
#include "RayTracer/HitRecord.h"
#include <QDebug>


SceneNode::SceneNode(){
    sceneObject = NULL;
    parent = NULL;
    children.clear();
    localTransform.toidentity();
}

SceneNode::SceneNode(const SceneNode& other):
    localTransform(other.localTransform),
    parent(other.parent)
{
    if(other.sceneObject != NULL){
        sceneObject = other.sceneObject->clone();
    }else{
        sceneObject = NULL;
    }

    children.resize(other.children.size());
    for(int i = 0;i < (int)children.size();++i){
        // create a copy of the new sceneNode
        children[i] = new SceneNode(*(other.children[i]));
        children[i]->parent = this;
    }
}

SceneNode::~SceneNode(){
    delete sceneObject;
    clear();
    parent = NULL;
}


void SceneNode::draw(jMat4& transform)
{
    jMat4 candTransform = localTransform*transform;
    if( sceneObject != NULL){
        sceneObject->draw(candTransform);
    }

    for(int i = 0;i < (int) children.size(); ++i)
    {
        children[i]->draw(candTransform);
    }
}

void SceneNode::addChild(SceneNode* child){
    child->parent = this;
    children.push_back(child);
}

void SceneNode::clear(){
    for(int i = 0;i < (int)children.size(); ++i){
        delete children[i];
    }
    children.clear();
}


void printMatrix(jMat4& mat){
    for (int row = 0; row < 4; ++row){
        for(int col = 0; col < 4; ++col){
            qDebug() <<  mat.ij(row,col) << " ";
        }
        qDebug() << "\n";
    }
}

HitRecord SceneNode::queryScene(Ray& ray, float min_dist, float max_dist){
    HitRecord hit;
    hit.min_dist = min_dist;
    hit.max_dist = max_dist;
    hit.hit = false;

    queryScene(ray,hit,localTransform);
    return hit;
}

void SceneNode::queryScene(Ray& ray, HitRecord& hitRecord, jMat4& transform){
    HitRecord candHitRecord;
    candHitRecord.min_dist = hitRecord.min_dist;
    candHitRecord.max_dist = hitRecord.max_dist;
    candHitRecord.hit = false;
    jMat4 candTransform = localTransform*transform;

    if(sceneObject != NULL)
    {
        sceneObject->intersects(ray,candHitRecord,candTransform);
        if( candHitRecord.hit)
        {
            candHitRecord.hitObject = sceneObject;
            candHitRecord.transform = candTransform;

            // record the result out into the hitRecord
            hitRecord = candHitRecord;
        }
    }

    for(int i = 0; i < (int)children.size(); ++i)
    {
        children[i]->queryScene(ray,hitRecord,candTransform);
    }

    // the result of this function, is the side-effect in the hitRecord
}

void SceneNode::flatten(jMat4& transform)
{
    // jMat4 localTransform;
    // Primitive* sceneObject;
    // std::vector<SceneNode*> children;
    // SceneNode* parent;

    jMat4 candTransform = localTransform*transform;
    if( sceneObject != NULL){
        sceneObject->flatten(candTransform);
    }

    for(int i = 0;i < (int)children.size(); ++i)
    {
        children[i]->flatten(candTransform);
    }
}


SceneNode* SceneNode::clone() const{
    return new SceneNode(*this);
}
