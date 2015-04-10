#include <queue>
#include "RayTracer/Ray.h"
#include "RayTracer/HitRecord.h"
#include "WorldModel.h"
#include "PrimitiveSphere.h"
#include "PrimitiveTriangle.h"
#include "PrimitiveCube.h"
#include "PrimitiveTriMesh.h"
#include "PrimitivePlane.h"
#include "Materials/MaterialFactory.h"
#include "Utils/ObjFileReader.h"
#include "Utils/jRand.h"
#include "RayTracer/RayTracerConfig.h"

WorldModel::WorldModel(){
    jRand& jrand = jRand::getInstance();
    jrand.init(400);

    // setup the camera
    // camera.pos = jVec3(9.85341,21.2548,23.5946);
    camera.pos = jVec3(0.61947,20.9607,74.3494);
    // camera.pos = jVec3(365.294,222.424,319.676);

    camera.at = jVec3(0,0,0);
    camera.dir = (camera.at - camera.pos).normalize();
    camera.focalLength = 1500;

    // config

    root = NULL;
    setupWorld(0);
    setupLights();
}

WorldModel::WorldModel(const WorldModel& other) :
    camera(other.camera),
    config(other.config)
{
    root = new SceneNode(*other.root);
    setupLights();
}

WorldModel::~WorldModel(){
    delete root;
    lights.clear();
}

PrimitiveTriMesh* createMesh(){
    PrimitiveTriMesh* mesh = new PrimitiveTriMesh();

    mesh->vertex_pool.push_back(jVec3(0,0,1));
    mesh->vertex_pool.push_back(jVec3(1,0,1));
    mesh->vertex_pool.push_back(jVec3(1,0,0));
    mesh->vertex_pool.push_back(jVec3(1,0,-1));
    mesh->vertex_pool.push_back(jVec3(0,0,-1));
    mesh->vertex_pool.push_back(jVec3(-1,0,-1));
    mesh->vertex_pool.push_back(jVec3(-1,0,0));
    mesh->vertex_pool.push_back(jVec3(-1,0,1));
    mesh->vertex_indices.push_back(jVec3(0,1,2));
    mesh->vertex_indices.push_back(jVec3(2,3,4));
    mesh->vertex_indices.push_back(jVec3(4,5,6));
    mesh->vertex_indices.push_back(jVec3(6,7,0));

    mesh->has_bounding_box = false;
    return mesh;
}

PrimitiveTriMesh* createLightMesh(){
    PrimitiveTriMesh* mesh = new PrimitiveTriMesh();

    mesh->vertex_pool.push_back(jVec3(-1,0,-1)); // 0
    mesh->vertex_pool.push_back(jVec3(0,0,0)); // 1
    mesh->vertex_pool.push_back(jVec3(0,1,0)); // 2
    mesh->vertex_pool.push_back(jVec3(-1,1,-1)); //3
    // mesh->vertex_pool.push_back(jVec3(1,0,-1)); // 4
    // mesh->vertex_pool.push_back(jVec3(1,1,-1)); // 5

    mesh->vertex_indices.push_back(jVec3(0,1,3));
    mesh->vertex_indices.push_back(jVec3(1,2,3));

    // mesh->vertex_indices.push_back(jVec3(4,1,5));
    // mesh->vertex_indices.push_back(jVec3(1,2,5));

    mesh->has_bounding_box = false;
    return mesh;
}


