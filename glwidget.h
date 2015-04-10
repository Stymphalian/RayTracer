//-------------------------------------------------------------------------------------------
//  University of Victoria Computer Science Department
//	FrameWork for OpenGL application under QT
//  Course title: Computer Graphics CSC305
//-------------------------------------------------------------------------------------------
//These two lines are header guiardians against multiple includes
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <vector>
#include <QGLWidget>
#include <QProgressBar>
#include <QtGui>
#include <QtOpenGL>
#include <QElapsedTimer>
#include <math.h>

#include "Models/WorldModel.h"
#include "RayTracer/RayTracer.h"
// #include "Camera.h"
// #include "SceneNode.h"

// #include "LightSource.h"

//This is our OpenGL Component we built it on top of QGLWidget
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    //Constructor for GLWidget
    GLWidget(QWidget *parent = 0);

    //Destructor for GLWidget
    virtual ~GLWidget();

    void setModel(WorldModel* model);
    void saveImage(QString fileBuf);
    void makeImage();
    void setMaxNumberThreads(int value);
    int getMaxNumberThreads();

public slots:
    void handle_finished();
    void handle_started();
    void handle_terminated();
    void handle_render_row_finished();

signals:
    void updateProgress(int val);
    void updateMaxProgress(int val);
    void render(int id,QImage* canvas,WorldModel* model,int start_row,int end_row);
    void timeTakenToRender(qint64);

protected:
    //Initialize the OpenGL Graphics Engine
    void initializeGL();

    //All our painting stuff are here
    void paintGL();

    //When user resizes main window, the scrollArea will be resized and it will call this function from
    //its attached GLWidget
    void resizeGL(int width, int height);

    // void mousePressEvent(QMouseEvent *event);
    // void mouseReleaseEvent(QMouseEvent *event);
    // void mouseMoveEvent(QMouseEvent *event);
    // void wheelEvent(QWheelEvent *event);
    // void handleKeyPress(QKeyEvent *e);

private:
    bool imageReady;  // for display of timage
    void displayImage();
    void prepareImageDisplay(QImage* myimage); // converts from Qt to opengl format
    QImage *glimage, *qtimage;  // paintGL will display the gl formatted image
    // keep the qtimage around for saving (one is a copy of the other

    WorldModel* model;
    WorldModel* ray_model;

    int max_num_threads;
    QImage* processing_image;

    int progress_count;
    int num_running_threads;
    bool locked;
    std::vector<QThread*> threads;
    std::vector<RayTracer*> tracers;
    QElapsedTimer timer;
};


#endif
