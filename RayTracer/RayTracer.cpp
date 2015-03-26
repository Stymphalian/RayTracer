#include "RayTracer.h"
#include <QImage>
#include "HitRecord.h"
#include "Models/SceneNode.h"
#include "Models/LightSource.h"
#include "Utils/linearalgebra.h"

RayTracer::RayTracer(){}
RayTracer::~RayTracer(){}

// void RayTracer::render(QImage& ca,SceneNode& s,std::vector<LightSource*>& l,Camera& cam){
void RayTracer::render(QImage& ca,WorldModel& model)
{
    this->scene = model.root;;
    this->lights = &model.lights;
    this->camera = &model.camera;
    this->canvas = &ca;

    Ray ray;
    HitRecord hitRecord;
    int depth = 1;
    int renderWidth = canvas->width();
    int renderHeight = canvas->height();
    float min_dist= 0.0f;
    float max_dist = 1000.0f;
    jVec3 pixelColor(0,0,0);
    jVec3 backgroundColor(0,0,0);

    //double focalLength = fabs((camera->dir - camera->pos).length());
    ray.calcUVW((camera->at - camera->pos),jVec3(0,1,0));

    // loop through each pixel
    for (int row = 0; row < renderHeight; ++row){
        for(int col = 0; col < renderWidth; ++col){
            // reset the pixel color
            pixelColor = jVec3(0,0,0);

            // see if we hit an object.
            ray.calcRay(renderWidth,renderHeight,col,renderHeight - row,camera->pos,camera->focalLength);
            hitRecord = scene->queryScene(ray,min_dist,max_dist);


            if( hitRecord.hit){
                // compute the colour based on materials and lights
                pixelColor += computePixelColor(ray,hitRecord);
            }else{
                // background colour
                pixelColor += backgroundColor;
            }

            // set the pixel color
            canvas->setPixel(col,row,qRgb(pixelColor[0]*255,pixelColor[1]*255,pixelColor[2]*255));
        }
    }
}

jVec3 RayTracer::computePixelColor(Ray& ray,HitRecord& hit){
    // the hitRecord at this point contains
    // hit // dist // min_dist // max_dist // hitObject // transform

    // for each light source
    // determine if the light ray will intersect with the surface point
    // if it does, then calculate shade

    jVec3 hitPoint = ray.getPoint(hit.dist);
    jVec3 surfaceNormal = hit.hitObject->getNormal(hitPoint, hit.transform, hit);
    Material& material = hit.hitObject->material;

    jVec3 matColor = material.color/255;
    jVec3 pixelColor(0,0,0);

    std::vector<LightSource*>::iterator it;
    for(it = lights->begin(); it != lights->end(); ++it){
        LightSource* light = *it;
        jVec3 light_ray = light->getDirection(hitPoint).normalize();

        // diffuse -- I * K * (N*L)
        // pixelColor += (light->intensity*material.diffuse)*material.color*fmax(surfaceNormal*light_ray.normalize(),0);
        // pixelColor += (light->intensity*material.diffuse)*matColor*fmax(light_ray.normalize()*surfaceNormal,0);
        pixelColor += light->intensity*matColor*fmax(light_ray*surfaceNormal,0);
    }

    pixelColor[0]=fmin(pixelColor[0],1);
    pixelColor[1]=fmin(pixelColor[1],1);
    pixelColor[2]=fmin(pixelColor[2],1);
    return pixelColor;
}
