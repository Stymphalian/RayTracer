// warning I have started to change this to column vectors and post multiplication
// all matrices switched from previous version
//
#ifndef j_linearalgebra_h
#define j_linearalgebra_h
using namespace std;
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <iostream>
#include <math.h>
#include <time.h>
#include <fstream>
#include <limits>

#include <QtOpenGL>

//#include <values.h>  this causes endless trouble for 2 constants
//one of which is not used !! Mark what were you thinking !
#include <float.h>

// to avoid calling values.h for compatability with windows
//#  define MAXFLOAT	    3.40282347e+38F
#  define MINFLOAT     -3.40282347e+38F

//---------------------------------------------------------------------------
// Type definitions

#ifdef NULL
#undef NULL
#endif
#define NULL 0

typedef double                  jFlt;
typedef int                     jInt;
typedef short int               jSInt;
typedef unsigned short int      jUSInt;
typedef unsigned long           jULng;
typedef long                    jLng;
typedef unsigned char           jByte;
typedef unsigned int            jUInt;

const jFlt MaxjFlt         = FLT_MAX;
const jFlt MinjFlt         = MINFLOAT;
const jFlt jFlt_EPSILON    = FLT_EPSILON;
//---------------------------------------------------------------------------
// if C++ type `bool' is not defined with compiler being used ...

#ifdef NOBOOL
typedef int bool;

#define true  1
#define false 0
#endif

//---------------------------------------------------------------------------
// Constant definitions

// mathematical definitions
const jFlt      pi       = 3.14159265359;
const jFlt      pi2      = pi*2;
const jFlt      pi_2     = pi*0.5;
const jFlt      root2    = 1.41421356237;
const jFlt      root3    = 1.73205080757;
const jFlt      ln2      = 0.69314718056;
const jFlt      e        = 2.71828182846;



//---------------------------------------------------------------------------
// Macros

#define Max(a,b) ((a)>(b) ? (a) : (b))
#define Min(a,b) ((a)<(b) ? (a) : (b))

#define Max3(a,b,c) ((a)>(b) ? ((a)>(c) ? (a) : (c)) : ((b)>(c) ? (b) : (c)))
#define Min3(a,b,c) ((a)<(b) ? ((a)<(c) ? (a) : (c)) : ((b)<(c) ? (b) : (c)))

#define Min3i(a,b,c) ((a)<(b) ? ((a)<(c) ? 0 : 2) : ((b)<(c) ? 1 : 2))

#define deg2rad(d) ((pi*(d))/180.0)

#define rad2deg(r) ((r)*180.0/pi)

#define sgn(x) ((x)>=0.0 ? 1.0 : -1.0)

#define mintime(a,b) (((a)>0 && (b)>0) ? Min(a,b) : ((a)>0 ? (a) : (b)))

#define scurve(t) ((t)*(t)*(3.0-2.0*(t)))

#define mclamp(x,a,b) ((x)<(a) ? (a) : ((x)>(b) ? (b) : (x)))

#ifdef NOCBRT
#define cbrt(x) pow(x,0.3333333)
#endif

#define QSORT_CMPF (int (*) (const void *, const void *))


//---------------------------------------------------------------------------
// Utility functions
/*! true if near zero */
inline int nearZero(jFlt x)
{
#define FEPSILON 0.00001
    return (x <= FEPSILON && x >= -FEPSILON);
}



/*! Produces a beep on the console by writing a char 7 to standard error. */
void beep();

/*! Computes the absolute value. */
inline jFlt fltabs ( jFlt x ){
  if( x < 0){return -x;}
  else return x;
}

/*! Squares a number */
inline jFlt sqr( jFlt );

/*! Gamma correction, x^g */
inline jFlt gammacorrect ( jFlt g, jFlt x );

/*! Bias,  x^(log(b)/log(0.5)) */
inline jFlt bias ( jFlt b, jFlt x );

/*! Gain, x<0.5 ? bias(1-g,  2*x)/2 : 1-bias(1-g,2-2*x)/2 */
inline jFlt gain ( jFlt g, jFlt x );

/*! Clamp, forces x to be a <= x <= b */
inline jFlt clamp ( jFlt x, jFlt a, jFlt b );

/*! Clamp, forces x to be a <= x <= b */
inline jUSInt clamp ( jUSInt x, jUSInt a, jUSInt b );

/*! Cubic interpolation at x between a and b.  Where a and b are at 0 and 1
  respectively. */
inline jFlt smoothstep ( jFlt x, jFlt a, jFlt b );

/*! Discontinuous step at a, x >= a ? 1.0 : 0.0 */
inline jFlt step ( jFlt x, jFlt a );

/*! Floating point mod, a%b  (ex.  0.7 % 0.5 == 0.2). */
inline jFlt mod ( jFlt a, jFlt b );

/*! Rounds the given value to the nearest integer. */
inline jFlt round ( jFlt );

/*! Returns a random sample with dx of x. */
inline jFlt jitter ( jFlt x, jFlt dx );

/*! Returns true if x is a <= x <= b */
inline bool inside ( jFlt x, jFlt a, jFlt b );

/*! Ray time lerping */
inline jFlt tlerp( jFlt, jFlt, jFlt, jFlt, jFlt );

/*! Swaps the two values. */
inline void swap ( jFlt&, jFlt& );

/*! Swaps the two values. */
inline void swap ( jInt&, jInt& );

/*! Linear interpolation of the values. */
inline jFlt lerp ( jFlt, jFlt, jFlt );

/*! Returns the floor of the given floating point number. */
inline jFlt fltflr( jFlt );

extern jFlt TrigTable[360][2];

/*! Returns the sin of the given degree. */
inline jFlt fstsin( jInt );

/*! Returns the cos of the given degree. */
inline jFlt fstcos( jInt );

/*! Returns the maximum of two floating point numbers. */
inline jFlt max(jFlt a, jFlt b);

/*! Returns the maximum of four floating point numbers. */
inline jFlt max(jFlt a, jFlt b, jFlt c, jFlt d);

/*! Returns the minimum of two floating point numbers. */
inline jFlt min(jFlt a, jFlt b);

/*! Returns the minimum of four floating point numbers. */
inline jFlt min(jFlt a, jFlt b, jFlt c, jFlt d);

/*! Returns -1 if a<0 o/w 1 */
inline jInt sign(jFlt a);

/*! Returns a IUD random number 0..1 */
//jFlt frand ();

/*! Prints a carriage return and flushes. */
//ostream& cr ( ostream& );    all this is I/O is old and no longer works



class jVec4;
class jMat4;
class jMat3;



/*!
  \class jVec3 linearalgebra.h
  \brief Definition of a 3D point/vector class.

  This class is used throughout the library as a point or vector class.  It
  is not a homogeneous coordinate class per se.  An implied homogenous
  coordinate of 1 is added for affine transformations (see the jMat4 class).
*/
class jVec3
{

protected:    // classes inheriting from this class can access protected members
    jFlt x,y,z;

public:

