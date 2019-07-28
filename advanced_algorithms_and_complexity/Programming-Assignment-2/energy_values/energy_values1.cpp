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

struct Position
{
    Position(int column, int row): column(column), row(row) {}
    
    int column;
    int row;
    
    bool is_good(const Matrix& a)
    {
        // checks if row and column are valid indeces for a given matrix
        if (row < 0 || column < 0 || a.empty())
            return false;
        if (row >= a.size() || column >= a[0].size())
            return false;
        return true;
    }
};

Matrix ReadInput()
{
    int size;
    std::cin >> size;
    Matrix a(size, Row(size + 1, 0.0));
    
    for (int r = 0; r < size; ++r)
    {
        for (int c = 0; c <= size; ++c)
        {
            std::cin >> a[r][c];
        }
    }
    
    return a;
}

void RescaleRow(Matrix& a, Position& p)
{
    // Multiply a row of a matrix given by position such that the value
    // at position becomes 1.
    double val = a[p.row][p.column];
    
    if (val == 1 || val == 0)
        return;
    for (int i = 0; i <= a.size(); ++i)
    {
        a[p.row][i] *= (1 / val);
    }
}

void SubtractRows(Matrix& a, int row, Position& p)
{
    // Element by element subtraction from the given row index by the row
    // given by position. Only the first row is changed so that the value
    // of the row at the positions column becomes zero.
    if (a[p.row][p.column] != 1)
        RescaleRow(a, p);
    double s = a[row][p.column];
    
    for (int i = 0; i <= a.size(); ++i)
    {
        a[row][i] = a[row][i] - (s * a[p.row][i]);
    }
}

Position FindNextPivot(const Matrix& a, const Position& last_pivot)
{
    // Find the next farthest left leading entry from the last pivot position.
    int n = (int) a.size();
    int r = last_pivot.row + 1;
    int c = last_pivot.column + 1;
    
    while (Position(r, c).is_good(a) && a[r][c] == 0)
    {
        if ( r < n - 1 )
        {
            r++;
        }
        else
        {
            r = last_pivot.row + 1;
            c++;
        }
    }
    return Position(r, c);
}

void SwapRows(Matrix& a, int i, int j)
{
    // Move the row at index j to the ith position and the row at i to the
    // jth position.
    Row temp(a[i]);
    a[i] = a[j];
    a[j] = temp;
}

Column SolveSystem(Matrix &a)
{
    // Gaussion row reduction on a matrix to reduced echelon form.  Returns the
    // last column of the resulting matrix.
    Column result(a.size());
    
    Position pivot = FindNextPivot(a, Position(-1, -1));
    int r = 0;
    while (pivot.is_good(a))
    {
        SwapRows(a, r, pivot.row);
        pivot.row = r;
        
        for (int i = 0; i < a.size(); ++i)
        {
            if (i == pivot.row || a[i][pivot.column] == 0)
                continue;
            
            SubtractRows(a, i, pivot);
        }
        
        r++;
        pivot = FindNextPivot(a, pivot);
    }
    
    for (int i = 0; i < a.size(); ++i)
    {
        result[i] = a[i].back();
    }
    
    return result;
}

void PrintColumn(const Column &column)
{
    int size = (int) column.size();
    std::cout.precision(PRECISION);
    for (int row = 0; row < size; ++row)
        std::cout << column[row] << std::endl;
}

Matrix test_matrix()
{
    Matrix a(2, Row(3, 0.0));
    a[0][0] = 5; a[0][1] = -5; a[0][2] = -1;
    a[1][0] = -1; a[1][1] = -2; a[1][2] = -1;
    
    return a;
}

int main()
{
    Matrix a = test_matrix(); // ReadInput();
    Column solution = SolveSystem(a);
    PrintColumn(solution);
    return 0;
}
