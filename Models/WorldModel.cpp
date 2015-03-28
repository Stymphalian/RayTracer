#include <queue>
#include "WorldModel.h"
#include "PrimitiveSphere.h"
#include "Materials/MaterialFactory.h"

WorldModel::WorldModel(){
    // setup the camera
    camera.pos = jVec3(9.85341,21.2548,23.5946);
    camera.at = jVec3(0,0,0);
    camera.dir = (camera.at - camera.pos).normalize();
    camera.focalLength = 1500;

    root = NULL;
    setupWorld();
    setupLights();
}

WorldModel::~WorldModel(){
    delete root;
    lights.clear();
}

void WorldModel::setupWorld()
{
    MaterialFactory matFact;
    root = new SceneNode();
    root->sceneObject = NULL;

    SceneNode *n = new SceneNode();
    n->sceneObject = new PrimitiveSphere(jVec3(0,0,0),1);
    n->sceneObject->material = matFact.get(MaterialFactory::WOOD);
    n->localTransform.toidentity();
    n->localTransform.scale(2,2,2);
    n->localTransform.translate(0,0,0);
    root->addChild(n);

    n = new SceneNode();
    n->sceneObject = new PrimitiveSphere(jVec3(0,0,0),1);
    n->sceneObject->material = matFact.get(MaterialFactory::PINE);
    n->localTransform.toidentity();
    // n->localTransform.scale(2,2,2);
    n->localTransform.translate(3,3,2.2);
    root->addChild(n);

    // Creating a light
    n = new SceneNode();
    Primitive* p = new PrimitiveSphere(jVec3(0,0,0),0.1);
    n->sceneObject = new LightSource(p,1.0f);
    n->sceneObject->isLight = true;
    n->sceneObject->material = matFact.get(MaterialFactory::WHITE_LIGHT);
    n->localTransform.translate(-10,20,5);
    root->addChild(n);

    n = new SceneNode();
    p = new PrimitiveSphere(jVec3(0,0,0),0.1);
    n->sceneObject = new LightSource(p,1.0f);
    n->sceneObject->isLight = true;
    n->sceneObject->material = matFact.get(MaterialFactory::WHITE_LIGHT);
    n->localTransform.translate(8,8,5);
    root->addChild(n);
}

void WorldModel::_setupLights(std::vector<LightSource*>& lights, SceneNode* node,jMat4& transform)
{
    jMat4 candTransform = node->localTransform*transform;

    if(node->sceneObject != NULL && node->sceneObject->isLight){
        LightSource* light = (LightSource*) node->sceneObject;
        light->local_transform = node->localTransform;
        light->parent_transform = transform;
        light->position = jVec3(0,0,0)*(candTransform);
        lights.push_back(light);
    }

    for(int i = 0;i < (int)node->children.size(); ++i){
        _setupLights(lights,node->children[i],candTransform);
    }
}

// travers the scenne graphs and extract out the lights.
void WorldModel::setupLights()
{
    jMat4 mat;
    mat.toidentity();
    lights.clear();
    _setupLights(lights,root,mat);

    // LightSource* light = new LightSource();
    // light->position = jVec3(0,0,0);
    // light->shape = new PrimitiveSphere(jVec3(0,0,0),1);
    // light->intensity = 1.0f;
    // lights.push_back(light);
}
