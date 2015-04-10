#ifndef _WORLD_MODEL_H_
#define _WORLD_MODEL_H_

#include <vector>
#include "LightSource.h"
#include "Camera.h"
#include "SceneNode.h"
#include "../RayTracer/RayTracerConfig.h"

class WorldModel{
public:
    WorldModel();
    WorldModel(const WorldModel& other);
    virtual ~WorldModel();

    // variables
    Camera camera;
    RayTracerConfig config;
    SceneNode* root;
    std::vector<LightSource*> lights;

    void flatten();
    void reload();

protected:
    void setupWorld();
    void setupLights();
    void _setupLights(std::vector<LightSource*>& lights, SceneNode* node);

    SceneNode* load_main_scene();
    SceneNode* load_obj_scene();
};

#endif
