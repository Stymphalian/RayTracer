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
}
Primitive::~Primitive(){}
