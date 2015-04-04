#include <QObject>
#include <QImage>
#include <cassert>
#include "RayTracer.h"
#include "HitRecord.h"
#include "Models/SceneNode.h"
#include "Models/LightSource.h"
#include "Utils/linearalgebra.h"
#include "../glwidget.h"

RayTracer::RayTracer(): QObject(0),epsilon(0.001f){
    min_dist= 0.001f;
    max_dist = 2000.0f;
    max_depth = 3;
    defaultRefractionIndex = 1.0f;
}
RayTracer::~RayTracer(){
    qDebug() << "destroying ray tracer " << thread()->currentThreadId();
}

// void RayTracer::render(QImage& ca,SceneNode& s,std::vector<LightSource*>& l,Camera& cam){
void RayTracer::render(int id,QImage* ca,WorldModel* model,int start_row,int end_row)
{
    if(this->id != id){return;}

    this->scene = model->root;;
    this->lights = &model->lights;
    this->camera = &model->camera;
    this->canvas = ca;

    this->start_row = start_row;
    this->end_row = end_row;

    Ray ray;
    int renderWidth = canvas->width();
    int renderHeight = canvas->height();
    jVec3 pixelColor(0,0,0);
    ray.calcUVW((camera->at - camera->pos),jVec3(0,1,0));

// loop through each pixel
    for (int row = start_row; row <= end_row; ++row){
        for(int col = 0; col < renderWidth; ++col){
            ray.calcRay(renderWidth,renderHeight,col,renderHeight - row,camera->pos,camera->focalLength);
            ray.dir.normalize();
            pixelColor = trace(ray,defaultRefractionIndex,max_depth);

            // set the pixel color
            canvas->setPixel(col,row,qRgb(pixelColor[0]*255,pixelColor[1]*255,pixelColor[2]*255));

            // update the progress bar
            //widget->_updateProgress((row*renderHeight + col)/(renderHeight*renderWidth));
            //widget->_updateProgress(100*((float)(row*renderHeight + col)/(renderHeight*renderWidth)));
        }

        // update the progress bar
        emit render_row_finished();
    }

    emit finished();
    // thread()->exit();
}

jVec3 RayTracer::trace(Ray& ray,float refractionIndex,int depth){
    jVec3 pixelColor(0,0,0);
    jVec3 backgroundColor(0,0,0);
    HitRecord hitRecord;

    if(depth <= 0){
        // TODO: return the last valid color
        return backgroundColor;
    }

    hitRecord = scene->queryScene(ray,min_dist,max_dist);
    if(hitRecord.hit == false){
        pixelColor += backgroundColor;
    }else{
        if( hitRecord.hitObject->isLight){
            pixelColor += hitRecord.hitObject->material.color;
        }else{
            // compute the colour based on materials and lights
            pixelColor += shade(ray,hitRecord,refractionIndex,depth);
        }

    }

    return pixelColor;
}

jVec3 RayTracer::shade(Ray& ray,HitRecord& hit,float refractionIndex,int depth){
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

        jVec3 matcolor = material.color.outerProduct(light->getEmmitance());

        // ambient  = I * K
        // pixelColor += light->intensity*material.color.outerProduct(material.ambient);
        pixelColor += light->intensity*matcolor.outerProduct(material.ambient);

        // Check if we are in shadow
        Ray new_ray;
        new_ray.origin = hitPoint;
        new_ray.dir = light_ray;
        HitRecord hitRecord = scene->queryScene(new_ray,min_dist + 0.01f,max_dist);
        if( hitRecord.hit == false || hitRecord.hitObject == light){
            // diffuse = I * K * (N*L)
            // pixelColor += light->intensity*material.color.outerProduct(material.diffuse)*fmax(light_ray*surfaceNormal,0);
            pixelColor += light->intensity*matcolor.outerProduct(material.diffuse)*fmax(light_ray*surfaceNormal,0);

            // specular =  I * K * (N * R)^p
            float spec = pow(fmax(0,viewVector*r),material.shininess);
            // pixelColor += light->intensity*material.color.outerProduct(material.specular)*spec;
            pixelColor += light->intensity*matcolor.outerProduct(material.specular)*spec;

            //jVec3 h = (viewVector + light_ray).normalize();
            // float spec = pow(fmax(0,h*surfaceNormal),material.shininess);
            // pixelColor += light->intensity*material.color.outerProduct(material.specular)*spec;
        }

    }

    // reflection
    jVec3 reflectColor(0,0,0);
    if( material.reflection > 0.0f){
        Ray reflectRay = ray.reflect(hitPoint,surfaceNormal);
        reflectColor += material.reflection*trace(reflectRay,refractionIndex,depth-1);
    }

    // refraction
    jVec3 refractColor(0,0,0);
    // if( material.refraction){
    //     refractColor += refract(ray,hit,material,hitPoint,surfaceNormal,refractionIndex,depth);
    // }

    pixelColor += reflectColor;
    pixelColor += refractColor;

    pixelColor[0]=fmin(pixelColor[0],1);
    pixelColor[1]=fmin(pixelColor[1],1);
    pixelColor[2]=fmin(pixelColor[2],1);
    return pixelColor;
}

jVec3 RayTracer::refract(Ray& ray,HitRecord& hitRecord,Material& material,jVec3& hitPoint,
        jVec3& normal, float refractionIndex,int depth)
{
    if( material.refraction <= 0.0f ){
        return jVec3(0,0,0);
    }

    Ray refract_ray;
    float cos_theta;
    jVec3 transparency(0,0,0);
    float new_refraction_index = 0.0;

    if( ray.dir*normal < 0.0f){
        // outside
        bool rs = ray.refract(hitPoint,ray.dir, normal,refractionIndex,
            material.refractionIndex,&refract_ray);
        assert(rs == true);
        cos_theta = -ray.dir*normal;
        transparency = jVec3(1,1,1);
        new_refraction_index = material.refractionIndex;

    }else{
        // inside
        float dist = (ray.origin - hitPoint).length();
        transparency[0] = exp(material.refractionAttenuation[0]*dist);
        transparency[1] = exp(material.refractionAttenuation[1]*dist);
        transparency[2] = exp(material.refractionAttenuation[2]*dist);

        // FUCKING HACKED,the outgoing refaraction index
        bool rs = ray.refract(hitPoint,ray.dir, normal,refractionIndex,1.0f,&refract_ray);
        if( rs ){
            cos_theta =  refract_ray.dir*normal;
            new_refraction_index = 1.0f;
        }else{
            // total internal reflection
            return transparency*trace(ray,1.0f,depth-1);
        }
    }

    float R = getSchlickApproximation(refractionIndex,cos_theta);
    return transparency*(1-R)*trace(refract_ray,new_refraction_index,depth-1);
}

float RayTracer::getSchlickApproximation(float refractionIndex,float cos_theta){
    float R0 = (refractionIndex - 1)*(refractionIndex - 1)/(refractionIndex+1)*(refractionIndex+1);
    float power = (1-cos_theta)*(1-cos_theta);
    power *= power;
    power *= (1-cos_theta);
    return R0 + (1-R0)*power;
}

