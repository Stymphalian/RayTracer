#ifndef _RAY_TRACER_H_
#define _RAY_TRACER_H_

#include <vector>
#include <QImage>
#include "Ray.h"
#include "Models/SceneNode.h"
#include "Models/Camera.h"
#include "Models/WorldModel.h"
#include "RayTracer/HitRecord.h"

class Primitive;
class LightSource;


class RayTracer
{
public:
    RayTracer();
    virtual ~RayTracer();

    void render(QImage& canvas,WorldModel& model);
    // void render(QImage& canvas,SceneNode& scene,std::vector<LightSource*>& lights,Camera& cam);
    jVec3 computePixelColor(Ray& ray, HitRecord& hit);

protected:
    SceneNode* scene;
    std::vector<LightSource*>* lights;
    QImage* canvas;
    Camera* camera;
};

#endif
