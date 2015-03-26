//-------------------------------------------------------------------------------------------
//   Painting with Flowsnakes
// fsnake program modified to use open gl vertex arrays  Brian Wyvill October 2012
// added save/restore and postscript driver November 2012
// fixed memory management November 2012 delete works properly now.
// added progress bar to show memory usage.
//-------------------------------------------------------------------------------------------

#include <GL/glu.h>
#include <QVector3D>
#include <QMatrix4x4>
#include "glwidget2.h"
#include "Models/Primitive.h"
#include "Models/PrimitiveSphere.h"
#include "Utils/linearalgebra.h"

const double RadPerPixel = -0.1;
const double MovePerPixel = -0.1;
GLfloat points[][3] = {
    {-1.0,-1.0,-1.0},
    {1.0,-1.0,-1.0},
    {1.0,1.0,-1.0},
    {-1.0,1.0,-1.0},
    {-1.0,-1.0,1.0},
    {1.0,-1.0,1.0},
    {1.0,1.0,1.0},
    {-1.0,1.0,1.0},
};

GLWidget2::GLWidget2(QWidget *parent)
    : QGLWidget(parent)
{
}

GLWidget2::~GLWidget2()
{
}

void GLWidget2::setModel(WorldModel* model)
{
    this->model = model;
}

void GLWidget2::initializeGL()
{
     //Background color will be white
     glClearColor(0.0,0.0,0.0, 1.0);
     glEnable(GL_DEPTH_TEST);
     glShadeModel( GL_FLAT );
     glMatrixMode( GL_MODELVIEW );

     // allow the specification of geometry using vertex arrays.
     glEnableClientState(GL_VERTEX_ARRAY);

     glLoadIdentity();
     //glPointSize(5);
}

/* 2D */
void GLWidget2::resizeGL( int winw, int winh )
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1.0,1.0,-1.0,1.0,5.0,1500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glViewport(0,0,winw,winh);
}


void polygon(int a, int b, int c, int d,GLenum type = GL_LINE_LOOP){
    glBegin(type);
    glVertex3fv(points[a]);
    glVertex3fv(points[b]);
    glVertex3fv(points[c]);
    glVertex3fv(points[d]);
    glEnd();
}

void GLWidget2::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  glLoadIdentity();

  // position the camera
  gluLookAt(
    this->model->camera.pos[0],
    this->model->camera.pos[1],
    this->model->camera.pos[2],

    this->model->camera.at[0],
    this->model->camera.at[1],
    this->model->camera.at[2],

    0,1,0
    );
  this->model->camera.LookAt();
  // jFlt cam_mat[16];
  // this->model->camera.rotMatrix.toOpenGLMat(cam_mat);
  // GLfloat mat2[16];
  // for(int i = 0;i <16; ++i){
  //   mat2[i] = (GLfloat) cam_mat[i];
  // }
  // glMultMatrixf(mat2);


  glBegin(GL_LINES);
  float w = 4.0;
  glColor3f(1.0,0.0,0.0);
  glVertex3f(0,0,0);
  glVertex3f(w,0,0);

  glColor3f(0.0,1.0,0.0);
  glVertex3f(0,0,0);
  glVertex3f(0,w,0);

  glColor3f(0.0,0.0,1.0);
  glVertex3f(0,0,0);
  glVertex3f(0,0,w);
  glEnd();

  glPushMatrix();
  glColor3f(1.0,1.0,0.1);
  glTranslatef(this->model->camera.at[0],
              this->model->camera.at[1],
              this->model->camera.at[2]);
  glScalef(0.05,0.05,0.05);
  polygon(0,1,2,3); // front face
  polygon(4,5,6,7); // back face
  polygon(0,3,7,4); // left face
  polygon(1,5,6,2); // right face
  polygon(3,2,6,7); // top face
  polygon(0,1,5,4); // bottom face
  glPopMatrix();

  jMat4 mat;
  mat.toidentity();
  if( this->model->root != NULL) {
    this->model->root->draw(mat);
  }
  glPopMatrix();
}



void GLWidget2::mousePressEvent(QMouseEvent *e){
    if( e->buttons() == Qt::LeftButton){
        lastMousePos = e->pos();
        isRotating = true;

    }else if( e->buttons() == Qt::RightButton){
        lastMousePos = e->pos();
        isScaling = true;
    }
}

