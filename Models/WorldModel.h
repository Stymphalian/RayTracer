#ifndef _WORLD_MODEL_H_
#define _WORLD_MODEL_H_

#include <vector>
#include "LightSource.h"
#include "Camera.h"
#include "SceneNode.h"

class WorldModel{
public:
    WorldModel();
    virtual ~WorldModel();

    // variables
    Camera camera;
    SceneNode* root;
    std::vector<LightSource*> lights;

protected:
    void setupWorld();
    void setupLights();
    void _setupLights(std::vector<LightSource*>& lights, SceneNode* node, jMat4& transform);
};

#endif