    //! Initializes the vector to the specified components
    inline jVec3( jFlt _x = 0, jFlt _y = 0, jFlt _z = 0 )
            : x(_x), y(_y), z(_z) {};

    /*! Computes the linear interpolation of the given vectors with the
      given scalar to initialize with. */
    inline jVec3( const jVec3&, const jVec3&, jFlt );

    inline void setValues(jFlt _x,jFlt _y,jFlt _z){
        x = _x;
        y = _y;
        z = _z;
    }

    void toOpenGLFormat(GLfloat* arr){
      arr[0] = x;
      arr[1] = y;
      arr[2] = z;
    }

    //! Computes the length of the given vector.
    inline jFlt length () const;

    //! Computes the square of the length.
    inline jFlt slength () const;

    //! Computes the square of the length.
    inline jFlt sqlen () const;

    //! normalizes the given vector
    const jVec3& normalize ();

    //! Returns the dot product of the two vectors
    inline jFlt operator* ( const jVec3& ) const;

    //! Returns the dot product of the two vectors
    friend jFlt dot(const jVec3&, const jVec3&);

    //! Returns the cross product of the two vectors
    friend jVec3 cross(const jVec3&, const jVec3&);

    //! Returns the scalar multiplication
    inline jVec3 operator* ( jFlt ) const;

    //! Returns the scalar division
    inline jVec3 operator/ ( jFlt ) const;

    //! Sets this vector the scalar multiple.
    inline void operator*= ( jFlt );

    //! Transformation of the position by an affine transformation.
    inline jVec3 operator* ( const jMat4& ) const;

    //! Sets this vector to the transformation by an affine transformation.
    inline void operator*= ( const jMat4& );

    //! Adds vector to this vector.
    inline void operator+= ( const jVec3& );

    //! Returns the sum of two vectors.
    inline jVec3 operator+ ( const jVec3& ) const;

    //! Returns the difference of two vectors
    inline jVec3 operator- ( const jVec3& ) const;

    //! Returns the cross product of two vectors
    inline jVec3 operator^ ( const jVec3& ) const;

    //! Returns the outer product between the two vectors
    inline jVec3 outerProduct( const jVec3& other){
      return jVec3(x*other[0], y*other[1],z*other[2]);
    }

    //! Equality test between vectors
    inline bool operator== ( const jVec3& ) const;

    //! Inequality test between vectors
    inline bool operator!= ( const jVec3& ) const;

    //! Negation
    friend jVec3 operator- ( const jVec3& );

    //! Index into the vector.
    inline jFlt& operator[] ( int );

    //! \overload
    inline jFlt  operator[] ( int ) const;

    /*! Returns the distance given the l-metric distance parameter. */
    inline jFlt ldistance( jFlt l ) const;


    /*! Non-uniform scale.  Returns a new vector scaled by the three floats
      given. */
    jVec3 nuscale( jFlt kx, jFlt ky, jFlt kz ) const
        { return jVec3(x*kx,y*ky,z*kz); }

    /*! Sets this vector to the absolute value of itself, and returns a
      reference to the changed vector. */
    const jVec3& abs()
        { x = fltabs(x), y = fltabs(y), z = fltabs(z); return *this; }

    //! Returns the major axis of the vector
    jUInt majorAxis() const;

    //! Returns the minor axis of the vector
    jUInt minorAxis() const;

    /*! \relates jVec3
      Read the components for a vector from an input stream. */
    //friend istream& operator>> ( istream&, jVec3& );

    /*! \relates jVec3
      Write the components of the vector to an output stream. */
  //  friend ostream& operator<< ( ostream&, const jVec3& );

    //! Returns the minimum of the two vectors.
    static jVec3 min ( const jVec3&, const jVec3& );

    //! returns the maximum of the two vectors.
    static jVec3 max ( const jVec3&, const jVec3& );

    static jVec3 lerp( const jVec3&, const jVec3&, jFlt );

    bool jnearZero();
};

/*!
  \class jNorm3 linearalgebra.h
  \brief Strictly vectors of unit length.

  jVec3 sub class to implement unit length vectors suitable for use as
  surface normal or basis definition vectors.  The unit length is preserved
  through operations such as addition and multiplication. */
class jNorm3 : public jVec3
{
public:

    //! Initialization to [0,1,0]
    inline jNorm3();

    /*! Initialization to the same vector of unit length, the bool indicates
      a need for normalization. */
    inline jNorm3( const jVec3&,bool = true );

    /*! Initialization to the vector specified by the components, the bool
      indicates a need for normalization. */
    inline jNorm3( jFlt, jFlt, jFlt, bool = true );

    /*! Compute and return the vector that is the reflection about the give
      normal vector, the second optional parameter is the dot-product
      between the two normals.  If the value is out of range the dot
      product is computed. */
    inline jNorm3 reflect( const jNorm3&, jFlt=2.0 ) const;

    /*! Computes the refraction vector about the given normal.  The parameters
      are the axis about which to refract and the incoming and outgoing
      indices of refraction of the media and if tir does not occur the
      result of the refraction.  If tir does occur the function returns false
      o/w true. */
    inline bool refract( const jNorm3&, jFlt, jFlt, jNorm3& ) const;

    /*! Post multiplication by a transformation matrix, this operation is
     overridden to ensure that transformations involving translation is
     refused and after a scale the vector is returned to unit length. */
    inline jNorm3 operator* ( const jMat4& ) const;

    /*! Cross product of a normal and a normal is a normal, orthogonal to
     the two operands. */
    inline jNorm3 operator^ ( const jNorm3& ) const;

    /*! Reference acess to the members of a normal vector is not allowed,
      the user can only get the values using indexing. */
    inline jFlt operator[] ( int ) const;

    /*! Sets the components of the normal. if the boolean is true the resulting
      vector is normalized. */
    jNorm3 set ( jFlt, jFlt, jFlt, bool=true );

    /*! Sets only the ith component of the normal, re-normalizes by default. */
    jNorm3 set ( jUInt, jFlt );

    /*! Assignment must be written to ensure that unit length is preserved. */
    inline virtual jNorm3 operator= ( const jVec3& );

    //! Scalar multiplication
    inline jVec3 operator* ( jFlt ) const;

    //! Addition of a vector
    inline jVec3 operator+ ( const jVec3& ) const;

    //! Subtraction of a vector
    inline jVec3 operator- ( const jVec3& ) const;

    //! Dot product
    inline jFlt   operator* ( const jVec3& ) const;

    //! Cross product
    inline jVec3 operator^ ( const jVec3& ) const;

    /*! This function is an identity it must be written to hide the
      jVec3::operator*=. */
    inline void operator *= ( jFlt ) ;

    //! Returns the negation of this normal.
    inline jNorm3 negation() const;

    //! Negates this normal
    inline const jNorm3& negate();

    /*! Adds another normal to this one without renormalizing the normal.
     This function is meant for use during averaging of normals, be sure to
     normalize this vector after you have finished adding. */
    void add ( const jNorm3& );
};

