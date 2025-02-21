#ifndef VECTOR_H
#define VECTOR_H

#include "Matrix.h"

template<int n, int m>
class Matrix;

template<int n>// size of the vector
class Vector
{
private:   
    double *arr; // array containing the elements in the vector

public:
    Vector();
    Vector(double *);
    ~Vector();
    Vector(const Vector<n> &);
    Vector(const Matrix<n,1>& );
    Vector<n>& operator=(const Vector<n>&);
    Vector<n> operator+(const Vector<n>) const;
    Vector<n> operator-(const Vector<n>) const;
    Vector<n> operator*(const double) const;
    double operator*(const Vector<n>) const;
    double &operator[](int index) const
    {
        if (index < 0 || index >= n)
        {
            throw "Invalid Index";
        }
        return arr[index];
    }
    double magnitude() const;
    operator Matrix<n,1>() const;
    Vector<3> crossProduct(const Vector<3>) const;
    Vector<n> unitVector() const;
    int getN() const;
    void print() const
    {
        for (int i = 0; i < n; i++)
        {
            std::cout << arr[i] << "\n";
        }
        std::cout << std::endl;
    };
};

#endif /*VECTOR_H*/