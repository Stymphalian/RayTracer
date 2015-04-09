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

WorldModel::WorldModel(const WorldModel& other) :
    camera(other.camera)
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

void WorldModel::setupWorld()
{
    MaterialFactory& matFact = MaterialFactory::getInstance();
    root = new SceneNode();
    root->sceneObject = NULL;
    SceneNode* n;

        // n = new SceneNode();
        // n->sceneObject = new PrimitiveSphere(jVec3(0,0,0),1);
        // n->sceneObject->material = matFact.get("WOOD");
        // // n->sceneObject->material = matFact.get(MaterialFactory::GLASS);
        // n->localTransform.toidentity();
        // n->localTransform.scale(2,2,2);
        // n->localTransform.translate(0,0,0);
        // root->addChild(n);

        // n = new SceneNode();
        // n->sceneObject = new PrimitiveSphere(jVec3(0,0,0),1);
        // n->sceneObject->material = matFact.get("PINE");
        // n->localTransform.toidentity();
        // // n->localTransform.scale(2,2,2);
        // // n->localTransform.translate(3,3,2.2);
        // n->localTransform.translate(3,3,1.2);
        // root->addChild(n);

        // n = new SceneNode();
        // n->sceneObject = new PrimitiveSphere(jVec3(0,0,0),1);
        // n->sceneObject->material = matFact.get("PINE");
        // n->localTransform.toidentity();
        // // n->localTransform.scale(2,2,2);
        // // n->localTransform.translate(3,3,2.2);
        // n->localTransform.translate(-1,0,-3);
        // root->addChild(n);

        // n = new SceneNode();
        // n->sceneObject = new PrimitiveTriangle(jVec3(-2,1,0),jVec3(0,0,1),jVec3(0,0,-1));
        // n->sceneObject->material = matFact.get("PINE");
        // n->localTransform.scale(3,3,3);
        // n->localTransform.translate(0,-1,-3);
        // root->addChild(n);

        // n = new SceneNode();
        // n->sceneObject = createMesh();
        // n->sceneObject->material = matFact.get("SILVER");
        // n->localTransform.scale(3,3,3);
        // n->localTransform.translate(0,5,0);
        // root->addChild(n);

        // n = new SceneNode();
        // n->sceneObject = new PrimitiveCube(jVec3(-1,-1,1),jVec3(1,1,-1));
        // n->sceneObject->material = matFact.get("CHARCOAL");
        // n->localTransform.scale(0.5,0.5,0.5);
        // n->localTransform.translate(-1.5,8,0);
        // root->addChild(n);

        // n = new SceneNode();
        // n->sceneObject = new PrimitivePlane(jVec3(1,0,1),jVec3(1,0,-1),jVec3(-1,0,-1),jVec3(-1,0,1));
        // n->sceneObject->material = matFact.get("BRONZE");
        // n->localTransform.scale(10,10,10);
        // n->localTransform.translate(0,-2,0);
        // root->addChild(n);

        // // ObjFileReader reader;
        // // ObjFileReader::Obj_Model obj_model;
        // // // reader.read("icosahedron.obj",&obj_model);
        // // reader.read("data/lamp.obj",&obj_model);
        // // n = new SceneNode();
        // // PrimitiveTriMesh* mesh = new PrimitiveTriMesh();
        // // mesh->fillTriMeshFromObjFile(obj_model);
        // // n->sceneObject = mesh;
        // // n->sceneObject->material = matFact.get("WOOD");
        // // n->localTransform.scale(2,2,2);
        // // n->localTransform.translate(5,5,0);
        // // root->addChild(n);

        // // // Creating a light
        // n = new SceneNode();
        // Primitive* p = new PrimitiveSphere(jVec3(0,0,0),0.1);
        // n->sceneObject = new LightSource(p,1.0f);
        // n->sceneObject->isLight = true;
        // n->sceneObject->material = matFact.get("WHITELIGHT");
        // n->localTransform.translate(-10,10,-5);
        // root->addChild(n);

        // n = new SceneNode();
        // n->sceneObject = new LightSource(new PrimitiveSphere(jVec3(0,0,0),0.1),1.0f);
        // n->sceneObject->isLight = true;
        // n->sceneObject->material = matFact.get("WHITELIGHT");
        // //n->localTransform.translate(8,8,5);
        // n->localTransform.translate(9,9,6.6);
        // root->addChild(n);


            n = new SceneNode();
            // n->sceneObject = new PrimitiveCube(jVec3(-1,-1,1),jVec3(1,1,-1));
            n->sceneObject = new PrimitiveSphere(jVec3(0,0,0),1.0f);
            n->sceneObject->material = matFact.get("GLASS");
            n->localTransform.toidentity();
            n->localTransform.scale(2,2,2);
            n->localTransform.translate(0,3.1,0);
            root->addChild(n);

            n = new SceneNode();
            // n->sceneObject = new PrimitiveCube(jVec3(-1,-1,1),jVec3(1,1,-1));
            n->sceneObject = new PrimitiveSphere(jVec3(0,0,0),1.0f);
            n->sceneObject->material = matFact.get("GLASS");
            n->localTransform.toidentity();
            n->localTransform.scale(2,2,2);
            n->localTransform.translate(5,3.1,5);
            root->addChild(n);


            n = new SceneNode();
            n->sceneObject = new PrimitivePlane(jVec3(1,0,1),jVec3(1,0,-1),jVec3(-1,0,-1),jVec3(-1,0,1));
            n->sceneObject->material = matFact.get("MARBLE");
            n->localTransform.scale(10,10,10);
            n->localTransform.translate(0,-2,0);
            root->addChild(n);

            n = new SceneNode();
            n->sceneObject = new LightSource(new PrimitiveSphere(jVec3(0,0,0),0.1),1.0f);
            n->sceneObject->isLight = true;
            n->sceneObject->material = matFact.get("WHITELIGHT");
            n->localTransform.translate(0,10,2);
            root->addChild(n);
}


void WorldModel::_setupLights(std::vector<LightSource*>& lights, SceneNode* node)
{
    if(node->sceneObject != NULL && node->sceneObject->isLight){
        LightSource* light = (LightSource*) node->sceneObject;
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
    // lights
}

void WorldModel::reload(){
    delete root;
    root = NULL;
    lights.clear();

    setupWorld();
    setupLights();
}
