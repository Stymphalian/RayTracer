#ifndef stuff_h
#define stuff_h


const  double pib5 = M_PI/5.0;
const  double torr = sin (2.0*pib5) / sin(pib5);
const  int  X = 0;
const  int  Y = 1;
const  int  Z = 2;

const double svth=1.0/7.0;
const double todeg = 180.0/M_PI;
const double r3 = sqrt(3.0);
const double r7 = sqrt(7.0);
const double r32= sqrt(3.0)/2.0;
const double ir7=1.0/sqrt(7.0);
const double ir72=0.5/sqrt(7.0);


#include <qgl.h>
#include <qmessagebox.h>

int nearEpsilon(double x, double epsilon);
//int nearZero(double x);
double myrand();
void fatal(char *s);
void drawCircle(double radius, double xcen, double ycen, bool line);
void drawLine(double x0, double y0, double x1, double y1 );
void backCol();
int sn(double posThresh);
int irand(int max);
double randRange(double a, double b);
void error(char *buf, bool fatal);
//QString today();
double dist(double x0, double y0, double x1, double y1);
jVec3 qcoltojvec3(QColor c);


#endif