/*!
  \class jVec2 linearalgebra.h
  \brief Definition of a 2D point/vector class.

  This class is used throughout the library as a 2D point or vector class.  It
  is not a homogenous coordinate class per se.  An implied homogenous
  coordinate of 1 is added for affine transformations (see the jMat3 class).
*/
class jVec2
{
public:

    //! Initializes the vector to the specified components
    inline jVec2( jFlt _x = 0, jFlt _y = 0 ) : x(_x), y(_y) {}

    /*! Computes the linear interpolation of the given vectors with the
      given scalar to initialize with. */
    inline jVec2( const jVec2&, const jVec2&, jFlt );

    //! Computes the length of the given vector.
    inline jFlt length () const;

    //! Computes the square of the length.
    inline jFlt slength () const;

    //! jNorm3izes the given vector
    const jVec2& normalize ();

    //! Returns the dot product of the two vectors
    inline jFlt operator* ( const jVec2& ) const;

    //! Returns the scalar multiplication
    inline jVec2 operator* ( jFlt ) const;

    //! Sets this vector the scalar multiple.
    inline void operator*= ( jFlt );

    //! Transformation of the position by an affine transformation.
    inline jVec2 operator* ( const jMat3& ) const;

    //! Sets this vector to the transformation by an affine transformation.
    inline void operator*= ( const jMat3& );

    //! Returns the sum of two vectors.
    inline jVec2 operator+ ( const jVec2& ) const;

    //! Returns the difference of two vectors
    inline jVec2 operator- ( const jVec2& ) const;

    //! Equality test between vectors
    inline bool operator== ( const jVec2& ) const;

    //! Inequality test between vectors
    inline bool operator!= ( const jVec2& ) const;

    //! Index into the vector.
    inline jFlt& operator[] ( int );

    //! \overload
    inline jFlt  operator[] ( int ) const;

    /*! Non-uniform scale.  Returns a new vector scaled by the three floats
      given. */
    jVec2 nuscale( jFlt kx, jFlt ky) const
        { return jVec2(x*kx,y*ky); }

    /*! Sets this vector to the absolute value of itself, and returns a
      reference to the changed vector. */
    const jVec2& abs()
        { x = fltabs(x), y = fltabs(y); return *this; }

    //! Returns the major axis of the vector
    jUInt majorAxis () const;

    /*! \relates jVec2
      Read the components for a vector from an input stream. */
    //friend istream& operator>> ( istream&, jVec2& );

    /*! \relates jVec2
      Write the components of the vector to an output stream. */
   // friend ostream& operator<< ( ostream&, const jVec2& );

    //! Returns the minimum of the two vectors.lag

    static jVec2 min ( const jVec2&, const jVec2& );

    //! returns the maximum of the two vectors.
    static jVec2 max ( const jVec2&, const jVec2& );

protected:

    jFlt x,y;
};

/*!
  \class jMat4 linearalgebra.h
  \brief Base class for affine transformation matrices.

  This class implements homogenous transformations.  All linear transformations
  of points (translate, scale, rotate) can be applied through product to
  combine a single transformation matrix. */
class jMat4
{
public:

    /*!
      This type is used for creating affine transformation matrices.  See the
      constructors for the jMat4 class. */
    enum Type
    {
        mNull, mIdentity,
        mRotateA, mRotateX, mRotateY, mRotateZ,
        mScale, mTranslate
    };

    /*! Creates a matrix from the specified components. */
    jMat4 ( jFlt, jFlt, jFlt, jFlt,
             jFlt, jFlt, jFlt, jFlt,
             jFlt, jFlt, jFlt, jFlt,
             jFlt, jFlt, jFlt, jFlt );

    /*! Creates a matrix from the specified rows. */
    jMat4 ( const jVec3&,
            const jVec3&,
            const jVec3&,
            const jVec3& );

    /*! Creates a Beta spline basis matrix, the parameters are the bias
      and tension parameters. */
    jMat4 ( jFlt, jFlt );

    /*! Creates a lookat matrix (implemented from Blinn).  Parameters are
      View origin, focal point, and world up vector. */
    jMat4( const jVec3&, const jVec3&, const jNorm3& );

    //! Creates an identity matrix or a null matrix.
    inline jMat4 ( Type=mIdentity );

    //! Creates any type except for rotate.
    inline jMat4 ( Type, jFlt, jFlt, jFlt );

    //! Creates any type of matrix, uniform scale, rotation angle in radians.
    inline jMat4 ( Type, jFlt );

    /*! Creates a rotation matrix about the given direciton vector.
      \arg must be mRotateA
      \arg the direction vector to rotate around
      \arg the number of radians to rotate by
      */
    inline jMat4 ( Type, jNorm3, jFlt );

    //! Creates a matrix from the array (there better be 16) of components
    jMat4(jFlt*);

    //! Scalar multiplication of the matrix.
    inline jMat4 operator* ( jFlt ) const;

    //! Addition of two matrices
    inline jMat4 operator+ ( const jMat4& ) const;

    //! Transformation by the given matrix.
    inline jMat4 operator* ( const jMat4& ) const;

    /*! \relates jMat4
      Write the matrix to an output stream. */
    //friend ostream& operator<< ( ostream&, const jMat4& );

    //! Computes the matrix with out the translation component.
    inline jMat4 rot () const;

    //! Return the ij'th (row,column) component of the matrix.
    jFlt ij( int i, int j ) const { return m[i][j]; }

    //! Set's the ij'th (row,column) component of the matrix.
    void ij( int i, int j, jFlt e ) { m[i][j] = e; }

    /*! Sets the column to the given vector, the homogenous coordinate is
     supplied. */
    void column ( jUSInt, const jVec3&, jFlt );

    //! Computes the determinant.
    jFlt determinant() const;

    //! Computes the adjoint matrix.
    jMat4 adjoint() const;

    //! Computes the inverse matrix, aborts on a singular matrix.
    jMat4 inverse() const;

    //! Computes the transpose of the matrix.
    jMat4 transpose() const;

    //! Tests to see if this matrix is the identity matrix.
    bool isidentity() const
        {
            return ((m[0][0]==1) && (m[0][1]==0) &&
                    (m[0][2]==0) && (m[0][3]==0) &&
                    (m[1][0]==0) && (m[1][1]==1) &&
                    (m[1][2]==0) && (m[1][3]==0) &&
                    (m[2][0]==0) && (m[2][1]==0) &&
                    (m[2][2]==1) && (m[2][3]==0) &&
                    (m[3][0]==0) && (m[3][1]==0) &&
                    (m[3][2]==0) && (m[3][3]==1));
        }

    void toidentity(){identity(); }

    virtual void translate(jFlt dx, jFlt dy, jFlt dz){
      jMat4 t(mTranslate,dx,dy,dz);
      // *this = t*(*this);
      *this = (*this)*t;
    }
    virtual void scale(jFlt dx, jFlt dy, jFlt dz){
      jMat4 t(mScale,dx,dy,dz);
      // *this = t*(*this);
      *this = (*this)*t;
    }