SceneNode* WorldModel::load_main_simple_scene()
{
    MaterialFactory& matFact = MaterialFactory::getInstance();
    SceneNode* root = new SceneNode();
    root->sceneObject = NULL;
    SceneNode* n;

    n = new SceneNode();
    n->sceneObject = new PrimitiveSphere(jVec3(0,0,0),1.0f);
    n->sceneObject->material = matFact.get("BLUEGLASS");
    n->localTransform.toidentity();
    n->localTransform.scale(4,4,4);
    n->localTransform.translate(0,2,5);
    root->addChild(n);

    // bottom plane
    n = new SceneNode();
    n->sceneObject = new PrimitivePlane(jVec3(1,0,1),jVec3(1,0,-1),jVec3(-1,0,-1),jVec3(-1,0,1));
    n->sceneObject->material = matFact.get("MARBLE");
    n->localTransform.scale(10,10,20);
    n->localTransform.translate(0,-2,10);
    root->addChild(n);

    // left wall
    n = new SceneNode();
    n->sceneObject = new PrimitivePlane(jVec3(1,0,1),jVec3(1,0,-1),jVec3(-1,0,-1),jVec3(-1,0,1));
    n->sceneObject->material = matFact.get("REDMARBLE");
    n->localTransform.scale(10,10,20);
    n->localTransform.rotate(jVec3(0,0,1),90*pi/180);
    n->localTransform.translate(-10,8,10);
    root->addChild(n);

    // right wall
    n = new SceneNode();
    n->sceneObject = new PrimitivePlane(jVec3(1,0,1),jVec3(1,0,-1),jVec3(-1,0,-1),jVec3(-1,0,1));
    n->sceneObject->material = matFact.get("GREENMARBLE");
    n->localTransform.scale(10,10,20);
    n->localTransform.rotate(jVec3(0,0,1),-90*pi/180);
    n->localTransform.translate(10,8,10);
    root->addChild(n);

    // // back wall
    n = new SceneNode();
    n->sceneObject = new PrimitivePlane(jVec3(1,0,1),jVec3(1,0,-1),jVec3(-1,0,-1),jVec3(-1,0,1));
    n->sceneObject->material = matFact.get("BLUEMARBLE");
    n->localTransform.scale(10,10,10);
    n->localTransform.rotate(jVec3(1,0,0),90*pi/180);
    n->localTransform.translate(0,8,-10);
    root->addChild(n);

    // bottom plane
    n = new SceneNode();
    n->sceneObject = new PrimitivePlane(jVec3(1,0,1),jVec3(1,0,-1),jVec3(-1,0,-1),jVec3(-1,0,1));
    n->sceneObject->material = matFact.get("MARBLE");
    n->localTransform.scale(10,10,20);
    n->localTransform.translate(0,18,10);
    root->addChild(n);

    n = new SceneNode();
    n->sceneObject = new LightSource(new PrimitivePlane(jVec3(-2,-1,0),jVec3(1,-1,0),jVec3(2,1,0),jVec3(-1,1,0)),1.0f);
    ((LightSource*)n->sceneObject)->setIsAreaLightSource(true);
    n->sceneObject->isLight = true;
    n->sceneObject->material = matFact.get("WHITELIGHT");
    n->localTransform.rotate(jVec3(1,0,0),45*pi/180);
    n->localTransform.translate(0,15,-3);
    root->addChild(n);

    return root;
}

