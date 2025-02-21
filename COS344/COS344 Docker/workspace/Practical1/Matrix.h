#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <cmath>
#include <iomanip>

#include "Vector.h"

template <int n>
class Vector;

template<int n, int m>
class Matrix
{
protected:
    double **arr;

public:
    Matrix();
    Matrix(double **);
    Matrix(const Matrix<n,m> &);
    virtual ~Matrix();
    double *&operator[](int index) const
    {
        if (index >= n || index < 0)
        {
            throw "Invalid index";
        }

        return arr[index];
    }

    
    Matrix<n,m>& operator=(const Matrix<n,m>&);
    template<int a>
    Matrix<n,a> operator*(const Matrix<m,a>) const;
    Matrix<n,m> operator*(const double) const;
    Matrix<n,m> operator+(const Matrix<n,m>) const;
    Matrix<m,n> operator~() const;
    void print() const
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                std::cout << (*this)[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
    int getM() const;
    int getN() const;

    
};

template <int n>
class SquareMatrix : public Matrix<n,n>
{
public:
    SquareMatrix();
    SquareMatrix(double **);
    virtual ~SquareMatrix();
    Vector<n> solve(const Vector<n>) const;
    double determinant() const;
    SquareMatrix<n> operator!() const;
};

template <int n>
class IdentityMatrix : public SquareMatrix<n>
{
public:
    IdentityMatrix();
    virtual ~IdentityMatrix();
};

#endif /*MATRIX_H*/