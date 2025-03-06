#include "Matrix.cpp"
#include "Vector.cpp"

#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char const *argv[])
{
    // // Test Vector
    // std::cout << "Testing Vector:\n";

    // Vector<3> v1;
    // v1[0] = 1; v1[1] = 2; v1[2] = 3;
    // std::cout << "v1: ";
    // v1.print();

    // Vector<3> v2;
    // v2[0] = 4; v2[1] = 5; v2[2] = 6;
    // std::cout << "v2: ";
    // v2.print();

    // // Vector addition
    // Vector<3> v3 = v1 + v2;
    // std::cout << "v1 + v2: ";
    // v3.print();

    // // Vector subtraction
    // Vector<3> v4 = v1 - v2;
    // std::cout << "v1 - v2: ";
    // v4.print();

    // // Scalar multiplication
    // Vector<3> v5 = v1 * 2.0;
    // std::cout << "v1 * 2.0: ";
    // v5.print();

    // // Dot product
    // double dotProduct = v1 * v2;
    // std::cout << "v1 . v2: " << dotProduct << "\n";

    // // Cross product (only for 3D vectors)
    // Vector<3> v6 = v1.crossProduct(v2);
    // std::cout << "v1 x v2: ";
    // v6.print();

    // // Magnitude
    // double mag = v1.magnitude();
    // std::cout << "|v1|: " << mag << "\n";

    // // Unit vector
    // try
    // {
    //     Vector<3> unitV1 = v1.unitVector();
    //     std::cout << "Unit vector of v1: ";
    //     unitV1.print();
    // }
    // catch (const std::invalid_argument& e)
    // {
    //     std::cerr << "Error: " << e.what() << "\n";
    // }

    // // Test Matrix
    // std::cout << "\nTesting Matrix:\n";

    // Matrix<2, 3> mat1;
    // mat1[0][0] = 1; mat1[0][1] = 2; mat1[0][2] = 3;
    // mat1[1][0] = 4; mat1[1][1] = 5; mat1[1][2] = 6;
    // std::cout << "mat1:\n";
    // mat1.print();

    // Matrix<3, 2> mat2;
    // mat2[0][0] = 7; mat2[0][1] = 8;
    // mat2[1][0] = 9; mat2[1][1] = 10;
    // mat2[2][0] = 11; mat2[2][1] = 12;
    // std::cout << "mat2:\n";
    // mat2.print();

    // // Matrix multiplication
    // Matrix<2, 2> mat3 = mat1 * mat2;
    // std::cout << "mat1 * mat2:\n";
    // mat3.print();

    // // Test SquareMatrix
    // std::cout << "\nTesting SquareMatrix:\n";

    // SquareMatrix<3> A;
    // A[0][0] = 4; A[0][1] = 7; A[0][2] = 2;
    // A[1][0] = 3; A[1][1] = 5; A[1][2] = 6;
    // A[2][0] = 8; A[2][1] = 1; A[2][2] = 9;
    // std::cout << "A:\n";
    // A.print();

    // // Determinant
    // double det = A.determinant();
    // std::cout << "Determinant of A: " << det << "\n";

    // // Inverse
    // try
    // {
    //     SquareMatrix<3> invA = !A;
    //     std::cout << "Inverse of A:\n";
    //     invA.print();
    // }
    // catch (const std::invalid_argument& e)
    // {
    //     std::cerr << "Error: " << e.what() << "\n";
    // }

    // //Solve system of linear equations
    // Vector<3> t;
    // t[0] = 1; t[1] = 2; t[2] = 3;
    // std::cout << "Constants vector t: \n";
    // t.print();

    // try
    // {
    //     Vector<3> solution = A.solve(t);
    //     std::cout << "Solution to A * x = t:\n";
    //     solution.print();
    // }
    // catch (const std::invalid_argument& e)
    // {
    //     std::cerr << "Error: " << e.what() << "\n";
    // }

    // Matrix multiplication
    // Matrix<1, 3> A;
    // A[0][0] = 4; A[0][1] = -5; A[0][2] = 6;

    // A.print();


    // Matrix<3, 2> B;
    // B[0][0] = -6; B[0][1] = 9;
    // B[1][0] = 3; B[1][1] = -4;
    // B[2][0] = -3; B[2][1] = -2;

    // B.print();

    // Matrix<1, 2> C = A * B;

    // C.print();

    Vector<3> A;

    A[0] = 0.2; A[1] = 0.8; A[2] = 0;

    Vector<3> B;
    B[0] = -0.1; B[1] = 0.2; B[2] = 0;

    Vector<3> C;
    C[0] = 0.3; C[1] = 0.4; C[2] = 0;

    Vector<3> D = A.crossProduct(B);

    Vector<3> N = (B - A).crossProduct(C - A);

    N.print();


    Vector<3> L;
    L[0] = 1.65; L[1] = 1.8; L[2] = 1.6;

    Vector<3> M = L - A;

    std::cout << M.magnitude() << std::endl;

    L.unitVector().print();

    double dot = N * L.unitVector();

    std::cout << dot << std::endl;

    double irradiance = std::pow((0.1/(2.3796*2.3796)),5);

    std::cout << irradiance << std::endl;


    Vector<3> O;
    O[0] = 9; O[1] = 0; O[2] = 9;

    Vector<3> S;
    S[0] = 4; S[1] = 0; S[2] = 10;

    Vector<3> T = S - O;

    T.unitVector().print();
    return 0;

}

