//-------------------------------------------------------------------------------------------
//   Painting with Flowsnakes
// fsnake program modified to use open gl vertex arrays  Brian Wyvill October 2012
// added save/restore and postscript driver November 2012
// fixed memory management November 2012 delete works properly now.
// added progress bar to show memory usage.
//-------------------------------------------------------------------------------------------

#include <QElapsedTimer>
#include <QDebug>
#include "glwidget.h"
#include "Models/Primitive.h"
#include "Models/PrimitiveSphere.h"

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    imageReady=false;
    qtimage=NULL;
    glimage=NULL;
}

GLWidget::~GLWidget()
{
}

void GLWidget::setModel(WorldModel* model)
{
    this->model = model;
}

void GLWidget::initializeGL()
{
    //Background color will be white
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glShadeModel( GL_FLAT );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glPointSize(5);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    if (imageReady) displayImage();
}

/* 2D */
void GLWidget::resizeGL( int winw, int winh )
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glOrtho(0.0,GLdouble(winw),0,GLdouble(winh),-10.0,10.0);
    glFlush();

    glMatrixMode(GL_MODELVIEW);
    glViewport( 0, 0, (GLint)winw, (GLint)winh );
    cerr << "gl new size "<< width() << " " <<  height() << std::endl;
    qtimage=new QImage(width(), height(), QImage::Format_RGB32);
    glimage=new QImage(width(), height(), QImage::Format_RGB32);
    // note this is a potential memory leak unless qt implicit data sharing works
}


void GLWidget::prepareImageDisplay(QImage* myimage)
{
    // does the below copy?
    *glimage = QGLWidget::convertToGLFormat( *myimage );  // flipped 32bit RGBA stored as mi
    imageReady=true;
    updateGL();
}

// ------------
// hooked to the gui buttons
// -----------
void GLWidget::displayImage()
{
    if (glimage->width()==0) {
        cerr << "Null Image\n";
    } else {
        // display the image stored in glimage
        glRasterPos2i(0,0);
        glDrawPixels(glimage->width(), glimage->height(), GL_RGBA, GL_UNSIGNED_BYTE, glimage->bits());
        glFlush();
    }
}

void GLWidget::saveImage( QString fileBuf )
{
    // there is no conversion  back toQImage
    // hence the need to keep qtimage as well as glimage
    qtimage->save ( fileBuf );   // note it is the qtimage in the right format for saving
}

void GLWidget::makeImage( )
{
    QImage* img = new QImage(width(), height(), QImage::Format_RGB32);

    //make copy of the model and flatten out all the transforms in order to
    // make tracing much much faster.
    this->_updateMaxProgress(100);
    this->_updateProgress(0);

    WorldModel ray_model(*model);
    ray_model.flatten();
    // rayTracer.render(*img,ray_model,this,0,height());

    int num_threads =4;
    QThread* threads[num_threads];
    RayTracer* tracers[num_threads];
    int segments = height()/num_threads;
    for(int i = 0; i < num_threads; ++i){
        threads[i] = new QThread();
        tracers[i] = new RayTracer();
        tracers[i]->moveToThread(threads[i]);

        connect(threads[i],SIGNAL(started()),tracers[i],SLOT(handle_started()));
        threads[i]->start();

    }

    // wait for all the thread to finish
    for(int i = 0;i < num_threads; ++i){
        threads[i]->wait();
        qDebug() <<" waited for a thread" << i;
    }


    // clean up all the threads + workers
    for(int i = 0 ;i < num_threads; ++i){
        delete threads[i];
        delete tracers[i];
    }

    this->_updateProgress(100);

    *qtimage=img->copy(0, 0,  img->width(), img->height());  // this is for subsequent saving
    prepareImageDisplay(img);
}

void GLWidget::handle_finished(){
    qDebug() << "Handle finished\n";
}
void GLWidget::handle_started(){
    qDebug() << "Handle started\n";
}
void GLWidget::handle_terminated(){
    qDebug() << "Handle terminated\n";
}
void GLWidget::handle_render_row_finished(){
    qDebug() << "Render Row Finished\n";
}
