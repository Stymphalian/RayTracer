 #include "linearalgebra.h"

//-----------------------------------------------------------------------------
// jVec3

// jVec3::normalize
//
// jNorm3izes the vector.
//
// Return
//      The vector with its length scaled so that it is a unit vector
const jVec3& jVec3::normalize ()
{
    jFlt l = length();
    
    if(l!=0) x/=l, y/=l, z/=l;
            
    return *this;
}

bool jVec3::jnearZero()
{
    if (nearZero(x) && nearZero(y) && nearZero(z)) return true;
    else return false;
}


// jVec3::majorAxis
//
// Returns the major axis of the vector.
/*
jUInt jVec3::majorAxis () const
{
    if(fltabs(x)>fltabs(y))
    {
        if(fltabs(x)>fltabs(z)) return 0;
        else return 2;
    }
    else
    {
        if(fltabs(y)>fltabs(z)) return 1;
        else return 2;
    }
}
*/
// jVec3::minorAxis
//
// Returns the minor axis of the vector.
/*
jUInt jVec3::minorAxis () const
{
    if(fltabs(x)<fltabs(y))
    {
        if(fltabs(x)<fltabs(z)) return 0;
        else return 2;
    }
    else
    {
        if(fltabs(y)<fltabs(z)) return 1;
        else return 2;
    }
}
*/

/* operator>>
//
// jVec3 input
istream& operator>> ( istream& is, jVec3& v )
{
  //    return is >> v[0] >> "," >> v[1] >> "," >> v[2];
}

// operator<<
//
// vector output
ostream& operator<< ( ostream& os, const jVec3& v )
{
    return os << "["
              << v.x << ", "
              << v.y << ", "
              << v.z << "]";
} */

jVec3 jVec3::min ( const jVec3& a, const jVec3& b )
{
    return jVec3(Min(a[0],b[0]),
                 Min(a[1],b[1]),
                 Min(a[2],b[2]));
}

jVec3 jVec3::max ( const jVec3& a, const jVec3& b )
{
    return jVec3(Max(a[0],b[0]),
                 Max(a[1],b[1]),
                 Max(a[2],b[2]));
}

jVec3 jVec3::lerp( const jVec3& v1, const jVec3& v2, jFlt t )
{
    return v1+((v2-v1)*t);
}

//-----------------------------------------------------------------------------
// jNorm3


// jNorm3::set
//
// The following functions should be obvious enough to figure out, this
// is how one sets the values in a normal vector, unit length is ensured.
jNorm3 jNorm3::set ( jFlt _x, jFlt _y, jFlt _z, bool n )
{
    x = _x, y = _y, z = _z;
    if(n)normalize();
    return *this;
}
jNorm3 jNorm3::set ( jUInt x, jFlt s )
{
    jVec3::operator[] (x) = s;
    normalize();
    return *this;
}

// jNorm3::add
//
// Adds another normal to this one without renormalizing the normal.
// This function is meant for use during averaging of normals, be sure to
// normalize this vector after you have finished adding.
void jNorm3::add ( const jNorm3& N )
{
    x += N.x, y+= N.y, z += N.z;
}

//-----------------------------------------------------------------------------
// jVec2

// jVec2::normalize
//
// normizes the vector.
//
// Return
//      The vector with its length scaled so that it is a unit vector
const jVec2& jVec2::normalize ()
{
    jFlt l = length();
    
    if(l!=0) x/=l, y/=l;
            
    return *this;
}

// jVec2::majorAxis
//
// Returns the major axis of the vector.
/*
jUInt jVec2::majorAxis () const
{
  if(fltabs(x)>fltabs(y)) return 0;
  else return 1;
}
*/

// operator>>
//
// jVec2 input
/*istream& operator>> ( istream& is, jVec2& v )
{
  // return is >> v[0] >> "," >> v[1];
}
*/
// operator<<
//
/* vector output
ostream& operator<< ( ostream& os, const jVec2& v )
{
	jFlt a,b;

	a = v.x; b=v.y;
	if (nearZero(v.x)) a=0.0;
	if (nearZero(v.y)) b=0.0;

    return os << "["
              << a << ", "
              << b << "]";
}*/

