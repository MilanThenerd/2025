#include "Vector.h"
#include <cmath>

template <int n>
Vector<n>::Vector() 
{
    arr = new double[n]();
}

template <int n>
Vector<n>::Vector(double* inputData) 
{
  arr = new double[n];
  for (int i = 0; i < n; i++) {
      arr[i] = inputData[i];
  }
}


template <int n>
Vector<n>::Vector(const Matrix<n , 1>& matrix) 
{
    arr = new double[n];
    for (int i = 0; i < n; i++) 
    {
        arr[i] = matrix(i , 0);
    }
}

template <int n>
Vector<n>::~Vector() 
{
    delete[] arr;
}

template <int n>
Vector<n>::Vector(const Vector<n>& other) 
{
    arr = new double[n];
    for (int i = 0; i < n; i++) 
    {
        arr[i] = other.arr[i];
    }
}

template <int n>
Vector<n>::operator Matrix<n,1>() const 
{
    Matrix<n,1> matrix;
    for (int i = 0; i < n; i++) 
    {
        matrix(i, 0) = arr[i];
    }
    return matrix;
}

template <int n>
Vector<n>& Vector<n>::operator=(const Vector<n>& other) 
{
    if (this != &other) 
    {
        delete[] arr;
        arr = new double[n];
        for (int i = 0; i < n; i++) 
        {
            arr[i] = other.arr[i];
        }
    }
    return *this;
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
Vector<n> Vector<n>::unitVector() const
{
  double magnitude = this->magnitude();
  Vector<n> unitVector;
  for (int i = 0; i < n; i++)
  {
    unitVector.arr[i] = arr[i] / magnitude;
  }
  return unitVector;
}

template <int n>
int Vector<n>::getN() const
{
    return n;
}

template <int n>
Vector<n> Vector<n>::operator+(const Vector<n> other) const
{
  Vector<n> result;
  for (int i = 0; i < n; i++)
  {
    result.arr[i] = arr[i] + other.arr[i];
  }
  return result;
}

template <int n>
Vector<n> Vector<n>::operator-(const Vector<n> other) const
{
  Vector<n> result;
  for (int i = 0; i < n; i++)
  {
    result.arr[i] = arr[i] - other.arr[i];
  }
  return result;
}

template <int n>
double Vector<n>::operator*(const Vector<n> other) const
{
  double sum = 0;
  for (int i = 0; i < n; i++)
  {
    sum += arr[i] * other.arr[i];
  }
  return sum;
}

template <int n>
Vector<n> Vector<n>::operator*(double scalar) const
{
  Vector<n> result;
  for (int i = 0; i < n; i++)
  {
    result.arr[i] = arr[i] * scalar;
  }
  return result;
}


template <int n>
Vector<3> Vector<n>::crossProduct(const Vector<3> other) const
{
  Vector<3> result;
  result.arr[0] = arr[1] * other.arr[2] - arr[2] * other.arr[1];
  result.arr[1] = arr[2] * other.arr[0] - arr[0] * other.arr[2];
  result.arr[2] = arr[0] * other.arr[1] - arr[1] * other.arr[0];
  return result;
}
