//-------------------------------------------------------------------------------------------
//  University of Victoria Computer Science Department
//  FrameWork for OpenGL application under QT
//  Course title: Computer Graphics CSC305
//-------------------------------------------------------------------------------------------
//These two lines are header guiardians against multiple includes
#ifndef GLWIDGET2_H
#define GLWIDGET2_H

#include <vector>
#include <QGLWidget>
#include <QProgressBar>
#include <QtGui>
#include <QtOpenGL>
#include <QPoint>
#include <math.h>
#if defined(Q_WS_MAC)
    #include <OpenGL/glu.h>
#else
    #include <GL/glu.h>
#endif
#include "Models/WorldModel.h"

//This is our OpenGL Component we built it on top of QGLWidget
class GLWidget2 : public QGLWidget
{
    Q_OBJECT

public:
    //Constructor for GLWidget
    GLWidget2(QWidget *parent = 0);

    //Destructor for GLWidget
    virtual ~GLWidget2();

    void setModel(WorldModel* model);
    void moveCam(QKeyEvent* ev);

signals:
    void updateUI(QString s);

protected:
    //Initialize the OpenGL Graphics Engine
    void initializeGL();

    //All our painting stuff are here
    void paintGL();

    //When user resizes main window, the scrollArea will be resized and it will call this function from
    //its attached GLWidget
    void resizeGL(int width, int height);


    // handle moving the camera around
    QPoint lastMousePos;
    bool isScaling;
    bool isRotating;

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void doRotate(QPoint start, QPoint dest);
    void doScale(QPoint start, QPoint dest);
    void RotateY(QVector3D& pVec, double rad);
    void RotateZ(QVector3D& pVec, double rad);

    void SetupMatrices(int type);
    jMat4 setupcamera(jVec3 eye,jVec3 at,jVec3 up_dir);
    jMat4 setuportho(float l,float r,float b, float t, float n,float f);
    jMat4 setupperspective(float l,float r,float b, float t, float n,float f);
    jMat4 setupproj(float l,float r,float b,float t, float n,float f);
    jMat4 setupviewport(int x, int y, int w, int h);

    jMat4 Mat_viewport;
    jMat4 Mat_camera;
    jMat4 Mat_perspective;
    int win_w;
    int win_h;

private:
    WorldModel* model;
};

#endif