jVec2 jVec2::min ( const jVec2& a, const jVec2& b )
{
    return jVec2(Min(a[0],b[0]),
                  Min(a[1],b[1]));
}

jVec2 jVec2::max ( const jVec2& a, const jVec2& b )
{
    return jVec2(Max(a[0],b[0]),
                  Max(a[1],b[1]));
}

//-----------------------------------------------------------------------------
// jMat4


const jMat4 jMat4::Mbs = jMat4(      -1/6.0,  3/6.0, -3/6.0, 1/6.0,
                                           3/6.0, -6/6.0,  3/6.0, 0,
                                          -3/6.0,  0,      3/6.0, 0,
                                           1/6.0,  4/6.0,  1/6.0, 0);
const jMat4 jMat4::MbsI = jMat4(     0,      0.111,  -0.167, 0.167,
                                          0,      -0.056, 0,      0.167,
                                          0,      0.111,  0.167,  0.167,
                                          1,      0.611,  0.333,  0.167);

const jMat4 jMat4::Mcr = jMat4(       -.5,   1.5,     -1.5,    .5,
                                        1,   -2.5,   2,      -.5,
                                      -.5,     0,      .5,     0,
                                        0,     1,      0,      0);

const jMat4 jMat4::Mb = jMat4(         -1,      3,      -3,     1,
                                        3,     -6,       3,     0,
                                       -3,      3,       0,     0,
                                        1,      0,       0,     0);
                                        
jMat4::jMat4 (     jFlt m00, jFlt m01, jFlt m02, jFlt m03,
                   jFlt m10, jFlt m11, jFlt m12, jFlt m13,
                   jFlt m20, jFlt m21, jFlt m22, jFlt m23,
                   jFlt m30, jFlt m31, jFlt m32, jFlt m33 )
{
    m[0][0] = m00, m[0][1] = m01, m[0][2] = m02, m[0][3] = m03;
    m[1][0] = m10, m[1][1] = m11, m[1][2] = m12, m[1][3] = m13;
    m[2][0] = m20, m[2][1] = m21, m[2][2] = m22, m[2][3] = m23;
    m[3][0] = m30, m[3][1] = m31, m[3][2] = m32, m[3][3] = m33;
}

jMat4::jMat4 ( const jVec3& r0,
                   const jVec3& r1,
                   const jVec3& r2,
                   const jVec3& r3 )
{
    m[0][0] = r0[0], m[0][1] = r0[1], m[0][2] = r0[2], m[0][3] = 1;
    m[1][0] = r1[0], m[1][1] = r1[1], m[1][2] = r1[2], m[1][3] = 1;
    m[2][0] = r2[0], m[2][1] = r2[1], m[2][2] = r2[2], m[2][3] = 1;
    m[3][0] = r3[0], m[3][1] = r3[1], m[3][2] = r3[2], m[3][3] = 1;
}

jMat4::jMat4 ( jFlt bias, jFlt tension )
{
    jFlt sbias = bias*bias;
    jFlt cbias = sbias*bias;
    
    m[0][0] = -2*cbias;
    m[1][0] = 6*cbias;
    m[2][0] = -6*cbias;
    m[3][0] = 2*cbias;

    m[0][1] = 2*(tension + cbias + sbias + bias);
    m[1][1] = -3*(tension + 2*cbias + 2*sbias);
    m[2][1] = 6*(cbias-bias);
    m[3][1] = tension + 4*(sbias+bias);

    m[0][2] = -2*(tension + sbias+bias+1);
    m[1][2] = 3*(tension+2*sbias);
    m[2][2] = 6*bias;
    m[3][2] = m[0][3] = 2;

    m[1][3] = m[2][3] = m[3][3] = 0;

    bias = tension + 2*cbias + 4*sbias + 4*bias + 2;

    for(int j,i=0;i<4;i++)
        for(j=0;j<4;j++)        m[i][j] = m[i][j]/bias;  
}