    virtual void rotate(jVec3& v, jFlt theta){
      jNorm3 n(v);
      jMat4 r(mRotateA,n,theta);
      *this = (*this)*r;
    }

    //! B-Spline basis
    static const jMat4 Mbs;

    //! Inverse of the B-Spline basis.
    static const jMat4 MbsI;

    //! Catmull-Rom basis
    static const jMat4 Mcr;

    //! Bezier basis
    static const jMat4 Mb;
    void pmat();


    void setValues(    jFlt m00, jFlt m01, jFlt m02, jFlt m03,
                       jFlt m10, jFlt m11, jFlt m12, jFlt m13,
                       jFlt m20, jFlt m21, jFlt m22, jFlt m23,
                       jFlt m30, jFlt m31, jFlt m32, jFlt m33 )
    {
        m[0][0] = m00, m[0][1] = m01, m[0][2] = m02, m[0][3] = m03;
        m[1][0] = m10, m[1][1] = m11, m[1][2] = m12, m[1][3] = m13;
        m[2][0] = m20, m[2][1] = m21, m[2][2] = m22, m[2][3] = m23;
        m[3][0] = m30, m[3][1] = m31, m[3][2] = m32, m[3][3] = m33;
    }

    void toOpenGLMat(GLfloat mat[16]){
      mat[0]  = m[0][0],   mat[1]  = m[0][1],   mat[2]  = m[0][2],  mat[3]  = m[0][3];
      mat[4]  = m[1][0],   mat[5]  = m[1][1],   mat[6]  = m[1][2],  mat[7]  = m[1][3];
      mat[8]  = m[2][0],   mat[9]  = m[2][1],   mat[10] = m[2][2],  mat[11] = m[2][3];
      mat[12] = m[3][0],   mat[13] = m[3][1],   mat[14] = m[3][2],  mat[15] = m[3][3];
    }

protected:
    jFlt m[4][4];
private:

    void identity ()
    {
        m[0][0]=1,m[0][1]=0,m[0][2]=0,m[0][3]=0;
        m[1][0]=0,m[1][1]=1,m[1][2]=0,m[1][3]=0;
        m[2][0]=0,m[2][1]=0,m[2][2]=1,m[2][3]=0;
        m[3][0]=0,m[3][1]=0,m[3][2]=0,m[3][3]=1;
    }
};

/*!
  \class jMat3 linearalgebra.h
  \brief Base class for affine transformation matrices.

  This class implements homogenous transformations.  All linear transformations
  of points (translate, scale, rotate) can be applied through product to
  combine a single transformation matrix. */
class jMat3
{
public:

    /*!
      This type is used for creating affine transformation matrices.  See the
      constructors for the jMat3 class. */
    enum Type
    {
        mNull, mIdentity, mRotate, mScale, mTranslate, mBetween
    };

    /*! Creates a matrix from the specified components. */
    jMat3 ( jFlt, jFlt, jFlt,
             jFlt, jFlt, jFlt,
             jFlt, jFlt, jFlt);

    /*! Creates a matrix from the specified rows. */
    jMat3 ( const jVec2&,
             const jVec2&,
             const jVec2& );

    /* Creates a Beta spline basis matrix, the parameters are the bias
      and tension parameters. */
  //    jMat3 ( jFlt, jFlt );

    //! Creates an identity matrix or a null matrix.
    inline jMat3 ( Type=mIdentity );

    //! Creates any type except for rotate.
    inline jMat3 ( Type, jFlt, jFlt );

    //! Creates any type of matrix, uniform scale & rotate.
    inline jMat3 ( Type, jFlt );

	// creates a special roatation scale matrix from 4 points
	inline jMat3 (Type, jVec2, jVec2, jVec2, jVec2); // mBetween

    //! Creates a matrix from the array (there better be 9) of components
    jMat3(jFlt*);

    //! Scalar multiplication of the matrix.
    jMat3 operator* ( jFlt ) const;

    //! Transformation by the given matrix.
    inline jMat3 operator* ( const jMat3& ) const;

    /*! \relates jMat3
      Write the matrix to an output stream. */
    friend ostream& operator<< ( ostream&, const jMat3& );

    //! Computes the matrix with out the translation component.
    inline jMat3 rot () const;

    //! Return the ij'th (row,column) component of the matrix.
    jFlt ij( int i, int j ) const { return m[i][j]; }

    //! Sets the ij'th component of the matrix
    void ij( int i, int j, jFlt e ) { m[i][j] = e; }

    /*! Sets the column to the given vector, the homogenous coordinate is
     supplied. */
    void column ( jUSInt, const jVec2&, jFlt );

    //! Computes the determinant.
    jFlt determinant() const;

    //! Computes the adjoint matrix.
    jMat3 adjoint() const;

    //! Computes the inverse matrix, aborts on a singular matrix.
    jMat3 inverse() const;

    //! Computes the transpose of the matrix.
    jMat3 transpose() const;

    //! Tests to see if this matrix is the identity matrix.
    bool isidentity() const
        {
            return ((m[0][0]==1) && (m[0][1]==0) && (m[0][2]==0) &&
                    (m[1][0]==0) && (m[1][1]==1) && (m[1][2]==0) &&
                    (m[2][0]==0) && (m[2][1]==0) && (m[2][2]==1));
        }

    // B-Spline basis
  //    static const jMat3 Mbs;

    // Inverse of the B-Spline basis.
  //    static const jMat3 MbsI;

    // Catmull-Rom basis
  //    static const jMat3 Mcr;

    // Bezier basis
  //    static const jMat3 Mb;

private:

    jFlt m[3][3];

    void identity ()
    {
        m[0][0]=1,m[0][1]=0,m[0][2]=0;
        m[1][0]=0,m[1][1]=1,m[1][2]=0;
        m[2][0]=0,m[2][1]=0,m[2][2]=1;
    }
};

//! Pre-multiplication for scalar multiplication of a vector.
inline jVec3 operator* ( jFlt, const jVec3& );

//! Pre-multiplication for scalar multiplication of a matrix.
inline jMat4 operator* ( jFlt, const jMat4& );

//! Pre-multiplication for scalar multiplication of a vector.
inline jVec2 operator* ( jFlt, const jVec2& );

//! Pre-multiplication for scalar multiplication of a matrix.
inline jMat3 operator* ( jFlt, const jMat3& );

/*!
  \class jMat3Build linearalgebra.h
  \brief Build a 3D transformation matrix.

  This allows someone to easily compose a 3D transformation matrix
  without having to specify the individual matrices themselves. */
class jMat3Build
{
public:
    /*! Create a 3D matrix stack, initially the identity matrix. */
    jMat3Build();
    ~jMat3Build();

    //! Sets the current matrix to the identity matrix.
    void identity();

    /*! Scale the current matrix. */
    void scale( jFlt, jFlt );

    /*! Transform the current matrix. */
    void translate( jFlt, jFlt );

    /*! Rotate the current matrix. */
    void rotate( jFlt );

