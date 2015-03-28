#include <QImage>
#include "RayTracer.h"
#include "HitRecord.h"
#include "Models/SceneNode.h"
#include "Models/LightSource.h"
#include "Utils/linearalgebra.h"
#include "../glwidget.h"

RayTracer::RayTracer():epsilon(0.001f){
    min_dist= 0.0f;
    max_dist = 2000.0f;
}

RayTracer::~RayTracer(){}

// void RayTracer::render(QImage& ca,SceneNode& s,std::vector<LightSource*>& l,Camera& cam){
void RayTracer::render(QImage& ca,WorldModel& model,GLWidget* widget)
{
    this->scene = model.root;;
    this->lights = &model.lights;
    this->camera = &model.camera;
    this->canvas = &ca;

    Ray ray;
    int depth = 1;
    int renderWidth = canvas->width();
    int renderHeight = canvas->height();
    jVec3 pixelColor(0,0,0);
    ray.calcUVW((camera->at - camera->pos),jVec3(0,1,0));


    //setup the progress bas
    widget->_updateMaxProgress(100);
    widget->_updateProgress(0);

// loop through each pixel
    for (int row = 0; row < renderHeight; ++row){
        for(int col = 0; col < renderWidth; ++col){
            ray.calcRay(renderWidth,renderHeight,col,renderHeight - row,camera->pos,camera->focalLength);
            ray.dir.normalize();
            pixelColor = trace(ray,depth);

            // set the pixel color
            canvas->setPixel(col,row,qRgb(pixelColor[0]*255,pixelColor[1]*255,pixelColor[2]*255));

            // update the progress bar
            //widget->_updateProgress((row*renderHeight + col)/(renderHeight*renderWidth));
            widget->_updateProgress(100*((float)(row*renderHeight + col)/(renderHeight*renderWidth)));
        }
    }

    widget->_updateProgress(100);
}

jVec3 RayTracer::trace(Ray& ray,int depth){
    jVec3 pixelColor(0,0,0);
    jVec3 backgroundColor(0,0,0);
    HitRecord hitRecord;

    if(depth <= 0){
        // TODO: return the last valid color
        return backgroundColor;
    }

    // reset the pixel color
    hitRecord = scene->queryScene(ray,min_dist,max_dist);

    if(hitRecord.hit == false){
        pixelColor += backgroundColor;
    }else{
        if( hitRecord.hitObject->isLight){
            pixelColor += hitRecord.hitObject->material.color;
        }else{
            // compute the colour based on materials and lights
            pixelColor += shade(ray,hitRecord,depth);

            // reflection

            // refraction
        }

    }

    return pixelColor;
}

jVec3 RayTracer::shade(Ray& ray,HitRecord& hit,int depth){
    // the hitRecord at this point contains
    // hit // dist // min_dist // max_dist // hitObject // transform

    // for each light source
    // determine if the light ray will intersect with the surface point
    // if it does, then calculate shade

    jVec3 hitPoint = ray.getPoint(hit.dist);
    jVec3 surfaceNormal = hit.hitObject->getNormal(hitPoint, hit.transform, hit);
    jVec3 viewVector = (ray.origin - hitPoint).normalize();

    Material& material = hit.hitObject->material;
    jVec3 pixelColor(0,0,0);

    std::vector<LightSource*>::iterator it;
    for(it = lights->begin(); it != lights->end(); ++it){
        LightSource* light = *it;
        jVec3 light_ray = light->getDirection(hitPoint).normalize();
        jVec3 r = 2*(light_ray*surfaceNormal)*surfaceNormal - light_ray;

        // mabient  = I * K
        pixelColor += light->intensity*material.color.outerProduct(material.ambient);


        // Check if we are in shadow
        Ray new_ray;
        new_ray.origin = hitPoint;
        new_ray.dir = light_ray;
        //qDebug("%f %f %f",v[0],v[1],v[2]);
        HitRecord hitRecord = scene->queryScene(new_ray,min_dist + 0.01f,max_dist);
        if( hitRecord.hit == false || hitRecord.hitObject == light){
            // diffuse = I * K * (N*L)
            pixelColor += light->intensity*material.color.outerProduct(material.diffuse)*fmax(light_ray*surfaceNormal,0);

            // specular =  I * K * (N * R)^p
            float spec = pow(fmax(0,viewVector*r),material.shininess);
            pixelColor += light->intensity*material.color.outerProduct(material.specular)*spec;

            //jVec3 h = (viewVector + light_ray).normalize();
            // float spec = pow(fmax(0,h*surfaceNormal),material.shininess);
            // pixelColor += light->intensity*material.color.outerProduct(material.specular)*spec;
        }

    }
    //qDebug("%f %f %f ", pixelColor[0],pixelColor[1],pixelColor[2]);

    pixelColor[0]=fmin(pixelColor[0],1);
    pixelColor[1]=fmin(pixelColor[1],1);
    pixelColor[2]=fmin(pixelColor[2],1);
    return pixelColor;
}
