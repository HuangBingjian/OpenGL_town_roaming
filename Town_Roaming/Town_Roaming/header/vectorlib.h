#ifndef __VECTORLIB_H_INCLUDED__
#define __VECTORLIB_H_INCLUDED__

#include <math.h>


/*************************** Macros and constants ***************************/
// returns a number ranging from -1.0 to 1.0
#define FRAND   (((float)rand()-(float)rand())/RAND_MAX)
#define Clamp(x, min, max)  x = (x<min  ? min : x<max ? x : max);

#define SQUARE(x)  (x)*(x)


struct vector3_t
{
  vector3_t(float x, float y, float z) : x(x), y(y), z(z) {}
  vector3_t(const vector3_t &v) : x(v.x), y(v.y), z(v.z) {}
  vector3_t() : x(0.0f), y(0.0f), z(0.0f) {}

  vector3_t& operator=(const vector3_t &rhs)
  {
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    return *this;
  }

  // vector add
  vector3_t operator+(const vector3_t &rhs) const
  {
    return vector3_t(x + rhs.x, y + rhs.y, z + rhs.z);
  }

  // vector subtract
  vector3_t operator-(const vector3_t &rhs) const
  {
    return vector3_t(x - rhs.x, y - rhs.y, z - rhs.z);
  }

  // scalar multiplication
  vector3_t operator*(const float scalar) const
  {
    return vector3_t(x * scalar, y * scalar, z * scalar);
  }

  // dot product
  float operator*(const vector3_t &rhs) const
  {
    return x * rhs.x + y * rhs.y + z * rhs.z;
  }

  // cross product
  vector3_t operator^(const vector3_t &rhs) const
  {
    return vector3_t(y * rhs.z - rhs.y * z, rhs.x * z - x * rhs.z, x * rhs.y - rhs.x * y);
  }

  float& operator[](int index)
  {
    return v[index];
  }

  float Length()
  {
    float length = (float)sqrt(SQUARE(x) + SQUARE(y) + SQUARE(z));
    return (length != 0.0f) ? length : 1.0f;
  }

/*****************************************************************************
 Normalize()

 Helper function to normalize vectors
*****************************************************************************/
  vector3_t Normalize()
  {
    *this = *this * (1.0f/Length());
    return *this;
  }

  union
  {
    struct
    {
      float x;
      float y;
      float z;
    };
    float v[3];
  };
};


#endif __VECTORLIB_H_INCLUDED__