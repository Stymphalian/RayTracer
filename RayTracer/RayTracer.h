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
class GLWidget;


class RayTracer
{
public:
    RayTracer();
    virtual ~RayTracer();

    void render(QImage& canvas,WorldModel& model,GLWidget* widget);
    // void render(QImage& canvas,SceneNode& scene,std::vector<LightSource*>& lights,Camera& cam);

    // actual calls that do the work
    jVec3 trace(Ray& ray, int depth);
    jVec3 shade(Ray& ray, HitRecord& hit);

protected:
    SceneNode* scene;
    std::vector<LightSource*>* lights;
    QImage* canvas;
    Camera* camera;
};

#endif