void GLWidget2::mouseReleaseEvent(QMouseEvent *e){
    if( (e->button() & Qt::LeftButton) && isRotating){
        doRotate(e->pos(),lastMousePos);
        isRotating = false;
    }
    if( (e->button() & Qt::RightButton) && isScaling){
        doScale(e->pos(), lastMousePos);
        isScaling = false;
    }
    updateGL();
}

void GLWidget2::mouseMoveEvent(QMouseEvent *e){
    bool flag = false;
    if( e->buttons() == Qt::LeftButton && isRotating){
        // left-click + dragging -> rotating the camera
        doRotate(e->pos(),lastMousePos);
        lastMousePos = e->pos();
        flag = true;
    }else if( e->buttons() == Qt::RightButton && isScaling){
        // right-click + dragging -> perform a zooming operation
        doScale(e->pos(), lastMousePos);
        lastMousePos = e->pos();
        flag = true;
    }

    if( flag){
        updateGL();
    }
}

void GLWidget2::RotateY(QVector3D& pVec, double rad)
{
    //TODO: rotate pVec around Z axis by the angle rad
    QMatrix4x4 m;
    m.setToIdentity();

    QVector3D UpVector(0,1,0);
    // UpVector.setX(this->model->camera.up[0]);
    // UpVector.setY(this->model->camera.up[1]);
    // UpVector.setZ(this->model->camera.up[2]);

    m.rotate(rad,UpVector);
    QVector3D p = m * pVec;
    pVec.setX(p.x());
    pVec.setY(p.y());
    pVec.setZ(p.z());
}

void GLWidget2::RotateZ(QVector3D& pVec, double rad)
{
    QMatrix4x4 m;
    m.setToIdentity();
    QVector3D UpVector(0,1,0);
    // UpVector.setX(this->model->camera.up[0]);
    // UpVector.setY(this->model->camera.up[1]);
    // UpVector.setZ(this->model->camera.up[2]);

    QVector3D CameraPos;
    CameraPos.setX(this->model->camera.pos[0]);
    CameraPos.setY(this->model->camera.pos[1]);
    CameraPos.setZ(this->model->camera.pos[2]);

    QVector3D axis = QVector3D::crossProduct(CameraPos,UpVector);

    m.rotate(rad,axis);
    QVector3D p = m * pVec;
    pVec.setX(p.x());
    pVec.setY(p.y());
    pVec.setZ(p.z());
}

void GLWidget2::doRotate(QPoint dest,QPoint orig){
    double yRot = (dest.x() - orig.x()) * RadPerPixel;
    double zRot = (dest.y() - orig.y()) * RadPerPixel;

    QVector3D CameraPos;
    CameraPos.setX(this->model->camera.pos[0]);
    CameraPos.setY(this->model->camera.pos[1]);
    CameraPos.setZ(this->model->camera.pos[2]);

    // left -right movement is azimuth
    RotateY(CameraPos,yRot);
    // up-down is elevation
    RotateZ(CameraPos,zRot);

    this->model->camera.pos[0] = CameraPos.x();
    this->model->camera.pos[1] = CameraPos.y();
    this->model->camera.pos[2] = CameraPos.z();
}

void GLWidget2::doScale(QPoint desc, QPoint orig){
    double length = this->model->camera.pos.length();
    double newLength = length + (desc.y() - orig.y()) * MovePerPixel;
    if (newLength > 0.5)
    {
        double ratio = newLength / length;
        this->model->camera.pos[0] *= ratio;
        this->model->camera.pos[1] *= ratio;
        this->model->camera.pos[2] *= ratio;
    }
}

void GLWidget2::moveCam(QKeyEvent* ev){
    this->model->camera.LookAt();

    if( ev->key() == Qt::Key_W){
        this->model->camera.at = this->model->camera.at + this->model->camera.v*0.1;
        updateGL();
    } else if( ev->key() == Qt::Key_S){
        this->model->camera.at = this->model->camera.at - this->model->camera.v*0.1;
        updateGL();
    } else if( ev->key() == Qt::Key_Q){
        this->model->camera.at = this->model->camera.at + this->model->camera.w*0.1;
        updateGL();
    } else if( ev->key() == Qt::Key_E){
        this->model->camera.at = this->model->camera.at - this->model->camera.w*0.1;
        updateGL();
    } else if( ev->key() == Qt::Key_A){
        this->model->camera.at = this->model->camera.at - this->model->camera.u*0.1;
        updateGL();
    } else if( ev->key() == Qt::Key_D){
        this->model->camera.at = this->model->camera.at + this->model->camera.u*0.1;
        updateGL();
    }
}
