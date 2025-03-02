#include "Matrix.h"

template <int n, int m>
Matrix<n, m>::Matrix()
{
  arr = new double*[n];
  for(int i = 0 ; i < n ; i++)
  {
    arr[i] = new double[m]();
  }
}

template <int n, int m>
Matrix<n, m>::Matrix(double **otherArr)
{
  arr = new double*[n];
  for(int i = 0 ; i < n ; i++)
  {
    arr[i] = new double[m]();
    for(int j = 0 ; j < m ; j++)
    {
      arr[i][j] = otherArr[i][j];
    }
  }
}

template <int n, int m>
Matrix<n, m>::Matrix(const Matrix &other)
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

template <int n, int m>
Matrix<n, m>::~Matrix()
{
  if (arr)
  {
    for (int i = 0; i < n; ++i)
    {
      delete[] arr[i];
    }
    delete[] arr;
  }
}

template <int n, int m>
Matrix<n, m>& Matrix<n, m>::operator=(const Matrix<n, m> &other)
{
  if (this != &other)
  {
    for (int i = 0; i < n; ++i)
    {
      delete[] arr[i];
    }
    delete[] arr;
    arr = new double*[n];
    for (int i = 0; i < n; ++i)
    {
      arr[i] = new double[m];
      for (int j = 0; j < m; ++j)
      {
        arr[i][j] = other.arr[i][j];
      }
    }
  }
  return *this;
}

template <int n, int m>
template <int a>
Matrix<n, a> Matrix<n, m>::operator*(const Matrix<m, a> rhs) const
{
  Matrix<n,a> result;
  for(int i = 0; i < n; i++)
  {
      for(int j = 0; j < a; j++)
      { 
          result[i][j] = 0;
          for(int k = 0; k < m; k++)
          {
              result[i][j] += (*this)[i][k] * rhs[k][j];
          }
      }
  }
  return result;
}

template <int n, int m>
Matrix<n, m> Matrix<n, m>::operator*(const double val) const
{
  Matrix<n, m> result;
  for(int i = 0 ; i < n ; i++)
  {
    for(int j = 0 ; j < m ; j++)
    {
      result[i][j] = (*this)[i][j] * val;
    }
  }
  return result;
}

template <int n, int m>
Matrix<n, m> Matrix<n, m>::operator+(const Matrix<n, m> rhs) const
{
  Matrix<n, m> result;
  for(int i = 0; i < n; i++)
  {
      for(int j = 0; j < m; j++)
      {
          result.arr[i][j] = arr[i][j] + rhs.arr[i][j];
      }
  }
  return result;
}

template <int n>
SquareMatrix<n> SquareMatrix<n>::operator!() const
{
  double det = determinant();
  if (det == 0)
  {
      throw std::invalid_argument("Inverse does not exist");
  }
  SquareMatrix<n> adjugate;
  for (int i = 0; i < n; i++)
  {
      for (int j = 0; j < n; j++)
      {
          SquareMatrix<n - 1> sub = submatrix(*this, i, j);
          double cofactor = ((i + j) % 2 == 0 ? 1 : -1) * sub.determinant();
          adjugate[j][i] = cofactor;
      }
  }
  SquareMatrix<n> inverse;
  for (int i = 0; i < n; i++)
  {
      for (int j = 0; j < n; j++)
      {
          inverse[i][j] = adjugate[i][j] / det;
      }
  }
  return inverse;
}

template <int n, int m>
Matrix<m, n> Matrix<n, m>::operator~() const
{
  Matrix<m, n> transposed;
  for(int i = 0; i < n; i++)
  {
      for(int j = 0; j < m; j++)
      {
          transposed[j][i] = arr[i][j];
      }
  }
  return transposed;
}

template <int n, int m>
int Matrix<n, m>::getM() const
{
  return m;
}

template <int n, int m>
int Matrix<n, m>::getN() const
{
  return n;
}

template <int n>
SquareMatrix<n>::SquareMatrix() : Matrix<n, n>()
{
}

template <int n>
SquareMatrix<n>::SquareMatrix(double **arr) : Matrix<n, n>(arr)
{
}

template <int n>
SquareMatrix<n>::~SquareMatrix()
{
}

template <int n>
Vector<n> SquareMatrix<n>::solve(const Vector<n> vec) const
{
  if(determinant() == 0)
  {
    throw std::runtime_error("Unsolvable set of linear equations");
  }
  return this->operator!() * vec.operator Matrix<n, 1>();
}



template <int n>
double SquareMatrix<n>::determinant() const
{
  static_assert(n > 0, "Matrix dimension must be positive");
  double det = 0.0;
  for (int col = 0; col < n; col++)
  {
      SquareMatrix<n - 1> sub = submatrix(*this, 0, col);
      det += ((col % 2 == 0) ? 1 : -1) * (*this)[0][col] * sub.determinant();
  }
  return det;
}

template <>
double SquareMatrix<1>::determinant() const
{
    return (*this)[0][0];
}

template <int n>
SquareMatrix<n - 1> submatrix(const SquareMatrix<n>& mat, int row, int col)
{
    SquareMatrix<n - 1> result;
    for (int i = 0, r = 0; i < n; i++)
    {
        if (i == row) continue;
        for (int j = 0, c = 0; j < n; j++)
        {
            if (j == col) continue;
            result[r][c] = mat[i][j];
            c++;
        }
        r++;
    }
    return result;
}


template <int n>
IdentityMatrix<n>::IdentityMatrix()
{
  this->arr = new double*[n];
  for (int i = 0; i < n; i++)
  {
      this->arr[i] = new double[n]();
      this->arr[i][i] = 1.0;
  }
}

template <int n>
IdentityMatrix<n>::~IdentityMatrix()
{
}
