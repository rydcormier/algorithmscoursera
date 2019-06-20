// energy_values.cpp
//
// Advanced Algorithms and Complexity - Programming Assignment 2
// Created by Ryan Cormier <rydcormier@gmail.com> on June 14, 2019.
//


#include <cmath>
#include <iostream>
#include <vector>

const double EPS = 1e-6;
const int PRECISION = 20;

typedef std::vector<double> Column;
typedef std::vector<double> Row;
typedef std::vector<Row> Matrix;

void PrintSolution(const Matrix& a)
{
    size_t n = a.size();
    std::cout.precision(PRECISION);
    
    for (size_t i = 0; i  < n; ++i)
    {
        std::cout << a[i][n] << std::endl;
    }
}

Matrix ReadInput()
{
    int size;
    std::cin >> size;
    Matrix a(size, std::vector <double> (size + 1, 0.0));
    
    for (int row = 0; row < size; ++row)
    {
        for (int column = 0; column <= size; ++column)
            std::cin >> a[row][column];
    }
    
    return a;
}

int ForwardReduction(Matrix& a)
{
    size_t n = a.size();
    for (size_t col = 0; col < n; ++col)
    {
        // Find the element in column with largest amplitude
        size_t r_max = col;
        double v_max = a[r_max][col];
        
        for (size_t row = col + 1; row < n; ++row)
        {
            if (std::fabs(a[row][col]) > v_max)
            {
                v_max = a[row][col];
                r_max = row;
            }
        }
        
        // If the element is not on the diagonal, swap rows.
        if (r_max != col)
            std::swap(a[col], a[r_max]);
        
        size_t row = col;
        
        // Rescale row
        if (a[row][col] != 1)
        {
            double val = a[row][col];
            a[row][col] = 1;
            
            for (size_t c = col + 1; c <= n; ++c)
            {
                a[row][c] *= (1 / val);
            }
        }
        
        // fill the rest of the column with zeros.
        for (size_t r = row + 1; r < n; ++r)
        {
            if (a[r][col] != 0)
            {
                double scalar = a[r][col];
                a[r][col] = 0;
                
                for (size_t c = col + 1; c <= n; ++c)
                {
                    a[r][c] -= (scalar * a[row][c]);
                }
            }
        }
    }
    
    // This is only for systems with a single solution.
    return 0;
}

void BackwardReduction(Matrix& a)
{
    int n = (int) a.size();
    
    // Start from bottom right fill the rest of the column with zeros
    for (int col = n - 1; col > 0; --col)
    {
        for (int row = col - 1; row >= 0; --row)
        {
            if (a[row][col] != 0)
            {
                double scalar = a[row][col];
                a[row][col] = 0;
                a[row][n] -= (scalar * a[col][n]);
            }
        }
    }
}

void Solve(Matrix& a)
{
    // reduce to REF
    int flag = ForwardReduction(a);
    
    if (flag != 0)
        return;
    
    // reduce to RREF
    BackwardReduction(a);
}

int main()
{
    Matrix a = ReadInput();
    Solve(a);
    PrintSolution(a);
    return 0;
}
