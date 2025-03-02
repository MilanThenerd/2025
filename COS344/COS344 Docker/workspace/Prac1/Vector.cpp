#include "Vector.h"

#include <cmath>

template <int n>
Vector<n>::Vector()
{
  arr = new double[n]();
}

template <int n>
Vector<n>::Vector(double *arr)
{
  this->arr = new double[n];
  for (int i = 0; i < n; i++) 
  {
      this->arr[i] = arr[i];
  }
}

template <int n>
Vector<n>::~Vector()
{
  delete[] arr;
}

template <int n>
Vector<n>::Vector(const Vector<n> &other)
{
  arr = new double[n];
  for (int i = 0; i < n; i++) 
  {
      arr[i] = other.arr[i];
  }
}

template <int n>
Vector<n>::Vector(const Matrix<n, 1> &other)
{
  arr = new double[n];
  for (int i = 0; i < n; i++) 
  {
      arr[i] = other[i][0];
  }
}

template <int n>
Vector<n> &Vector<n>::operator=(const Vector<n> &other)
{
  if (this != &other) 
  {
      double* newArr = new double[n];
      for (int i = 0; i < n; i++) 
      {
          newArr[i] = other.arr[i];
      }
      delete[] arr;
      arr = newArr;
  }
  return *this;
}

template <int n>
Vector<n> Vector<n>::operator+(const Vector rhs) const
{
  Vector<n> result;
  for(int i = 0 ; i < n ; i++)
  {
    result.arr[i] = arr[i] + rhs.arr[i];
  }
  return result;
}

template <int n>
Vector<n> Vector<n>::operator-(const Vector<n> rhs) const
{
  Vector<n> result;
  for(int i = 0 ; i < n ; i++)
  {
    result.arr[i] = arr[i] - rhs.arr[i];
  }
  return result;
}

template <int n>
Vector<n> Vector<n>::operator*(double val) const
{
  Vector<n> result;
  for (int i = 0; i < n; i++)
  {
    result.arr[i] = arr[i] * val;
  }
  return result;
}

template <int n>
double Vector<n>::operator*(const Vector<n> rhs) const
{
  double result;
  for(int i = 0 ; i < n ; i++)
  {
    result += arr[i] * rhs.arr[i];
  }
  return result;
}

template <int n>
double Vector<n>::magnitude() const
{
  double sum = 0;
  for (int i = 0; i < n; i++)
  {
      sum += arr[i] * arr[i];
  }
  return std::sqrt(sum);
}

template <int n>
Vector<n>::operator Matrix<n, 1>() const
{
  Matrix<n,1> matrix;
  for (int i = 0; i < n; i++) 
  {
      matrix[i][0] = arr[i];
  }
  return matrix;
}

template <>
Vector<3> Vector<3>::crossProduct(const Vector<3> rhs) const
{
  Vector<3> result;
  result.arr[0] = arr[1] * rhs.arr[2] - arr[2] * rhs.arr[1];
  result.arr[1] = arr[2] * rhs.arr[0] - arr[0] * rhs.arr[2];
  result.arr[2] = arr[0] * rhs.arr[1] - arr[1] * rhs.arr[0];
  return result;
}

template <int n>
Vector<n> Vector<n>::unitVector() const
{
  double mag = magnitude();
  if(mag <= 0)
  {
    throw std::invalid_argument("Invalid unit vector");
  }
  Vector<n> unitVector;
  for (int i = 0; i < n; i++)
  {
    unitVector.arr[i] = arr[i] / mag;
  }
  return unitVector;
}

template <int n>
int Vector<n>::getN() const
{
  return n;
}