    /*! Push the current matrix onto the stack. */
    void push();

    /*! Pop the top of the stack into the current matrix, no change if stack
      is empty. */
    void pop();

    operator const jMat3& () const { return T; }

private:

    struct m_node
    {
        m_node( jMat3 _T, m_node* _n )
                : T(_T), n(_n)
            {};
        jMat3 T;
        m_node* n;
    }
    * stack;

    jMat3 T;
};




// -*-c++-*-
//


// jVec3::jVec3
//
// Initialize the vector to the linear interpolation of the two vectors
// by the scalar.
jVec3::jVec3 ( const jVec3& u, const jVec3& v, jFlt k )
{
    x = u.x+k*(v.x-u.x);
    y = u.y+k*(v.y-u.y);
    z = u.z+k*(v.z-u.z);
}

// jVec3::operator^
//
// Compute the cross product of the given vectors.
//
// Arguments
//      (*this)
//      v       the two vectors for the cross product
//
// Return
//      jVec3 containing the cross product of (*this) and v.
jVec3 jVec3::operator^ ( const jVec3& v ) const
{
    return jVec3(y*v.z - z*v.y,
                  z*v.x - x*v.z,
                  x*v.y - y*v.x);
}

// jVec3::operator*
//
// Compute the dot product between the given vectors.
//
// Arguments
//      (*this)
//      v       the two vector for the dot product
//
// Return
//      Scalar containing the dot product of (*this) and v.
jFlt jVec3::operator* ( const jVec3& v ) const
{
    return x*v.x + y*v.y + z*v.z;
}

