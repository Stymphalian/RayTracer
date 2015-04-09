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

    sum = 0;
    count = 0;
}
RayTracer::~RayTracer(){
    //qDebug() << "destroying ray tracer " << thread()->currentThreadId();
    qDebug() << sum << " " << count << " " << sum/count;
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
            ray.refractIndex= defaultRefractionIndex;
            pixelColor = trace(ray,max_depth);

            // set the pixel color
            canvas->setPixel(col,row,qRgb(pixelColor[0]*255,pixelColor[1]*255,pixelColor[2]*255));
        }

        // update the progress bar
        emit render_row_finished();
    }

    emit finished();
    // thread()->exit();
}

jVec3 RayTracer::trace(Ray& ray,int depth){
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
            pixelColor += shade(ray,hitRecord,depth);
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

        jVec3 matcolor = material.color.outerProduct(light->getEmmitance());

        // ambient  = I * K
        pixelColor += light->intensity*matcolor.outerProduct(material.ambient);

        // Check if we are in shadow
        Ray new_ray;
        new_ray.origin = hitPoint;
        new_ray.dir = light_ray;

        HitRecord hitRecord = scene->queryScene(new_ray,min_dist + 0.01f,max_dist);
        if( isInShadow(hitRecord,*light,new_ray) == false){
        //if( hitRecord.hit == false || hitRecord.hitObject == light){
            // diffuse = I * K * (N*L)
            pixelColor += light->intensity*matcolor.outerProduct(material.diffuse)*fmax(light_ray*surfaceNormal,0);

            // specular =  I * K * (N * R)^p
            float spec = pow(fmax(0,viewVector*r),material.shininess);
            pixelColor += light->intensity*matcolor.outerProduct(material.specular)*spec;
        }

    }

    // reflection + refraction
   jVec3 reflectRefractColor(0,0,0);
   if( material.reflection > 0.0f || material.refraction > 0.0f){
       reflectRefractColor = reflectRefract(ray,hit,depth,surfaceNormal,material,hitPoint);
   }
   pixelColor += reflectRefractColor;

    // reflection
    // jVec3 reflectColor(0,0,0);
    // if( material.reflection > 0.0f){
    //     Ray reflectRay = ray.reflect(hitPoint,surfaceNormal);
    //     reflectColor += material.reflection*trace(reflectRay,depth-1);
    // }
    // pixelColor += reflectColor;

    pixelColor[0]=fmin(pixelColor[0],1);
    pixelColor[1]=fmin(pixelColor[1],1);
    pixelColor[2]=fmin(pixelColor[2],1);
    return pixelColor;
}

jVec3 RayTracer::reflectRefract(Ray& ray, HitRecord& hitRecord,int depth,
    jVec3& surfaceNormal,Material& material,jVec3& hitPoint)
{
    jVec3 reflectColor(0,0,0);

    if( material.reflection > 0.0f){
        Ray reflectRay = ray.reflect(hitPoint, surfaceNormal);
        reflectColor += material.reflection*trace(reflectRay,depth-1);
    }

    if( material.refraction > 0.0f){
        jVec3 pixelColor;
        jVec3 atten(0,0,0);
        float cosTheta = 0;
        Ray refractRay;

        float dn = ray.dir*surfaceNormal;
        if( dn < 0){
            // inside to outside
            ray.refract(hitPoint,ray.dir,surfaceNormal,ray.refractIndex,1.01f,&refractRay);
            refractRay.refractIndex = 1.0f;
            cosTheta = -dn;
            atten.setValues(1,1,1);
        }else{
            // outside to inside
            float dist = fabs((hitPoint - ray.origin).length());
            atten[0] = exp(-material.refractionAttenuation[0]*dist);
            atten[1] = exp(-material.refractionAttenuation[1]*dist);
            atten[2] = exp(-material.refractionAttenuation[2]*dist);

            jVec3 neg_surfaceNormal = -surfaceNormal;
            // bool tir = ray.refract(hitPoint,ray.dir,neg_surfaceNormal,ray.refractIndex,material.refractionIndex,&refractRay);
            bool tir = ray.refract(hitPoint,ray.dir,neg_surfaceNormal,1.01f,material.refractionIndex,&refractRay);
            refractRay.refractIndex = material.refractionIndex;
            if(tir){
                // total internal reflection; therefore return only the reflected component
                return atten.outerProduct(reflectColor);
            }else{
                cosTheta = -refractRay.dir*surfaceNormal;
            }
        }

        float R = getSchlickApproximation(material.refractionIndex,cosTheta);
        jVec3 reflectWeight = R*reflectColor;
        jVec3 refractWeight = (1-R)*trace(refractRay,depth-1);
        pixelColor += atten.outerProduct(reflectWeight + refractWeight);
        return pixelColor;
    }else{
        return reflectColor;
    }
}


float RayTracer::getSchlickApproximation(float refractionIndex,float cos_theta){
    float R0 = (refractionIndex - 1)/(refractionIndex+1);
    R0 = R0*R0;
    float power = (1-cos_theta)*(1-cos_theta);
    power *= power;
    power *= (1-cos_theta);
    return R0 + (1-R0)*power;
}


bool RayTracer::isInShadow(HitRecord& hit,LightSource& light,Ray& ray)
{
    // didn't hit any object, not even the light...
    if(hit.hit == false){return true;}

    // hit an object, but it was the light.
    if(hit.hitObject == &light){
        if( light.isDirectional()){

            jVec3 hitPoint = ray.getPoint(hit.dist);
            jVec3 surfaceNormal = hit.hitObject->getNormal(hitPoint, hit.transform, hit);
            jVec3 viewVector = (ray.origin - hitPoint).normalize();

            // directional light,therefore make sure we hit the right side.
            float dn = viewVector*surfaceNormal;

            // if dn <  0 then we hit the wrongside; therefore we are in shadow
            return (dn < 0.0);
        }else{
            // not directional, so definitly not in shadow
            return false;
        }
    }

    // no other choice, we are blocked from the light source.
    return true;
}
