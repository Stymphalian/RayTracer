#ifndef _RAY_TRACER_H_
#define _RAY_TRACER_H_

#include <vector>
#include <QObject>
#include <QImage>


#include "Ray.h"
#include "Models/SceneNode.h"
#include "Models/Camera.h"
#include "Models/WorldModel.h"
#include "RayTracer/HitRecord.h"

class Primitive;
class LightSource;
class GLWidget;

class RayTracer : public QObject
{
    Q_OBJECT
public:
    explicit RayTracer();
    virtual ~RayTracer();

    int id;

    // actual calls that do the work
    jVec3 trace(Ray& ray,int depth);
    jVec3 shade(Ray& ray, HitRecord& hit,int depth);

public slots:
    void render(int id,QImage* canvas,WorldModel* model,int start_row,int end_row);

signals:
    void render_row_finished();
    void finished();

protected:
    SceneNode* scene;
    std::vector<LightSource*>* lights;
    QImage* canvas;
    Camera* camera;
    RayTracerConfig* config;
    int start_row;
    int end_row;


    jVec3 reflectRefract(Ray& ray, HitRecord& hitRecord,int depth,
            jVec3& surfaceNormal,Material& hitMaterial,jVec3& hitPoint);
    float getSchlickApproximation(float refractionIndex,float cos_theta);
    bool isInShadow(HitRecord& hit,LightSource& light,Ray& ray);
};

#endif