inline jFlt dot(const jVec3& a, const jVec3& b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline jVec3 cross(const jVec3& a, const jVec3& b)
{
    return a^b;
}

inline jVec3 operator- (const jVec3& a)
{
    return jVec3(-a.x,-a.y,-a.z);
}

void jVec3 ::operator*= ( jFlt k )
{
    x *= k, y*= k, z *= k;
}

void jVec3::operator+= ( const jVec3 &v )
{
  x += v.x;
  y += v.y;
  z += v.z;
}

// jVec3::operator*
//
// Scale the given vector by the scalar
//
// Arguments
//      (*this) the vector to scale
//      k       the scalar to multiply the vector by
//
// Return
//      jVec3 equal to (*this) scaled by k
jVec3 jVec3::operator* ( jFlt k ) const
{
    return jVec3(x*k,y*k,z*k);
}

jVec3 jVec3::operator/ ( jFlt k ) const
{
    return jVec3(x/k,y/k,z/k);
}

// operator*
//
// Pre multiplication of a vector by a
//
// Arguments
//      (*this) the column vector
//      M       the matrix to multiply
//
// Returns
//      The vector (*this)*M

jVec3 jVec3::operator* ( const jMat4& M ) const
{
    return jVec3(x*M.ij(0,0)+y*M.ij(1,0)+z*M.ij(2,0)+M.ij(3,0),
                  x*M.ij(0,1)+y*M.ij(1,1)+z*M.ij(2,1)+M.ij(3,1),
                  x*M.ij(0,2)+y*M.ij(1,2)+z*M.ij(2,2)+M.ij(3,2));
}
jVec3 operator* ( jFlt k, const jVec3& v ) { return v*k; }

void jVec3::operator *= ( const jMat4& M )
{
    *this = *this*M;
}

// jVec3::operator+
//
// Addition of two vectors.
//
// Arguments
//      (*this)
//      v       operands of the addition
//
// Returns
//      the jVec3 (*this)+v
jVec3 jVec3::operator+ ( const jVec3& v ) const
{
    return jVec3(x+v.x,y+v.y,z+v.z);
}

// jVec3::operator-
//
// Subtraction of two vectors
//
// Arguments
//      (*this)
//      v       operands of the subtraction
//
// Returns
//      the jVec3 (*this)-v
jVec3 jVec3::operator- ( const jVec3& v ) const
{
    return jVec3(x-v.x,y-v.y,z-v.z);
}

// jVec3::operator==
//
// Comparison of vectors
bool jVec3::operator== ( const jVec3& v ) const
{
    return x==v.x && y==v.y && z==v.z;
}

// jVec3::operator==
//
// Comparison of vectors
bool jVec3::operator!= ( const jVec3& v ) const
{
    return !(*this==v);
}

// jVec3::length
//
// Computes the length of the vector.
//
// Return
//      The scalar length of the vector.
jFlt jVec3::length () const
{
    return sqrt(x*x+y*y+z*z);
}

jFlt jVec3::slength () const
{
    return x*x+y*y+z*z;
}

jFlt jVec3::sqlen () const
{
    return x*x+y*y+z*z;
}

// operator[]
//
// Index into the vector
//
// Arguments
//      i       index into the vector
//
// Return
//      The scalar for the index
jFlt& jVec3::operator[] ( int i )
{
    return i ? (i==1 ? y : z) : x;
}
jFlt  jVec3::operator[] ( int i ) const
{
    return i ? (i==1 ? y : z) : x;
}

jFlt jVec3::ldistance( jFlt l ) const
{
    return l==2 ? length()
        : pow(fltabs(pow(x,l)) + fltabs(pow(y,l)) + fltabs(pow(z,l)),1.0/l);
}

//-----------------------------------------------------------------------------
// jNorm3

// jNorm3::jNorm3/0
//
// Default constructor, initializes to 0,1,0
jNorm3::jNorm3 ()

    : jVec3(0,1,0)
{}

// jNorm3::jNorm3/1
//
// Initializes to the given vector and ensures unit length
jNorm3::jNorm3 ( const jVec3& v, bool n )

    : jVec3(v)
{
    if(n)normalize();
}

// jNorm3::jNorm3/3
//
// Component initialization
jNorm3::jNorm3 ( jFlt _x, jFlt _y, jFlt _z, bool n )

    : jVec3(_x,_y,_z)
{
    if(n)normalize();
}

// jNorm3::operator[]
//
// Reference access to the members of a normal vector is not allowed,
// the user can only get the values using indexing.
jFlt jNorm3::operator[] ( int i ) const
{
    return jFlt(jVec3::operator[](i));
}

// jNorm3::operator=
//
// Assignment must be written to ensure that unit length is preserved.
jNorm3 jNorm3::operator= ( const jVec3& v )
{
    x = v[0], y = v[1], z = v[2];
    normalize();
    return *this;
}

// jNorm3::operator^
//
// Cross product of normals
jNorm3 jNorm3::operator ^ ( const jNorm3& n ) const
{
    jNorm3 t;

    t.x = y*n[2] - z*n[1];
    t.y = z*n[0] - x*n[2];
    t.z = x*n[1] - y*n[0];

    t.normalize();

    return t;
}

jVec3 jNorm3::operator* ( jFlt k ) const
{ return jVec3::operator*(k); }

jVec3 jNorm3::operator+ ( const jVec3& v ) const
{ return jVec3::operator+(v); }

jVec3 jNorm3::operator- ( const jVec3& v ) const
{ return jVec3::operator-(v); }

jFlt   jNorm3::operator* ( const jVec3& v ) const
{ return jVec3::operator*(v); }

jVec3 jNorm3::operator^ ( const jVec3& v ) const
{ return jVec3::operator^(v); }

void jNorm3::operator*= ( jFlt k )
{
    x*=k,y*=k,z*=k;
    if(!(k==1.0 || k==-1.0))normalize();
}

// jVec2::jVec2
//
// Initialize the vector to the linear tion of the two vectors
// by the scalar.
jVec2::jVec2 ( const jVec2& u, const jVec2& v, jFlt k )
{
    x = u.x+k*(v.x-u.x);
    y = u.y+k*(v.y-u.y);
}

// jVec2::operator*
//
// Compute the dot product between the given vectors.
//
// Arguments
//      (*this)
//      v       the two vector for the dot product
//
// Return
//      Scalar containing the dot product of (*this) and v.
jFlt jVec2::operator* ( const jVec2& v ) const
{
    return x*v.x + y*v.y;
}
void jVec2 ::operator*= ( jFlt k )
{
    x *= k, y*= k;
}

// jVec2::operator*
//
// Scale the given vector by the scalar
//
// Arguments
//      (*this) the vector to scale
//      k       the scalar to multiply the vector by
//
// Return
//      jVec2 equal to (*this) scaled by k
jVec2 jVec2::operator* ( jFlt k ) const
{
    return jVec2(x*k,y*k);
}

// operator*
//
// Pre multiplication of a vector by a
//
// Arguments
//      (*this) the column vector
//      M       the matrix to multiply
//
// Returns
//      The vector (*this)*M

jVec2 jVec2::operator* ( const jMat3& M ) const
{
    return jVec2(x*M.ij(0,0)+y*M.ij(0,1)+M.ij(0,2), // vector x,y,1
                  x*M.ij(1,0)+y*M.ij(1,1)+M.ij(1,2));  // this one changed
}
jVec2 operator* ( jFlt k, const jVec2& v ) { return v*k; }

void jVec2::operator *= ( const jMat3& M )
{
    *this = *this*M;
}

// jVec2::operator+
//
// Addition of two vectors.
//
// Arguments
//      (*this)
//      v       operands of the addition
//
// Returns
//      the jVec2 (*this)+v
jVec2 jVec2::operator+ ( const jVec2& v ) const
{
    return jVec2(x+v.x,y+v.y);
}

// jVec2::operator-
//
// Subtraction of two vectors
//
// Arguments
//      (*this)
//      v       operands of the subtraction
//
// Returns
//      the jVec2 (*this)-v
jVec2 jVec2::operator- ( const jVec2& v ) const
{
    return jVec2(x-v.x,y-v.y);
}

// jVec2::operator==
//
// Comparison of vectors
bool jVec2::operator== ( const jVec2& v ) const
{
    return x==v.x && y==v.y;
}

// jVec2::operator==
//
// Comparison of vectors
bool jVec2::operator!= ( const jVec2& v ) const
{
    return !(*this==v);
}

// jVec2::length
//
// Computes the length of the vector.
//
// Return
//      The scalar length of the vector.
jFlt jVec2::length () const
{
    return sqrt(x*x+y*y);
}
jFlt jVec2::slength () const
{
    return x*x+y*y;
}

// operator[]
//
// Index into the vector
//
// Arguments
//      i       index into the vector
//
// Return
//      The scalar for the index
jFlt& jVec2::operator[] ( int i )
{
    return i ? y : x;
}
jFlt  jVec2::operator[] ( int i ) const
{
    return i ? y : x;
}

// jMat4::jMat4
//
// Initializes a matrix to either a null or identity matrix.
//
// Arguments
//      type    the type of matrix to initialize to
jMat4::jMat4 ( Type )
{
    identity();
}

// jMat4::jMat4
//
// Initializes a matrix to a rotation matrix
//
// Arguments
//      type    the type of matrix to initialize to
//      k       the degrees in radians to rotate by
jMat4::jMat4 ( Type type, jFlt a )
{
    identity();

    switch(type)
    {
        case mTranslate :
            m[3][0] = m[3][1] = m[3][2] = a;
            break;
        case mScale :
            m[0][0] = m[1][1] = m[2][2] = a;
            break;
        case mRotateX :
            m[1][1] = m[2][2] = cos(a);
            m[2][1] = m[1][2] = sin(a);
            m[2][1] *= -1;
            break;
        case mRotateY :
            m[0][0] = m[2][2] = cos(a);
            m[2][0] = m[0][2] = sin(a);
            m[2][0] *= -1;
            break;
        case mRotateZ :
            m[1][1] = m[0][0] = cos(a);
            m[1][0] = m[0][1] = sin(a);
            m[1][0] *= -1;
            break;
        default:   break;

           /* case mNull:
            case mIdentity,
            mRotateA, mRotateX, mRotateY, mRotateZ,
            mScale, mTranslate
            */
    }
}

// jMat4::jMat4
//
// Initializes a matrix to an identity scale or translate matrix
//
// Arguments
//      type    the type of matrix to initialize to
//      x
//      y
//      z       the components of the vector to use for the type of matrix
jMat4::jMat4 ( Type type, jFlt x, jFlt y, jFlt z )
{
    identity();

    switch(type)
    {
        case mTranslate :
            m[3][0]=x,m[3][1]=y,m[3][2]=z;
            break;
        case mScale :
            m[0][0]=x,m[1][1]=y,m[2][2]=z;
            break;
        default:   break;
    }
}

jMat4::jMat4( Type type, jNorm3 u, jFlt theta )
{
    if(type!=mRotateA)
    {
        cerr << "Ambiguous matrix type, abort.\n";
        abort();
    }

    identity();

    jFlt x2 = u[0]*u[0];
    jFlt y2 = u[1]*u[1];
    jFlt z2 = u[2]*u[2];
    jFlt xy = u[0]*u[1];
    jFlt xz = u[0]*u[2];
    jFlt yz = u[1]*u[2];
    jFlt cost = cos(theta);
    jFlt sint = sin(theta);

    column(0,
           jVec3(x2 + cost*(1-x2),
                 xy*(1-cost)-u[2]*sint,
                 xz*(1-cost)-u[1]*sint),
           0);
    column(1,
           jVec3(xy*(1-cost)+u[2]*sint,
                 y2+cost*(1-y2),
                 yz*(1-cost)-u[0]*sint),
           0);
    column(2,
           jVec3(xz*(1-cost)+u[1]*sint,
                 yz*(1-cost)+u[0]*sint,
                 z2+cost*(1-z2)),
           0);
    column(3,
           jVec3(0,0,0),
           1);


}

// jMat4::rot
//
// Computes the matrix eqeuivelent to the argument without the translations.
//
// Arguments
//      M       The matrix to compute the rotation matrix for.
jMat4 jMat4::rot () const
{
    jMat4 R(*this);

    R.m[3][0] = R.m[3][1] = R.m[3][2] = 0;

    return R;
}

jMat4 jMat4::operator* (jFlt k) const
{
    jMat4 t;

    t.ij(0,0,ij(0,0)*k);
    t.ij(0,1,ij(0,1)*k);
    t.ij(0,2,ij(0,2)*k);
    t.ij(0,3,ij(0,3)*k);
    t.ij(1,0,ij(1,0)*k);
    t.ij(1,1,ij(1,1)*k);
    t.ij(1,2,ij(1,2)*k);
    t.ij(1,3,ij(1,3)*k);
    t.ij(2,0,ij(2,0)*k);
    t.ij(2,1,ij(2,1)*k);
    t.ij(2,2,ij(2,2)*k);
    t.ij(2,3,ij(2,3)*k);
    t.ij(3,0,ij(3,0)*k);
    t.ij(3,1,ij(3,1)*k);
    t.ij(3,2,ij(3,2)*k);
    t.ij(3,3,ij(3,3)*k);

    return t;
}

jMat4 jMat4::operator+ (const jMat4& b) const
{
    jMat4 t;

    t.ij(0,0,ij(0,0)+b.ij(0,0));
    t.ij(0,1,ij(0,1)+b.ij(0,1));
    t.ij(0,2,ij(0,2)+b.ij(0,2));
    t.ij(0,3,ij(0,3)+b.ij(0,3));

    t.ij(1,0,ij(1,0)+b.ij(1,0));
    t.ij(1,1,ij(1,1)+b.ij(1,1));
    t.ij(1,2,ij(1,2)+b.ij(1,2));
    t.ij(1,3,ij(1,3)+b.ij(1,3));

    t.ij(2,0,ij(2,0)+b.ij(2,0));
    t.ij(2,1,ij(2,1)+b.ij(2,1));
    t.ij(2,2,ij(2,2)+b.ij(2,2));
    t.ij(2,3,ij(2,3)+b.ij(2,3));

    t.ij(3,0,ij(3,0)+b.ij(3,0));
    t.ij(3,1,ij(3,1)+b.ij(3,1));
    t.ij(3,2,ij(3,2)+b.ij(3,2));
    t.ij(3,3,ij(3,3)+b.ij(3,3));

    return t;
}



// jMat4::operator*
//
// Multiplication of two matrices
//
// Arguments
//      A
//      B       the matrices
//
// Return
//      The matrix that is the product of A and B
jMat4 jMat4::operator* ( const jMat4& B ) const
{
    jMat4 r;

    r.m[0][0] =
        m[0][0]*B.m[0][0] +
        m[0][1]*B.m[1][0] +
        m[0][2]*B.m[2][0] +
        m[0][3]*B.m[3][0];

    r.m[0][1] =
        m[0][0]*B.m[0][1] +
        m[0][1]*B.m[1][1] +
        m[0][2]*B.m[2][1] +
        m[0][3]*B.m[3][1];

    r.m[0][2] =
        m[0][0]*B.m[0][2] +
        m[0][1]*B.m[1][2] +
        m[0][2]*B.m[2][2] +
        m[0][3]*B.m[3][2];

    r.m[0][3] =
        m[0][0]*B.m[0][3] +
        m[0][1]*B.m[1][3] +
        m[0][2]*B.m[2][3] +
        m[0][3]*B.m[3][3];

    r.m[1][0] =
        m[1][0]*B.m[0][0] +
        m[1][1]*B.m[1][0] +
        m[1][2]*B.m[2][0] +
        m[1][3]*B.m[3][0];

    r.m[1][1] =
        m[1][0]*B.m[0][1] +
        m[1][1]*B.m[1][1] +
        m[1][2]*B.m[2][1] +
        m[1][3]*B.m[3][1];

    r.m[1][2] =
        m[1][0]*B.m[0][2] +
        m[1][1]*B.m[1][2] +
        m[1][2]*B.m[2][2] +
        m[1][3]*B.m[3][2];

    r.m[1][3] =
        m[1][0]*B.m[0][3] +
        m[1][1]*B.m[1][3] +
        m[1][2]*B.m[2][3] +
        m[1][3]*B.m[3][3];

    r.m[2][0] =
        m[2][0]*B.m[0][0] +
        m[2][1]*B.m[1][0] +
        m[2][2]*B.m[2][0] +
        m[2][3]*B.m[3][0];

    r.m[2][1] =
        m[2][0]*B.m[0][1] +
        m[2][1]*B.m[1][1] +
        m[2][2]*B.m[2][1] +
        m[2][3]*B.m[3][1];

    r.m[2][2] =
        m[2][0]*B.m[0][2] +
        m[2][1]*B.m[1][2] +
        m[2][2]*B.m[2][2] +
        m[2][3]*B.m[3][2];

    r.m[2][3] =
        m[2][0]*B.m[0][3] +
        m[2][1]*B.m[1][3] +
        m[2][2]*B.m[2][3] +
        m[2][3]*B.m[3][3];

    r.m[3][0] =
        m[3][0]*B.m[0][0] +
        m[3][1]*B.m[1][0] +
        m[3][2]*B.m[2][0] +
        m[3][3]*B.m[3][0];

    r.m[3][1] =
        m[3][0]*B.m[0][1] +
        m[3][1]*B.m[1][1] +
        m[3][2]*B.m[2][1] +
        m[3][3]*B.m[3][1];

    r.m[3][2] =
        m[3][0]*B.m[0][2] +
        m[3][1]*B.m[1][2] +
        m[3][2]*B.m[2][2] +
        m[3][3]*B.m[3][2];

    r.m[3][3] =
        m[3][0]*B.m[0][3] +
        m[3][1]*B.m[1][3] +
        m[3][2]*B.m[2][3] +
        m[3][3]*B.m[3][3];

    return r;
}


// jMat3::jMat3
//
// Initializes a matrix to either a null or identity matrix.
//
// Arguments
//      type    the type of matrix to initialize to
jMat3::jMat3 ( Type )
{
    identity();
}

// jMat3::jMat3
//
// Initializes a matrix to a rotation matrix
//
// Arguments
//      type    the type of matrix to initialize to
//      k       the degrees in radians to rotate by
jMat3::jMat3 ( Type type, jFlt a )
{
    identity();

    switch(type)    // this routine changed
    {
        case mTranslate :
            m[0][2] = m[1][2] = a;
            break;
        case mScale :
            m[0][0] = m[1][1] = a;
            break;
        case mRotate :
            m[1][1] = m[0][0] = cos(a);
            m[1][0] = m[0][1] = sin(a);
            m[0][1] *= -1;
            break;
       default:   break;
    }
}

// jMat3::jMat3
//
// Initializes a matrix to an identity scale or translate matrix
//
// Arguments
//      type    the type of matrix to initialize to
//      x
//      y
jMat3::jMat3 ( Type type, jFlt x, jFlt y)
{
    identity();

    switch(type)
    {
        case mTranslate :
            m[0][2]=x,m[1][2]=y;   // changed
            break;
        case mScale :
            m[0][0]=x,m[1][1]=y;
            break;
        default:   break;
    }
}

// jMat3::rot
//
// Computes the matrix eqeuivelent to the argument without the translations.
//
// Arguments
//      M       The matrix to compute the rotation matrix for.
jMat3 jMat3::rot () const
{
    jMat3 R(*this);

    R.m[0][2] = R.m[1][2] = 0;  // changed  clearly wrong I don't know what this is meant to do!
    cerr << "badbadbadbad+++++++++++++++++++++++ rot +++++++++++++++++ \n";
	exit( -1);

	return R;
}

// Arguments
//      type    mbetween
//      pick up by two points a,b and put down on c,d
//      figures out the matrix
//
//      type    the type of matrix to initialize to
//      x
//      y
jMat3::jMat3 ( Type type, jVec2 a, jVec2 b, jVec2 c, jVec2 d)
{
    identity();
	if (type != mBetween) cerr << "Must be mBetween type\n";
	else {
		jFlt scalef;
		jVec2 trn;
		jFlt angle;
		jVec2 q,s,t;

		scalef = (d-c).length()/(b-a).length();
		trn=(c-a);
		q = b+(c-a);
		s=(d-c);
		t=(q-c);
			// angle between cd and cq
		if (nearZero(s*t)) {
			cerr << "Between not worked - zero length\n";
			return;
		} else {
		angle = acos( (s*t)/(s.length()*t.length()) );
		m[1][1] = m[0][0] = cos(angle);
		m[1][0] = m[0][1] = sin(angle);
        m[0][1] *= -1; // rotate done

		m[0][2]=trn[0];
		m[1][2]=trn[1]; // trans
		m[0][0]*=scalef;
		m[1][1]*=scalef; // scale
		}

    }
}


// jMat3::operator*
//
// Multiplication of two matrices
//
// Arguments
//      A
//      B       the matrices
//
// Return
//      The matrix that is the product of A and B
jMat3 jMat3::operator* ( const jMat3& B ) const
{
    jMat3 r;

    r.m[0][0] =
        m[0][0]*B.m[0][0] +
        m[0][1]*B.m[1][0] +
        m[0][2]*B.m[2][0];

    r.m[0][1] =
        m[0][0]*B.m[0][1] +
        m[0][1]*B.m[1][1] +
        m[0][2]*B.m[2][1];

    r.m[0][2] =
        m[0][0]*B.m[0][2] +
        m[0][1]*B.m[1][2] +
        m[0][2]*B.m[2][2];

    r.m[1][0] =
        m[1][0]*B.m[0][0] +
        m[1][1]*B.m[1][0] +
        m[1][2]*B.m[2][0];

    r.m[1][1] =
        m[1][0]*B.m[0][1] +
        m[1][1]*B.m[1][1] +
        m[1][2]*B.m[2][1];

    r.m[1][2] =
        m[1][0]*B.m[0][2] +
        m[1][1]*B.m[1][2] +
        m[1][2]*B.m[2][2];

    r.m[2][0] =
        m[2][0]*B.m[0][0] +
        m[2][1]*B.m[1][0] +
        m[2][2]*B.m[2][0];

    r.m[2][1] =
        m[2][0]*B.m[0][1] +
        m[2][1]*B.m[1][1] +
        m[2][2]*B.m[2][1];

    r.m[2][2] =
        m[2][0]*B.m[0][2] +
        m[2][1]*B.m[1][2] +
        m[2][2]*B.m[2][2];

    return r;
}


// jNorm3::operator*
//
// Post multiplication by a transformation matrix, this operation is
// overridden to ensure that transformations involving translation is
// refused and after a scale the vector is returned to unit length.
jNorm3 jNorm3::operator* ( const jMat4& T ) const
{
    return jNorm3(jVec3::operator*(T.rot()));
}


jNorm3 jNorm3::negation() const
{
    return jNorm3(-x,-y,-z,false);
}

const jNorm3& jNorm3::negate()
{
    x = -x, y = -y, z = -z;
    return *this;
}

// jNorm3::reflect
//
// Compute and return the vector that is the reflection about the given
// normal vector.
jNorm3 jNorm3::reflect ( const jNorm3& N, jFlt dp ) const
{
    dp = dp>1.0 ? N*(*this) : dp;
    return jNorm3((2*dp)*N-(*this));
}

// refract
//
// Computes the refraction vector about the given normal.
//
// Arguments
//          the normal vector to refract about
//          the index of refraction of the medium we are leaving
//          the index of refraction of the medium we are entering
//
// Returns
//          the computed refraction vector
bool jNorm3:: refract( const jNorm3& N, jFlt n1, jFlt n2, jNorm3& T ) const
{
    bool ret = true;

    jFlt NdI = N*(*this);
    jFlt nr = n1/n2;

    jFlt sr = 1.0 - nr*nr*(1-NdI*NdI);

    if(sr<0.0)ret = false;
    else
        T = (nr*NdI-sqrt(sr))*N-nr*(*this);

    return ret;
}

//#include "linearalgebra.inl"

/*!
  \class jVec3 linearalgebra.h
  \brief Definition of a 3D point/vector class.

  This class is used throughout the library as a point or vector class.  It
  is not a homogeneous coordinate class per se.  An implied homogenous
  coordinate of 1 is added for affine transformations (see the jMat4 class).
*/
class jVec4
{
public:

    //! Initializes the vector to the specified components
    inline jVec4( jFlt _x = 0, jFlt _y = 0, jFlt _z = 0, jFlt _h = 0 )
            : x(_x), y(_y), z(_z), h(_h) {};

    //! Transformation of the position by an affine transformation.
    inline jVec4 operator* ( const jMat4& ) const;

    //! Sets this vector to the transformation by an affine transformation.
    inline void operator*= ( const jMat4& );

    //! Index into the vector.
    inline jFlt& operator[] ( int );

    //! \overload
    inline jFlt  operator[] ( int ) const;

protected:

    jFlt x,y,z,h;
};

jVec4 jVec4::operator* ( const jMat4& M ) const
{
    return jVec4(x*M.ij(0,0)+y*M.ij(1,0)+z*M.ij(2,0)+h*M.ij(3,0),
                  x*M.ij(0,1)+y*M.ij(1,1)+z*M.ij(2,1)+h*M.ij(3,1),
                  x*M.ij(0,2)+y*M.ij(1,2)+z*M.ij(2,2)+h*M.ij(3,2),
                  x*M.ij(0,3)+y*M.ij(1,3)+z*M.ij(2,3)+h*M.ij(3,3));
}

void jVec4::operator *= ( const jMat4& M )
{
    *this = *this*M;
}


jFlt& jVec4::operator[] ( int i )
{
  switch(i){
    case(0): return x;
    case(1): return y;
    case(2): return z;
    case(3): return h;
  }
}
jFlt  jVec4::operator[] ( int i ) const
{
  switch(i){
    case(0): return x;
    case(1): return y;
    case(2): return z;
    case(3): return h;
  }
}


#endif
