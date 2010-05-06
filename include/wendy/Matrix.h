///////////////////////////////////////////////////////////////////////
// Wendy core library
// Copyright (c) 2005 Camilla Berglund <elmindreda@elmindreda.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any
// damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any
// purpose, including commercial applications, and to alter it and
// redistribute it freely, subject to the following restrictions:
//
//  1. The origin of this software must not be misrepresented; you
//     must not claim that you wrote the original software. If you use
//     this software in a product, an acknowledgment in the product
//     documentation would be appreciated but is not required.
//
//  2. Altered source versions must be plainly marked as such, and
//     must not be misrepresented as being the original software.
//
//  3. This notice may not be removed or altered from any source
//     distribution.
//
///////////////////////////////////////////////////////////////////////
#ifndef WENDY_MATRIX_H
#define WENDY_MATRIX_H
///////////////////////////////////////////////////////////////////////

#include <stack>

///////////////////////////////////////////////////////////////////////

namespace wendy
{

///////////////////////////////////////////////////////////////////////

class Quat;

///////////////////////////////////////////////////////////////////////

class Mat2
{
public:
  Mat2(void);
  Mat2(const Vec2& sx, const Vec2& sy);
  explicit Mat2(const String& string);
  void invert(void);
  void transpose(void);
  void normalize(void);
  void rotateVector(Vec2& vector) const;
  float determinant(void) const;
  operator float* (void);
  operator const float* (void) const;
  float operator () (unsigned int column, unsigned int row) const;
  float& operator () (unsigned int column, unsigned int row);
  Mat2 operator + (const Mat2& matrix) const;
  Mat2 operator - (const Mat2& matrix) const;
  Mat2 operator * (const Mat2& matrix) const;
  Mat2& operator += (const Mat2& matrix);
  Mat2& operator -= (const Mat2& matrix);
  Mat2& operator *= (const Mat2& matrix);
  bool operator == (const Mat2& matrix) const;
  bool operator != (const Mat2& matrix) const;
  String asString(void) const;
  void setIdentity(void);
  void set(const Vec2& sx, const Vec2& sy);
  void setRotation(const float angle);
  Vec2 x;
  Vec2 y;
};

///////////////////////////////////////////////////////////////////////

class Mat3
{
public:
  Mat3(void);
  Mat3(const Vec3& sx, const Vec3& sy, const Vec3& sz);
  explicit Mat3(const String& string);
  void invert(void);
  void transpose(void);
  void normalize(void);
  void rotateVector(Vec3& vector) const;
  float determinant(void) const;
  operator float* (void);
  operator const float* (void) const;
  float operator () (unsigned int column, unsigned int row) const;
  float& operator () (unsigned int column, unsigned int row);
  Mat3 operator + (const Mat3& matrix) const;
  Mat3 operator - (const Mat3& matrix) const;
  Mat3 operator * (const Mat3& matrix) const;
  Mat3& operator += (const Mat3& matrix);
  Mat3& operator -= (const Mat3& matrix);
  Mat3& operator *= (const Mat3& matrix);
  bool operator == (const Mat3& matrix) const;
  bool operator != (const Mat3& matrix) const;
  String asString(void) const;
  void setIdentity(void);
  void set(const Vec3& sx, const Vec3& sy, const Vec3& sz);
  void setQuatRotation(const Quat& quat);
  void setEulerRotation(const Vec3& angles);
  void setVectorRotation(const Vec3& vector);
  void setAxisRotation(const Vec3& axis, float angle);
  Vec3 x;
  Vec3 y;
  Vec3 z;
};

///////////////////////////////////////////////////////////////////////

class Mat4
{
public:
  Mat4(void);
  Mat4(const Vec4& sx, const Vec4& sy, const Vec4& sz, const Vec4& sw);
  explicit Mat4(const String& string);
  void invert(void);
  void transpose(void);
  void normalize(void);
  void transformVector(Vec3& vector) const;
  void transformVector(Vec4& vector) const;
  void rotateVector(Vec3& vector) const;
  void translateVector(Vec3& vector) const;
  operator float* (void);
  operator const float* (void) const;
  float operator () (unsigned int column, unsigned int row) const;
  float& operator () (unsigned int column, unsigned int row);
  Mat4 operator + (const Mat4& matrix) const;
  Mat4 operator - (const Mat4& matrix) const;
  Mat4 operator * (const Mat4& matrix) const;
  Mat4& operator += (const Mat4& matrix);
  Mat4& operator -= (const Mat4& matrix);
  Mat4& operator *= (const Mat4& matrix);
  bool operator == (const Mat4& matrix) const;
  bool operator != (const Mat4& matrix) const;
  String asString(void) const;
  void setIdentity(void);
  void set(const Vec4& sx, const Vec4& sy, const Vec4& sz, const Vec4& sw);
  void setEulerRotation(const Vec3& angles);
  void setQuatRotation(const Quat& quat);
  void setVectorRotation(const Vec3& vector);
  void setAxisRotation(const Vec3& axis, float angle);
  void getMatrixRotation(Mat3& matrix) const;
  void setMatrixRotation(const Mat3& matrix);
  void getScaling(Vec3& scaling) const;
  void setScaling(const Vec3& scaling);
  void getTranslation(Vec3& vector) const;
  void setTranslation(const Vec3& vector);
  void setProjection2D(float width, float height);
  void setProjection3D(float FOV, float aspect, float nearZ, float farZ);
  Vec4 x;
  Vec4 y;
  Vec4 z;
  Vec4 w;
};

///////////////////////////////////////////////////////////////////////

template <typename T>
class MatrixStack
{
public:
  inline MatrixStack(void);
  inline void push(const T& matrix);
  inline void pop(void);
  inline bool isEmpty(void) const;
  inline unsigned int getCount(void) const;
  inline const T& getTop(void) const;
  inline const T& getTotal(void) const;
private:
  struct Entry
  {
    T local;
    T total;
  };
  std::stack<Entry> stack;
};

///////////////////////////////////////////////////////////////////////

typedef MatrixStack<Mat2> MatrixStack2;
typedef MatrixStack<Mat3> MatrixStack3;
typedef MatrixStack<Mat4> MatrixStack4;

///////////////////////////////////////////////////////////////////////

template <typename T>
inline MatrixStack<T>::MatrixStack(void)
{
  Entry entry;
  entry.local.setIdentity();
  entry.total.setIdentity();
  stack.push(entry);
}

template <typename T>
inline void MatrixStack<T>::push(const T& matrix)
{
  Entry entry;
  entry.local = matrix;
  entry.total = stack.top().total;
  entry.total *= matrix;
  stack.push(entry);
}

template <typename T>
inline void MatrixStack<T>::pop(void)
{
  if (stack.size() > 1)
    stack.pop();
}

template <typename T>
inline bool MatrixStack<T>::isEmpty(void) const
{
  return stack.size() == 1;
}

template <typename T>
inline unsigned int MatrixStack<T>::getCount(void) const
{
  return stack.size() - 1;
}

template <typename T>
inline const T& MatrixStack<T>::getTop(void) const
{
  return stack.top().local;
}

template <typename T>
inline const T& MatrixStack<T>::getTotal(void) const
{
  return stack.top().total;
}

///////////////////////////////////////////////////////////////////////

} /*namespace wendy*/

///////////////////////////////////////////////////////////////////////
#endif /*WENDY_MATRIX_H*/
///////////////////////////////////////////////////////////////////////
