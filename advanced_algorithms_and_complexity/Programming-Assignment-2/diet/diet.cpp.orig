//
//  diet.cpp
//  Advanced Algorithms and Complexity - Programming Assignment 2
//
//  Created by Ryan Cormier on 6/21/19.
//

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>

using namespace std;

typedef vector<vector<double>> Matrix;
typedef vector<double> Row;
typedef vector<double> Column;

//  Data Structure for a linear program. Constructs from standard form -
//  (A, b, c) such that A*x <= b and maximizes x * c - and stores it in
//  slack form - (N, B, A, b, c, v) - for use in a simplex algorithm.
struct LinearProgram
{
    int         n;  // Number of variables
    int         m;  // Number of constraints
    set<int>    N;  // Indices of nonbasic variables
    set<int>    B;  // Indices of basic variables
    Matrix      A;  // coefficients for constraint inequalities
    Column      b;  // the right hand side of constraint inequalities
    Row         c;  // the coefficients of the objective function
    double      v;  // the optional constant for the objective function
    
    LinearProgram(int n_,
                  int m_,
                  const Matrix& A_,
                  const Column& b_,
                  const Row& c_)    :
    n(n_),
    m(m_),
    N(set<int>()),
    B(set<int>()),
    A(n_ + m_, Row(n_ + m_, 0.0)),
    b(Column(n_ + m_, 0.0)),
    c(Row(n_ + m_, 0.0)),
    v(0.0)
    {
        for (int i = 0; i < n + m; ++i)
        {
            if (i < n)
            {
                N.insert(i);
                c[i] = c_[i];
            }
            else
            {
                B.insert(i);
                b[i] = b_[i - n];
                
                for (int j = 0; j < n; ++j)
                {
                    A[i][j] = A_[i - n][j];
                }
            }
        }
    }
    
    LinearProgram(const LinearProgram& other)   :
    n(other.n), m(other.m), N(other.N), B(other.B), A(other.A), b(other.b),
    c(other.c), v(other.v) {}
    
    // Test if the given index corresponds to a basic variable.
    bool is_basic(int i) const
    {
        return (B.count(i) > 0);
    }
    
    // Test if a given index corresponds to a nonbasic variable.
    bool is_nonbasic(int i) const
    {
        return (N.count(i) > 0);
    }
    
};

// Formulates an auxiliary linear program with a basic feasible solution.
LinearProgram AuxiliaryLinearProgram(const LinearProgram& L)
{
    LinearProgram LX(L);
    
    // Add a nonbasic auxiliary variable with index n + m, add it to each
    // constraint, and create a new objective function maximizing its negative.
    int aux_var = LX.n + LX.m;
    LX.n++;
    
    for (int i = 0; i < LX.A.size(); ++i)
    {
        if (LX.is_basic(i))
            LX.A[i].push_back(-1);
        else
            LX.A[i].push_back(0);
    }
    
    LX.A.push_back(Row(LX.n + LX.m, 0.0));
    
    LX.c = Row(LX.n + LX.m, 0.0);
    LX.c.back() = -1;
    
    LX.N.insert(aux_var);
    
    return LX;
}

// The pivot operation used in the simplex algorithm. It swaps roles of a
// basic "leaving" variable and a nonbasic "entering" variable.
LinearProgram Pivot(const LinearProgram& LP, int l, int e)
{
    int n = LP.n;
    int m = LP.m;
    LinearProgram L(n, m, Matrix(m, Row(n, 0.0)),
                    Column(m, 0.0), Row(n, 0.0));
    
    set<int> N(LP.N);
    set<int> B(LP.B);
    Matrix A(LP.A);
    Column b(LP.b);
    Row c(LP.c);
    double v = LP.v;
    
    N.erase(e);
    B.erase(l);
    
    // Compute the coefficients for the new basic variable xe.
    L.b[e] = b[l] / A[l][e];
    for (set<int>::iterator j = N.begin(); j != N.end(); ++j)
        L.A[e][*j] = A[l][*j] / A[l][e];
    L.A[e][l] = 1 / A[l][e];
    
    // Compute the coefficients of the remaing constraints.
    for (set<int>::iterator i = B.begin(); i != B.end(); ++i)
    {
        L.b[*i] = b[*i] - (A[*i][e] * L.b[e]);
        
        for (set<int>::iterator j = N.begin(); j != N.end(); ++j)
            L.A[*i][*j] = A[*i][*j] - (A[*i][e] * L.A[e][*j]);
        
        L.A[*i][l] = (-1) * (A[*i][e] * L.A[e][l]);
    }
    
    // Compute the objective function.
    L.v = v + (c[e] * L.b[e]);
    
    for (set<int>::iterator j = N.begin(); j != N.end(); ++j)
        L.c[*j] = c[*j] - (c[e] * L.A[e][*j]);
    
    L.c[l] = (-1) * (c[e] * L.A[e][l]);
    
    // Update sets of basic and nonbasic indeces.
    N.insert(l);
    B.insert(e);
    
    L.N = N;
    L.B = B;
    
    return L;
}

