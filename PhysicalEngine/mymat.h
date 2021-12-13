#ifndef MYMAT_H
#define MYMAT_H

#include <cmath> // abs, sqrt
#include <cassert> // assert
#include <algorithm> // max, min
#include <QVector2D>


const float PI = 3.141592741f;
const float EPSILON = 0.0001f;

//  void Rotate( float radians )
//  {
//    float c = std::cos( radians );
//    float s = std::sin( radians );

//    float xp = x * c - y * s;
//    float yp = x * s + y * c;

//    x = xp;
//    y = yp;
//  }


struct Mat2{
  union
  {
    struct
    {
      float m00, m01;
      float m10, m11;
    };

    float m[2][2];
    float v[4];
  };

  Mat2() {}
  Mat2(float radians ){
    float c = std::cos( radians );
    float s = std::sin( radians );

    m00 = c; m01 = -s;
    m10 = s; m11 =  c;
  }

  Mat2( float a, float b, float c, float d )
    : m00( a ), m01( b )
    , m10( c ), m11( d )
  {}

  void Set( float radians ){
    float c = std::cos( radians );
    float s = std::sin( radians );

    m00 = c; m01 = -s;
    m10 = s; m11 =  c;
  }

  Mat2 Abs( void ) const{
    return Mat2( std::abs( m00 ), std::abs( m01 ), std::abs( m10 ), std::abs( m11 ) );
  }

  QVector2D AxisX( void ) const{
    return QVector2D( m00, m10 );
  }

  QVector2D AxisY( void ) const{
    return QVector2D( m01, m11 );
  }

  Mat2 Transpose( void ) const{
    return Mat2( m00, m10, m01, m11 );
  }

  const QVector2D operator*( const QVector2D& rhs ) const{
    return QVector2D( m00 * rhs.x() + m01 * rhs.y(), m10 * rhs.x() + m11 * rhs.y() );
  }

  const Mat2 operator*( const Mat2& rhs ) const{
    // [00 01]  [00 01]
    // [10 11]  [10 11]

    return Mat2(
      m[0][0] * rhs.m[0][0] + m[0][1] * rhs.m[1][0],
      m[0][0] * rhs.m[0][1] + m[0][1] * rhs.m[1][1],
      m[1][0] * rhs.m[0][0] + m[1][1] * rhs.m[1][0],
      m[1][0] * rhs.m[0][1] + m[1][1] * rhs.m[1][1]
    );
  }
};




// Comparison with tolerance of EPSILON
inline bool Equal( float a, float b )
{
  // <= instead of < for NaN comparison safety
  return std::abs( a - b ) <= EPSILON;
}

inline float Sqr( float a )
{
  return a * a;
}

inline float Clamp( float min, float max, float a )
{
  if (a < min) return min;
  if (a > max) return max;
  return a;
}

inline int Round( float a )
{
  return (int)(a + 0.5f);
}

inline float Random( float l, float h )
{
  float a = (float)rand( );
  a /= RAND_MAX;
  a = (h - l) * a + l;
  return a;
}

inline bool BiasGreaterThan( float a, float b )
{
  const float k_biasRelative = 0.95f;
  const float k_biasAbsolute = 0.01f;
  return a >= b * k_biasRelative + a * k_biasAbsolute;
}

//const f32 gravityScale = 5.0f;
//const QVector2D gravity( 0, 10.0f * gravityScale );
//const float dt = 1.0f / 60.0f;

#endif // MYMAT_H
