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
    jVec3 trace(Ray& ray, float refractionIndex,int depth);
    jVec3 shade(Ray& ray, HitRecord& hit,float refractionIndex,int depth);

protected:
    SceneNode* scene;
    std::vector<LightSource*>* lights;
    QImage* canvas;
    Camera* camera;

    int max_depth;
    float defaultRefractionIndex;
    float min_dist;
    float max_dist;
    const float epsilon;

    jVec3 refract(Ray& ray,HitRecord& hitRecord,Material& material,jVec3& hitPoint,
        jVec3& normal, float refractionIndex,int depth);
    float getSchlickApproximation(float refractionIndex,float cos_theta);
};

#endif