jMat4::jMat4( const jVec3& eye, const jVec3& at, const jNorm3& U )
{
    identity();
    
    jNorm3 T = jNorm3(at-eye)*-1;
    jFlt vz = T*U;
    jFlt vy = sqrt(1-vz*vz);

    jNorm3 r = U*(1/vy) - T*(vz/vy);
    
    column(2,T,0),column(1,r,0),column(0,(U^T) * (1/vy),0);

    *this = jMat4(mTranslate,-eye[0],-eye[1],-eye[2])*(*this);
}

jMat4::jMat4( jFlt* T )
{
    for(int i=0; i<16; i++)
    {
        m[i/4][i%4] = T[i];
    }
}

//jMat4 operator* ( jFlt k, const jMat4& M ) { return M*k; }

// jMat4::column
//
// Sets the column to the given vector, the homogenous coordinate is
// supplied.
void jMat4::column ( jUSInt i, const jVec3& c, jFlt h )
{
    m[0][i] = c[0], m[1][i] = c[1], m[2][i] = c[2], m[3][i] = h;
}

static inline jFlt det2x2(jFlt a, jFlt b, jFlt c, jFlt d)
{
  return a*d - b*c;
}

static inline jFlt det3x3(jFlt a1, jFlt a2, jFlt a3,
       jFlt b1, jFlt b2, jFlt b3,
       jFlt c1, jFlt c2, jFlt c3)
{
  return (  a1 * det2x2(b2, b3, c2, c3)
	  - b1 * det2x2(a2, a3, c2, c3)
	  + c1 * det2x2(a2, a3, b2, b3));
}

jFlt jMat4::determinant() const
{
  jFlt a1 = m[0][0],  b1 = m[0][1],  c1 = m[0][2],  d1 = m[0][3];
  jFlt a2 = m[1][0],  b2 = m[1][1],  c2 = m[1][2],  d2 = m[1][3];
  jFlt a3 = m[2][0],  b3 = m[2][1],  c3 = m[2][2],  d3 = m[2][3];
  jFlt a4 = m[3][0],  b4 = m[3][1],  c4 = m[3][2],  d4 = m[3][3];
  
  jFlt ans = 
    + a1 * det3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4)
    - b1 * det3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4)
    + c1 * det3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4)
    - d1 * det3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4);
  return ans;
}

jMat4 jMat4::adjoint() const
{
  jFlt a1 = m[0][0],  b1 = m[0][1],  c1 = m[0][2],  d1 = m[0][3];
  jFlt a2 = m[1][0],  b2 = m[1][1],  c2 = m[1][2],  d2 = m[1][3];
  jFlt a3 = m[2][0],  b3 = m[2][1],  c3 = m[2][2],  d3 = m[2][3];
  jFlt a4 = m[3][0],  b4 = m[3][1],  c4 = m[3][2],  d4 = m[3][3];

  jMat4 M;

  M.m[0][0]  =   det3x3( b2, b3, b4, c2, c3, c4, d2, d3, d4);
  M.m[1][0]  = - det3x3( a2, a3, a4, c2, c3, c4, d2, d3, d4);
  M.m[2][0]  =   det3x3( a2, a3, a4, b2, b3, b4, d2, d3, d4);
  M.m[3][0]  = - det3x3( a2, a3, a4, b2, b3, b4, c2, c3, c4);
  
  M.m[0][1]  = - det3x3( b1, b3, b4, c1, c3, c4, d1, d3, d4);
  M.m[1][1]  =   det3x3( a1, a3, a4, c1, c3, c4, d1, d3, d4);
  M.m[2][1]  = - det3x3( a1, a3, a4, b1, b3, b4, d1, d3, d4);
  M.m[3][1]  =   det3x3( a1, a3, a4, b1, b3, b4, c1, c3, c4);
  
  M.m[0][2]  =   det3x3( b1, b2, b4, c1, c2, c4, d1, d2, d4);
  M.m[1][2]  = - det3x3( a1, a2, a4, c1, c2, c4, d1, d2, d4);
  M.m[2][2]  =   det3x3( a1, a2, a4, b1, b2, b4, d1, d2, d4);
  M.m[3][2]  = - det3x3( a1, a2, a4, b1, b2, b4, c1, c2, c4);
  
  M.m[0][3]  = - det3x3( b1, b2, b3, c1, c2, c3, d1, d2, d3);
  M.m[1][3]  =   det3x3( a1, a2, a3, c1, c2, c3, d1, d2, d3);
  M.m[2][3]  = - det3x3( a1, a2, a3, b1, b2, b3, d1, d2, d3);
  M.m[3][3]  =   det3x3( a1, a2, a3, b1, b2, b3, c1, c2, c3);

  return M;
}

