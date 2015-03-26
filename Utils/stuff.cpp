// #include "stuff.h"

// double dist(double x0, double y0, double x1, double y1)
// {
// 	return sqrt( (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0) ) ;
// }

// void fatal(char *s)
// {
//   cerr << s << "\n";
//   exit(99);
// }

// void drawCircle(double radius, double xcen, double ycen, bool line)
// {
//    GLint i;
//    static GLfloat circcoords[100][2];

//    for(i=0;i<100;i++) {
//      circcoords[i][0]=radius*cos(i*2*M_PI/100.0)+xcen;
//      circcoords[i][1]=radius*sin(i*2*M_PI/100.0)+ycen;
//    }
//    if (line)   glBegin(GL_LINES); else glBegin(GL_POLYGON);
//       for(i=0;i<100;i++)
//          glVertex2fv(&circcoords[i][0]);
//    glEnd();
// }

// void drawLine(double x0, double y0, double x1, double y1 )
// {
// 	glBegin(GL_LINES);
// 	glVertex2f(x0, y0);
//     glVertex2f(x1, y1);
//     glEnd();
// }

// void backCol()
// {
//   glColor3f(1.0f, 1.0f, 0.9f);
// }

// int sn(double posThresh)
// {
//   if (myrand()>posThresh) return 1; else return -1;
// }

// double randRange(double a, double b)
// {
//   return fabs(a-b)*myrand()+a;
// }

// int irand(int max)
// {
//   return (int)(max*myrand());
// }

// double myrand()
// {
//   return rand()/(double)RAND_MAX;
// }

// void error(char *buf, bool fatal)
// {
//   cerr << buf << "\n";
//   if (fatal) exit(-1);
// }

// /*int nearZero(double x)
// {
//     return (x <= EPSILON && x >= -EPSILON);
// }
// */
// int nearEpsilon(double x, double epsilon)
// {
//   return (x <= epsilon && x >= -epsilon);
// }

// jVec3 qcoltojvec3(QColor c)
// {
//     jVec3 a;
//     a[0]=c.redF();
//     a[1]=c.greenF();
//     a[2]=c.blueF();
//     return a;
// }

// /*QString today()
// {
//         QDate date = QDate::currentDate();
// 	QString s,syear;
// 	// returns a QString with todays date
// 	s.sprintf( "%2d/", date.day());
// 	s.append(date.shortMonthName(date.month()));
// 	syear.sprintf( "/%4d", date.year() );
// 	s.append(syear);
// 	return s;
// }*/
