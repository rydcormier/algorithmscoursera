// energy_values.cpp
//
// Advanced Algorithms and Complexity - Programming Assignment 2
// Created by Ryan Cormier <rydcormier@gmail.com> on July 14, 2019.
//

#include <cmath>
#include <iostream>
#include <vector>

const double EPS = 1e-6;
const int PRECISION = 20;

typedef std::vector<double> Vector;
typedef std::vector<Vector> Matrix;
typedef std::vector<size_t> Permutation;

struct Equation
{
    Equation(const Matrix &a, const Vector &b):
    a(a),
    b(b)
    {}
    
    Matrix a;
    Vector b;
};

Equation ReadEquation() {
    size_t size;
    std::cin >> size;
    Matrix a(size, Vector(size, 0.0));
    Vector b(size, 0.0);
    
    for (size_t row = 0; row < size; ++row)
    {
        for (size_t column = 0; column < size; ++column)
            std::cin >> a[row][column];
        
        std::cin >> b[row];
    }
    
    return Equation(a, b);
}

Permutation LUPDecomposition(Matrix& A)
{
    // Decomposes A, storing L and U in place in the matrix A, and returns
    // the permutation matrix P as a vector of position indexes.
    
    size_t n = A.size();
    Permutation P(n);
    
    for (size_t i = 0; i < n; i++)
        P[i] = i;
    
    for (size_t k = 0; k < n; k++)
    {
        // find pivot
        double pmax = 0;
        size_t pidx = k;
        for (size_t i = k; i < n; i++)
        {
            if (std::fabs(A[i][k]) > pmax)
            {
                pmax = std::fabs(A[i][k]);
                pidx = i;
            }
        }
        
        if (pmax == 0)  // singular matrix
            return Permutation();
        
        // swap rows
        std::swap(P[k], P[pidx]);
        std::swap(A[k], A[pidx]);
        
        // update A
        for (size_t i = k + 1; i < n; i++)
        {
            A[i][k] = (A[i][k] / A[k][k]);
            
            for (size_t j = k + 1; j < n; j++)
                A[i][j] = A[i][j] - (A[i][k]*A[k][j]);
        }
    }
    return P;
}

Vector LUPSolve(Matrix& A, const Vector& b)
{
    // Computes the LUP decomposition of A and then solves the system of
    // linear equations using forward and back substitution.
    
    size_t n = A.size();
    Vector x(n), y(n);
    
    Permutation P = LUPDecomposition(A);  //  L and U computed in A
    
    // Forward Substitution
    for (size_t i = 0; i < n; i ++)
    {
        double sum = 0;
        for (size_t j = 0; j < i; j++)
            sum += (A[i][j] * y[j]);
        
        y[i] = b[P[i]] - sum;
    }
    
    // Back Substitution
    for (int i = (int) n - 1; i >= 0; i--)
    {
        double sum = 0;
        for (size_t j = i + 1; j < n; j++)
            sum += (A[i][j] * x[j]);
        
        x[i] = (y[i] - sum) / A[i][i];
    }
    
    return x;
}

void PrintVector(const Vector &column)
{
    size_t size = column.size();
    std::cout.precision(PRECISION);
    
    for (size_t row = 0; row < size; ++row)
        std::cout << column[row] << std::endl;
}

int main()
{
    Equation equation = ReadEquation();
    Vector solution = LUPSolve(equation.a, equation.b);
    PrintVector(solution);
    
    return 0;
}