jMat4 jMat4::inverse() const
{
    jMat4 M = adjoint();
    jFlt d = determinant();
    
    if(fabs(d) < 0.00000001)
    {
        cerr << "determinant: " << d << endl;
        cerr << "Error: Attempt to compute inverse of a ";
        cerr << "singular matrix." << endl;
        //cerr << *this << endl;
        abort();
    }
    
    for(int i=0; i<4; i++)
        for(int j=0; j<4; j++)
            M.m[i][j] /= d;

    return M;
}

jMat4 jMat4::transpose() const
{
  jMat4 T;
  int i,j;

  for(i=0; i<4; i++)
    for(j=0; j<4; j++)
      T.m[i][j] = m[j][i];
  return T;
}

// jMat4::operator<<
//
/* matrix output
ostream& operator<< ( ostream& os, const jMat4& M )
{
    for (int y=0;y<4;y++)
    {
        if(y) os << " ["; else os << "[[";
        for(int x=0;x<4;x++)
        {
            os << M.m[y][x] << " ";
        }
        if(y==3)os << "]]"; else os << "]\n";
    }
    
    return os;
}  */
// for now jst use this:
void jMat4::pmat()
{
    int i,j;

    double x;

    for (j=0; j<4; j++) {
      for (i=0; i<4; i++) {
          x = m[i][j];
          if ( nearZero(x)) x = 0.0;
        cerr << x << "\t ";

     }
      cerr << "\n";
  }
      cerr << "\n";
}

//-----------------------------------------------------------------------------
// jMat3


jMat3::jMat3 ( jFlt m00, jFlt m01, jFlt m02, 
                   jFlt m10, jFlt m11, jFlt m12,
                   jFlt m20, jFlt m21, jFlt m22 )
{
    m[0][0] = m00, m[0][1] = m01, m[0][2] = m02;
    m[1][0] = m10, m[1][1] = m11, m[1][2] = m12;
    m[2][0] = m20, m[2][1] = m21, m[2][2] = m22;
}

jMat3::jMat3 ( const jVec2& r0,
                   const jVec2& r1,
                   const jVec2& r2 )
{
    m[0][0] = r0[0], m[0][1] = r0[1], m[0][2] = 1;
    m[1][0] = r1[0], m[1][1] = r1[1], m[1][2] = 1;
    m[2][0] = r2[0], m[2][1] = r2[1], m[2][2] = 1;
}

jMat3::jMat3( jFlt* T )

{
    for(int i=0; i<9; i++)
    {
        m[i/3][i%3] = T[i];
    }
}

// jMat3::operator*
//
// Multiplies each component of a matrix by a scalar.
//
// Arguments
//      (*this) the matrix to scale
//      k       the scalar to multiply the components of A by
//
// Return
//      The scaled version of A.
jMat3 jMat3::operator* ( jFlt k ) const
{
    jMat3 A;
    for(int j,i=0;i<3;i++)
        for(j=0;j<3;j++)        A.m[i][j] = m[i][j]*k;
    return A;
}

