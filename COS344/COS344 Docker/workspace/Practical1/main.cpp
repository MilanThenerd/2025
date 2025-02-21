#include <iostream>

#include "Vector.h"

using namespace std;

int main()
{
  double* v1_data = new double[3];
  v1_data[0] = 1.0;
  v1_data[1] = 2.0;
  v1_data[2] = 3.0;
  Vector<3> v1 = v1_data;

  double* v2_data = new double[3];
  v2_data[0] = 4.0;
  v2_data[1] = 5.0;
  v2_data[2] = 6.0;
  Vector<3> v2 = v2_data;



  delete[] v1_data;
  delete[] v2_data;

  return 0;
}