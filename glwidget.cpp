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

    model = NULL;
    progress_count = 0;
    max_num_threads = 4;
    num_running_threads =0;
    locked = false;
    processing_image = NULL;
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


// Everything below here has to do with signal + slot in order to
// get threads working....
void GLWidget::makeImage( )
{
    if(locked == true){return;}

    processing_image = new QImage(width(), height(), QImage::Format_RGB32);
    progress_count = 0;
    num_running_threads = 0;
    locked = true;
    threads.resize(max_num_threads);
    tracers.resize(max_num_threads);

    // init the progress bar
    emit updateMaxProgress(height());
    emit updateProgress(progress_count);

    //make copy of the model and flatten out all the transforms in order to
    ray_model = new WorldModel(*model);
    ray_model->flatten();

    int segments = height()/max_num_threads;
    for(int i = 0; i < max_num_threads; ++i){
        threads[i] = new QThread();
        tracers[i] = new RayTracer();
        tracers[i]->id = i;
        tracers[i]->moveToThread(threads[i]);

        // threads -> tracers
        // when the thread starts, begin the rendering..
        // connect(threads[i],SIGNAL(started()),tracers[i],SLOT(jordan()));
        connect(this,SIGNAL(render(int,QImage*,WorldModel*,int,int)),tracers[i],SLOT(render(int,QImage*,WorldModel*,int,int)));

        // tracers -> this
        // when the tracer signals a finished row, update the progresss bar
        connect(tracers[i],SIGNAL(render_row_finished()),this,SLOT(handle_render_row_finished()));

        // clean up the tracers when they are finished rendering..
        // this also informs the thread to quit.
        connect(tracers[i],SIGNAL(finished()),threads[i],SLOT(quit()));
        connect(tracers[i],SIGNAL(finished()),tracers[i],SLOT(deleteLater()));
        connect(threads[i],SIGNAL(finished()),threads[i],SLOT(deleteLater()));

        // thread -> this
        // keep track of when all the threads are complete so that we can
        // safetly delete them and unlock the rendering workflow.
        connect(threads[i],SIGNAL(started()),this,SLOT(handle_started()));
        connect(threads[i],SIGNAL(finished()),this,SLOT(handle_finished()));
        //connect(threads[i],SIGNAL(terminated()),this,SLOT(handle_terminated()));
    }

    // begin the threads all at once.
    for(int i = 0 ; i< max_num_threads; ++i){
        threads[i]->start();
        if( i == max_num_threads - 1){
            emit render(i,processing_image,ray_model,i*segments,height()-1);
        }else{
            emit render(i,processing_image,ray_model,i*segments,(i+1)*segments - 1);
        }
    }
}


void GLWidget::handle_started(){
    if( num_running_threads == 0){
        timer.restart();
        timer.start();
    }
    num_running_threads += 1;
    //qDebug() << "Handle started " << num_running_threads;
}

void GLWidget::handle_finished(){
    num_running_threads -= 1;
//    qDebug() << "Handle finished "<< num_running_threads;

    if( num_running_threads == 0){
        // emit how long it has taken to finish the full render
        emit timeTakenToRender(timer.elapsed());

        // clean up all the threads + workers
        for(int i = 0 ;i < max_num_threads; ++i){
            threads[i] = NULL;
            tracers[i] = NULL;
        }
        delete ray_model;
        ray_model = NULL;

        // final update to the progress bar
        emit updateProgress(height());

        *qtimage=processing_image->copy(0, 0,processing_image->width(),processing_image->height());  // this is for subsequent saving
        prepareImageDisplay(processing_image);

        // unlock the workflow
        locked = false;
    }
}

void GLWidget::handle_terminated(){
    //qDebug() << "Handle terminated";
}

void GLWidget::handle_render_row_finished(){
    progress_count += 1;
    emit updateProgress(progress_count);
    //qDebug() << "Render Row Finished "<< progress_count;
}

void GLWidget::setMaxNumberThreads(int value){
    max_num_threads = value;
}
int GLWidget::getMaxNumberThreads(){
    return max_num_threads;
}