SceneNode* WorldModel::load_main_scene()
{
    MaterialFactory& matFact = MaterialFactory::getInstance();
    SceneNode* root = new SceneNode();
    root->sceneObject = NULL;
    SceneNode* n;

    // create three spheres
    n = new SceneNode();
    n->sceneObject = new PrimitiveSphere(jVec3(0,0,0),1.0f);
    // n->sceneObject->material = matFact.get("GLASS");
    n->sceneObject->material = matFact.get("REDGLASS");
    n->localTransform.toidentity();
    n->localTransform.scale(4.8,4.8,4.8);
    // n->localTransform.translate(0,3,15);
    n->localTransform.translate(0,4,-3.5);
    root->addChild(n);

    n = new SceneNode();
    n->sceneObject = new PrimitiveSphere(jVec3(0,0,0),1.0f);
    n->sceneObject->material = matFact.get("BLUEGLASS");
    n->localTransform.toidentity();
    n->localTransform.scale(3,3,3);
    // n->localTransform.translate(-7,1,-5);
    n->localTransform.translate(-7,1,15);
    // n->localTransform.translate(-5,1,-3);
    root->addChild(n);

    n = new SceneNode();
    n->sceneObject = new PrimitiveSphere(jVec3(0,0,0),1.0f);
    // n->sceneObject->material = matFact.get("GREENGLASS");
    n->sceneObject->material = matFact.get("WATER");
    n->localTransform.toidentity();
    n->localTransform.scale(5,5,5);
    // n->localTransform.translate(7,1,2);
    n->localTransform.translate(6,0.5,4.75);
    root->addChild(n);

    n = new SceneNode();
    n->sceneObject = new PrimitiveCube(jVec3(0,0,0),jVec3(1,1,-1));
    n->sceneObject->material = matFact.get("BRONZE");
    n->localTransform.toidentity();
    n->localTransform.scale(2,2,2);
    n->localTransform.translate(-5,0,35);
    root->addChild(n);

    // bottom plane
    n = new SceneNode();
    n->sceneObject = new PrimitivePlane(jVec3(1,0,1),jVec3(1,0,-1),jVec3(-1,0,-1),jVec3(-1,0,1));
    n->sceneObject->material = matFact.get("MARBLE");
    n->localTransform.scale(10,10,20);
    n->localTransform.translate(0,-2,10);
    // n->localTransform.scale(10,10,10);
    // n->localTransform.translate(0,-2,0);
    root->addChild(n);

    // left wall
    n = new SceneNode();
    n->sceneObject = new PrimitivePlane(jVec3(1,0,1),jVec3(1,0,-1),jVec3(-1,0,-1),jVec3(-1,0,1));
    n->sceneObject->material = matFact.get("REDMARBLE");
    n->localTransform.scale(10,10,20);
    n->localTransform.rotate(jVec3(0,0,1),90*pi/180);
    n->localTransform.translate(-10,8,10);
    root->addChild(n);

    // right wall
    n = new SceneNode();
    n->sceneObject = new PrimitivePlane(jVec3(1,0,1),jVec3(1,0,-1),jVec3(-1,0,-1),jVec3(-1,0,1));
    n->sceneObject->material = matFact.get("GREENMARBLE");
    n->localTransform.scale(10,10,20);
    n->localTransform.rotate(jVec3(0,0,1),-90*pi/180);
    n->localTransform.translate(10,8,10);
    root->addChild(n);

    // // back wall
    n = new SceneNode();
    n->sceneObject = new PrimitivePlane(jVec3(1,0,1),jVec3(1,0,-1),jVec3(-1,0,-1),jVec3(-1,0,1));
    n->sceneObject->material = matFact.get("BLUEMARBLE");
    n->localTransform.scale(10,10,10);
    n->localTransform.rotate(jVec3(1,0,0),90*pi/180);
    n->localTransform.translate(0,8,-10);
    root->addChild(n);

    // bottom plane
    n = new SceneNode();
    n->sceneObject = new PrimitivePlane(jVec3(1,0,1),jVec3(1,0,-1),jVec3(-1,0,-1),jVec3(-1,0,1));
    n->sceneObject->material = matFact.get("MARBLE");
    n->localTransform.scale(10,10,20);
    n->localTransform.translate(0,18,10);
    root->addChild(n);

    n = new SceneNode();
    n->sceneObject = new LightSource(new PrimitivePlane(jVec3(-2,-1,0),jVec3(1,-1,0),jVec3(2,1,0),jVec3(-1,1,0)),1.0f);
    ((LightSource*)n->sceneObject)->setIsAreaLightSource(true);
    n->sceneObject->isLight = true;
    n->sceneObject->material = matFact.get("WHITELIGHT");
    n->localTransform.rotate(jVec3(1,0,0),45*pi/180);
    n->localTransform.translate(0,15,-3);
    root->addChild(n);

    // n = new SceneNode();
    // LightSource* ls = new LightSource(new PrimitivePlane(jVec3(-1,-1,0),jVec3(1,-1,0),jVec3(1,1,0),jVec3(-1,1,0)),1.0f);
    // ls->setIsAreaLightSource(true);
    // ls->intensity = 0.44;
    // n->sceneObject = ls;
    // n->sceneObject->isLight = true;
    // n->sceneObject->material = matFact.get("WHITELIGHT");
    // n->localTransform.scale(0.4,0.4,0.4);
    // n->localTransform.rotate(jVec3(0,1,0),-120*pi/180);
    // n->localTransform.rotate(jVec3(0,0,1),-20*pi/180);
    // n->localTransform.translate(-12,18,38);
    // root->addChild(n);

    return root;
}