// Determines if a Linear Program is feasible or not and whether the solution
// is bounded. If a single bounded solution exists, return an equivalent
// linear program with a basic feasible solution for use in the simplex
// algorithm.
LinearProgram InitiateSimplex(int n, int m, const Matrix& A, const Column& b,
                              const Row& c, int& flag)
{
    flag = 0;
    LinearProgram L(n, m, A, b, c);
    
    // find index for the minimum of b
    int k = 0;
    double min_b = b[k];
    
    for (int i = 0; i < m; ++i)
    {
        if (b[i] < min_b)
        {
            min_b = b[i];
            k = i;
        }
    }
    
    if (min_b >= 0)  // We already have a basic feasible solution
        return L;
    
    
    // Create an auxiliary linear program
    LinearProgram LX = AuxiliaryLinearProgram(L);
    
    int l = n + k;
    int e = n + m; // the auxiliary variable
    LX = Pivot(LX, l, e);
    
    // Find optimal solution for LX
    while (true)
    {
        e = -1;
        // Find lowest index e for which c[e] > 0; if it exists.
        for (set<int>::iterator j = LX.N.begin(); j != LX.N.end(); ++j)
        {
            if (LX.c[*j] > 0)
            {
                e = *j;
                break;
            }
        }
        
        if (e < 0)  // we already have an optimal solution
            break;
        
        // Compute delta and find its minimum
        Row delta(LX.n + LX.m, INFINITY);
        l = -1;
        double min_delta = INFINITY;
        for (set<int>::iterator i = LX.B.begin(); i != LX.B.end(); ++i)
        {
            if (LX.A[*i][e] > 0)
            {
                delta[*i] = LX.b[*i] / LX.A[*i][e];
                if (delta[*i] < min_delta)
                {
                    l = *i;
                    min_delta = delta[l];
                }
            }
        }
        
        if (min_delta == INFINITY)   // Unbounded
        {
            flag = 1;
            return L;
        }
        
        LX = Pivot(LX, l, e);
        
    }
    
    // Set optimal solution
    Row x(LX.n + LX.m, 0);
    for (set<int>::iterator i = LX.B.begin(); i != LX.B.end(); ++i)
        x[*i] = LX.b[*i];
    
    if (x[n + m] != 0) // Infeasible
    {
        flag = -1;
        return L;
    }
    
    if (LX.is_basic(n + m)) // one degenerate pivot to make n + m nonbasic
        LX = Pivot(LX, n + m, *LX.N.begin());
    
    // Remove the auxiliary variable from LX and restore the objective function
    LX.A.pop_back();
    for (int i = 0; i < LX.A.size(); ++i)
        LX.A[i].pop_back();
    
    LX.n--;
    LX.N.erase(n + m);
    
    Row c_(n + m, 0);
    
    for (set<int>::iterator i = L.N.begin(); i != L.N.end(); ++i)
    {
        if (LX.is_nonbasic(*i))
            c_[*i] = L.c[*i];
        else
        {
            LX.v += L.c[*i] * LX.b[*i];
            
            for (set<int>::iterator j = LX.N.begin(); j != LX.N.end(); ++j)
                c_[*j] += (-1) * LX.A[*i][*j] * L.c[*i];
        }
    }
    LX.c = c_;
    
    return LX;
}

// An implementation of the Simplex algorithm
pair<int, Row> solve_diet_problem( int n, int m, Matrix A, Column b, Row c)
{
    int flag;
    Row sol(n, 0);
    
    LinearProgram L = InitiateSimplex(n, m, A, b, c, flag);
    
    if (flag != 0)  // No unique solution exists
        return {flag, sol};
    
    while (true)
    {
        // find e in N for which c[e] > 0
        int e = -1;
        for (set<int>::iterator j = L.N.begin(); j != L.N.end(); ++j)
        {
            if (L.c[*j] > 0)
            {
                e = *j;
                break;
            }
        }
        
        if (e < 0)
            break;
        
        // Compute delta and find its minimum
        Row delta(L.n + L.m, INFINITY);
        int l = -1;
        double min_delta = INFINITY;
        for (set<int>::iterator i = L.B.begin(); i != L.B.end(); ++i)
        {
            if (L.A[*i][e] > 0)
            {
                delta[*i] = L.b[*i] / L.A[*i][e];
                if (delta[*i] < min_delta)
                {
                    l = *i;
                    min_delta = delta[l];
                }
            }
        }
        
        if (min_delta == INFINITY) // Unbounded
            return {1, sol};
        
        L = Pivot(L, l, e);
    }
    
    for (int i = 0; i < n; ++i)
    {
        if (L.is_basic(i))
            sol[i] = L.b[i];
        else
            sol[i] = 0;
    }
    
    return {flag, sol};
}

int main()
{
    int n, m;
    cin >> m >> n;
    Matrix A(m, vector<double>(n));
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> A[i][j];
        }
    }
    
    Column b(m);
    for (int i = 0; i < m; i++)
    {
        cin >> b[i];
    }
    
    Row c(n);
    for (int i = 0; i < n; i++)
    {
        cin >> c[i];
    }
    
    pair<int, Row> ans = solve_diet_problem(n, m, A, b, c);
    
    switch (ans.first)
    {
        case -1:
            printf("No solution\n");
            break;
            
        case 0:
            printf("Bounded solution\n");
            for (int i = 0; i < n; i++)
            {
                printf("%.18f%c", ans.second[i], " \n"[i + 1 == n]);
            }
            break;
            
        case 1:
            printf("Infinity\n");
            break;
    }
    
    return 0;
}

