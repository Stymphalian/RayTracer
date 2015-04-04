#include "Primitive.h"

#include <QVector3D>
#include <QRgb>
#include <QDebug>
#include "Utils/linearalgebra.h"

// --------------
// Scene Object
// --------------
Primitive::Primitive(){
    isLight = false;
    isFlat = false;
}
Primitive::Primitive(const Primitive& other){
    material = other.material;
    isLight = other.isLight;
    isFlat = other.isFlat;
}

Primitive::~Primitive(){}

bool Primitive::hasBoundingBox(){
    return false;
}

bool Primitive::intersectsBoundingBox(Ray& ray){
    return true;
}

bool Primitive::very_close(float v,float v2, float e){
    return (v - v2 < e && v- v2 > -e);
}

