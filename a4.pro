#-------------------------------------------------
#
# Project created by QtCreator 2015-03-25T19:16:11
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = a4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    Materials/MaterialFactory.cpp \
    Materials/Material.cpp \
    Models/WorldModel.cpp \
    Models/SceneNode.cpp \
    Models/PrimitiveSphere.cpp \
    Models/Primitive.cpp \
    Models/LightSource.cpp \
    Models/Camera.cpp \
    RayTracer/RayTracer.cpp \
    RayTracer/Ray.cpp \
    RayTracer/HitRecord.cpp \
    Utils/stuff.cpp \
    Utils/linearalgebra.cpp \
    glwidget2.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    Materials/MaterialFactory.h \
    Materials/Material.h \
    Models/WorldModel.h \
    Models/SceneNode.h \
    Models/PrimitiveSphere.h \
    Models/Primitive.h \
    Models/LightSource.h \
    Models/Camera.h \
    RayTracer/RayTracer.h \
    RayTracer/Ray.h \
    RayTracer/HitRecord.h \
    Utils/stuff.h \
    Utils/linearalgebra.h \
    glwidget2.h

FORMS    += mainwindow.ui