SceneNode* WorldModel::load_obj_scene()
{
    MaterialFactory& matFact = MaterialFactory::getInstance();
    SceneNode* root = new SceneNode();
    root->sceneObject = NULL;
    SceneNode* n;

    ObjFileReader reader;
    ObjFileReader::Obj_Model obj_model;
    reader.read("data/icosahedron.obj",&obj_model);
    n = new SceneNode();
    PrimitiveTriMesh* mesh = new PrimitiveTriMesh();
    mesh->fillTriMeshFromObjFile(obj_model);
    n->sceneObject = mesh;
    n->sceneObject->material = matFact.get("BLUEMARBLE");
    root->addChild(n);

    n = new SceneNode();
    mesh = new PrimitiveTriMesh();
    mesh->fillTriMeshFromObjFile(obj_model);
    n->sceneObject = mesh;
    n->sceneObject->material = matFact.get("PINE");
    n->localTransform.scale(2,2,2);
    n->localTransform.translate(5,2,0);
    root->addChild(n);

    n = new SceneNode();
    n->sceneObject = new PrimitivePlane(jVec3(1,0,1),jVec3(1,0,-1),jVec3(-1,0,-1),jVec3(-1,0,1));
    n->sceneObject->material = matFact.get("MARBLE");
    n->localTransform.scale(10,10,10);
    n->localTransform.translate(0,-2,0);
    root->addChild(n);


    // lights
    n = new SceneNode();
    n->sceneObject = new LightSource(new PrimitivePlane(jVec3(-1,-1,0),jVec3(1,-1,0),jVec3(1,1,0),jVec3(-1,1,0)),1.0f);
    ((LightSource*)n->sceneObject)->setIsAreaLightSource(true);
    n->sceneObject->isLight = true;
    n->sceneObject->material = matFact.get("WHITELIGHT");
    n->localTransform.rotate(jVec3(1,0,0),45*pi/180);
    n->localTransform.translate(100,50,60);
    root->addChild(n);

    n = new SceneNode();
    n->sceneObject = new LightSource(new PrimitivePlane(jVec3(-1,-1,0),jVec3(1,-1,0),jVec3(1,1,0),jVec3(-1,1,0)),1.0f);
    ((LightSource*)n->sceneObject)->setIsAreaLightSource(true);
    n->sceneObject->isLight = true;
    n->sceneObject->material = matFact.get("WHITELIGHT");
    n->localTransform.rotate(jVec3(1,0,0),45*pi/180);
    n->localTransform.translate(20,100,-60);
    root->addChild(n);

    return root;
}

void WorldModel::setupWorld(int sceneNum){
    switch(sceneNum){
        case(0): root = load_main_simple_scene(); break;
        case(1): root = load_main_scene(); break;
        case(2): root = load_obj_scene(); break;
    }
}


void WorldModel::_setupLights(std::vector<LightSource*>& lights, SceneNode* node)
{
    if(node->sceneObject != NULL && node->sceneObject->isLight){
        LightSource* light = (LightSource*) node->sceneObject;

        if(light->isAreaLightSource()){
            int rows = sqrt(config.numSoftShadowSamples);
            light->generateSamples(rows,rows);
        }

        lights.push_back(light);
    }

    for(int i = 0;i < (int)node->children.size(); ++i){
        _setupLights(lights,node->children[i]);
    }
}

// travers the scenne graphs and extract out the lights.
void WorldModel::setupLights()
{
    lights.clear();
    _setupLights(lights,root);
}


void WorldModel::flatten()
{
    // camera
    // SceneNodes
    jMat4 transform;
    transform.toidentity();
    root->flatten(transform);
}

void WorldModel::reload(int sceneNum){
    delete root;
    root = NULL;
    lights.clear();

    setupWorld(sceneNum);
    setupLights();
}
