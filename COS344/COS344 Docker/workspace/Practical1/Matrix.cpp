#include "Matrix.h"

template<int n , int m>
Matrix<n,m>::Matrix()
{
  arr = new double*[n];
  for(int i = 0 ; i < n ; i++)
  {
    arr[i] = new double[m]();
  }
}

template<int n , int m>
Matrix<n,m>::Matrix(double** other)
{
  arr = other;
}

template<int n , int m>
Matrix<n,m>::Matrix(const Matrix<n,m>& other)
{
  arr = new double*[n];
  for(int i = 0 ; i < n ; i++)
  {
    arr[i] = new double[m];
    for(int j = 0 ; j < m ; j++)
    {
      arr[i][j] = other.arr[i][j];
    }
  }
}

template<int n , int m>
Matrix<n,m>::~Matrix()
{
  for(int i = 0 ; i < n ; i++)
  {
    delete[] arr[i];
  }
  delete[] arr;
}

template<int n , int m>
Matrix<n,m>& Matrix<n,m>::operator=(const Matrix<n,m>& other)
{
  if(this == &other)
  {
    return *this;
  }
  for(int i = 0 ; i < n ; i++)
  {
    delete[] arr[i];
  }
  delete[] arr;

  arr = new double*[n];
  for(int i = 0 ; i < n ; i++)
  {
    arr[i] = new double[m];
    for(int j = 0 ; j < m ; j++)
    {
      arr[i][j] = other.arr[i][j];
    }
  }
  return *this;
}

template<int n, int m>
template<int a>
Matrix<n,a> Matrix<n,m>::operator*(const Matrix<m,a> other) const 
{
    Matrix<n,a> result;
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < a; j++)
        {
            result[i][j] = 0;
            for(int k = 0; k < m; k++)
            {
                result[i][j] += this->arr[i][k] * other[k][j];
            }
        }
    }
    return result;
}

template<int n , int m>
Matrix<n,m> Matrix<n,m>::operator*(double scalar) const
{
  for(int i = 0 ; i < m ; i++)
  {
    for(int j = 0 ; j < n ; j++)
    {
      arr[i][j] *= scalar;
    }
  }
}

template<int n, int m>
Matrix<n,m> Matrix<n,m>::operator+(const Matrix<n,m> other) const
{
  Matrix<n,m> result;
  for(int i = 0; i < n; i++)
  {
    for(int j = 0; j < m; j++)
    {
      result[i][j] = arr[i][j] + other.arr[i][j];
    }
  }
  return result;
}

template<int n, int m>
Matrix<m,n> Matrix<n,m>::operator~() const
{
  for(int i = 0; i < n; i++)
  {
    delete arr[i];
  }
  delete[] arr;
}

template<int n , int m>
int Matrix<n,m>::getN() const
{
  return n;
}

template<int n , int m>
int Matrix<n,m>::getM() const
{
  return m;
}

template <int n>
SquareMatrix<n>::SquareMatrix()
{
  this->arr = new double*[n];
  for(int i = 0; i < n; i++)
  {
    this->arr[i] = new double[n]();
  }
}

template <int n>
SquareMatrix<n>::~SquareMatrix()
{
  for(int i = 0; i < n; i++)
  {
    delete[] this->arr[i];
  }
  delete[] this->arr;
}

#include <array>
#include <stdexcept>

template<int n>
Vector<n> SquareMatrix<n>::solve(const Vector<n> t) const 
{
  Matrix<n, n> A = *this;
  Vector<n> b = t;

  for (int i = 0; i < n; i++) 
  {
    int pivot = i;
    for (int j = i + 1; j < n; j++) 
    {
      if (std::abs(A[j][i]) > std::abs(A[pivot][i])) 
      {
        pivot = j;
      }
    }
    if (pivot != i) 
    {
      std::swap(A[i], A[pivot]);
      std::swap(b[i], b[pivot]);
    }
    if (A[i][i] == 0)
    {
      throw std::runtime_error("Singular matrix: No unique solution exists.");
    }
    for (int j = i + 1; j < n; j++) 
    {
      double factor = A[j][i] / A[i][i];
      for (int k = i; k < n; k++) 
      {
        A[j][k] -= factor * A[i][k];
      }
      b[j] -= factor * b[i];
    }
  }
  Vector<n> x;
  for (int i = n - 1; i >= 0; i--) 
  {
    x[i] = b[i];
    for (int j = i + 1; j < n; j++) 
    {
      x[i] -= A[i][j] * x[j];
    }
    x[i] /= A[i][i];
  }
  return x;
}

template<int n>
double SquareMatrix<n>::determinant() const
{
    Matrix<n, n> A = *this; // Copy the matrix
    double det = 1;
    
    for (int i = 0; i < n; i++)
    {
        // Pivot selection (find the maximum absolute value in the column)
        int pivot = i;
        for (int j = i + 1; j < n; j++)
        {
            if (std::abs(A[j][i]) > std::abs(A[pivot][i]))
                pivot = j;
        }

        // If the pivot is zero, determinant is zero
        if (A[pivot][i] == 0)
            return 0;

        // Swap rows if needed
        if (pivot != i)
        {
            std::swap(A[i], A[pivot]);
            det *= -1; // Row swap changes the determinant's sign
        }

        // Multiply determinant by the pivot element
        det *= A[i][i];

        // Perform row operations to create upper triangular form
        for (int j = i + 1; j < n; j++)
        {
            double factor = A[j][i] / A[i][i];
            for (int k = i; k < n; k++)
            {
                A[j][k] -= factor * A[i][k];
            }
        }
    }
    return det;
}