//jMat3 operator* ( jFlt k, const jMat3& M ) { return M*k; }

// jMat3::column
//
// Sets the column to the given vector, the homogenous coordinate is
// supplied.
void jMat3::column ( jUSInt i, const jVec2& c, jFlt h )
{
    m[0][i] = c[0], m[1][i] = c[1], m[2][i] = h;
}

jFlt jMat3::determinant() const
{
  jFlt a1 = m[0][0],  b1 = m[0][1],  c1 = m[0][2];
  jFlt a2 = m[1][0],  b2 = m[1][1],  c2 = m[1][2];
  jFlt a3 = m[2][0],  b3 = m[2][1],  c3 = m[2][2];

  return (  a1 * det2x2(b2, b3, c2, c3)
	  - b1 * det2x2(a2, a3, c2, c3)
	  + c1 * det2x2(a2, a3, b2, b3));
}

jMat3 jMat3::adjoint() const
{
  jFlt a1 = m[0][0],  b1 = m[0][1],  c1 = m[0][2];
  jFlt a2 = m[1][0],  b2 = m[1][1],  c2 = m[1][2];
  jFlt a3 = m[2][0],  b3 = m[2][1],  c3 = m[2][2];

  jMat3 M;

  M.m[0][0]  =   det2x2( b2, b3, c2, c3);
  M.m[1][0]  = - det2x2( a2, a3, c2, c3);
  M.m[2][0]  =   det2x2( a2, a3, b2, b3);
  
  M.m[0][1]  = - det2x2( b1, b3, c1, c3);
  M.m[1][1]  =   det2x2( a1, a3, c1, c3);
  M.m[2][1]  = - det2x2( a1, a3, b1, b3);
  
  M.m[0][2]  =   det2x2( b1, b2, c1, c2);
  M.m[1][2]  = - det2x2( a1, a2, c1, c2);
  M.m[2][2]  =   det2x2( a1, a2, b1, b2);

  return M;
}

jMat3 jMat3::inverse() const
{
    jMat3 M = adjoint();
    jFlt d = determinant();
    if(fabs(d) < 0.00001)
    {
        cerr << "Error: Attempt to compute inverse of a ";
        cerr << "singular matrix." << endl;
        abort();
    }
    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
            M.m[i][j] /= d;

    return M;
}

jMat3 jMat3::transpose() const
{
  jMat3 T;
  int i,j;

  for(i=0; i<3; i++)
    for(j=0; j<3; j++)
      T.m[i][j] = m[j][i];
  return T;
}

// jMat3::operator<<
//
// matrix output
ostream& operator<< ( ostream& os, const jMat3& M )
{
	 
    for (int y=0;y<3;y++)
    {
        if(y) os << " ["; else os << "[[";
        for(int x=0;x<3;x++)
        {   
			jFlt a;
			a=M.m[y][x];
            if (nearZero(a)  ) os << 0.0 << " "; else 
				os << M.m[y][x] << " ";
        }
        if(y==2)os << "]]"; else os << "]\n";
    }
    
    return os;
}

//===========================================================================

jMat3Build::jMat3Build()

        : T(), stack(0)
{}

jMat3Build::~jMat3Build()
{
    m_node* mn = stack;
    while(mn)
    {
        stack = mn;
        mn = mn->n;
        delete stack;
    }
}

void jMat3Build::identity()
{
    T = jMat3();
}

void jMat3Build::scale( jFlt a, jFlt b )
{
    T = T*jMat3(jMat3::mScale,a,b);
}

void jMat3Build::translate( jFlt a, jFlt b )
{
    T = T*jMat3(jMat3::mTranslate,a,b);
}

void jMat3Build::rotate( jFlt a )
{
    T = T*jMat3(jMat3::mRotate,a);
}

void jMat3Build::push()
{
    stack = new m_node(T,stack);
}

void jMat3Build::pop()
{
    if(stack)
    {
        T = stack->T;
        m_node * top = stack;
        stack = stack->n;
        delete top;
    }
}
