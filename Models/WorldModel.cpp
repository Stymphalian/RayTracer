#include "WorldModel.h"
#include "PrimitiveSphere.h"

WorldModel::WorldModel(){
    // setup the camera
    camera.pos = jVec3(0,0,-5);
    camera.at = jVec3(0,0,0);
    camera.dir = (camera.at - camera.pos).normalize();
    camera.focalLength = 1500;

    root = NULL;
    setupWorld();
    setupLights();
}

WorldModel::~WorldModel(){
    delete root;
    for(int i = 0;i < lights.size(); ++i){
        delete lights[i];
    }
    lights.clear();
}

void WorldModel::setupWorld()
{
    root = new SceneNode();
    root->sceneObject = NULL;

    SceneNode *n = new SceneNode();
    n->sceneObject = new PrimitiveSphere(jVec3(0,0,0),1);
    n->sceneObject->material.color = jVec3(180,0,0);
    n->sceneObject->material.diffuse = 0.1;
    n->localTransform.toidentity();
    // n->localTransform.scale(5,5,5,1);
    n->localTransform.translate(0,0,5,0);
    root->addChild(n);

    n = new SceneNode();
    n->sceneObject = new PrimitiveSphere(jVec3(0,0,0),1);
    n->sceneObject->material.color = jVec3(0,180,0);
    n->sceneObject->material.diffuse = 0.5;
    n->localTransform.toidentity();
    // n->localTransform.scale(5,5,5,1);
    n->localTransform.translate(-1.5,0,10,0);
    root->addChild(n);
}

void WorldModel::setupLights()
{
    LightSource* light = new LightSource();
    light->position = jVec3(0,0,0);
    light->shape = new PrimitiveSphere(jVec3(0,0,0),1);
    light->intensity = 1.0f;
    lights.push_back(light);
}
