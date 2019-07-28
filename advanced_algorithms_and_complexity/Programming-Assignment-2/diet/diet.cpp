//
//  diet.cpp
//  Advanced Algorithms and Complexity - Programming Assignment 2
//
//  Created by Ryan Cormier on 7/7/19.
//

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <map>
#include <set>
#include <vector>

using namespace std;

typedef set<int> Set;
typedef map<int, double> Vector;
typedef map<int, Vector> Matrix;

// Data structure representing a linear program stored in slack form.
struct LinearProgram
{
    int n, m;
    Set N, B;
    Matrix A;
    Vector b;
    Vector c;
    double v;
    
    LinearProgram()
    :
    n(0),
    m(0),
    N(Set()),
    B(Set()),
    A(Matrix()),
    b(Vector()),
    c(Vector()),
    v(0.0) {}
    
    LinearProgram(int n_,
                  int m_,
                  const vector<vector<double>> &A_,
                  const vector<double> &b_,
                  const vector<double> &c_)
    :
    n(n_),
    m(m_),
    N(Set()),
    B(Set()),
    A(Matrix()),
    b(Vector()),
    c(Vector()),
    v(0.0)
    {
        for (int j = 1; j <= n; j++)
        {
            N.insert(j);
            c[j] = c_[j - 1];
        }
        
        for (int i = n + 1; i <= n + m; i++)
        {
            B.insert(i);
            b[i] = b_[i - n - 1];
            
            for (int j = 1; j <= n; j++)
                A[i][j] = A_[i - n - 1][j - 1];
        }
    }
    
    LinearProgram(int n_,
                  int m_,
                  const Set &N_,
                  const Set &B_,
                  const Matrix &A_,
                  const Vector &b_,
                  const Vector &c_,
                  double v_)
    :
    n(n_),
    m(m_),
    N(N_),
    B(B_),
    A(A_),
    b(b_),
    c(c_),
    v(v_) {}
    
    LinearProgram(const LinearProgram &L)
    :
    n(L.n),
    m(L.m),
    N(L.N),
    B(L.B),
    A(L.A),
    b(L.b),
    c(L.c),
    v(L.v) {}
    
    bool is_basic(int i)
    {
        return (B.count(i) > 0);
    }
    
    bool is_nonbasic(int j)
    {
        return (N.count(j) > 0);
    }
};

// Given a linear program, return the corresponding auxiliary linear program
// for use in InitiateSimplex.
LinearProgram Auxiliary(const LinearProgram& L)
{
    LinearProgram LX(L);
    
    // add a new auxiliary variable x0 and reset objective function to -x0.
    LX.n++;
    
    for (Set::iterator j = LX.N.begin(); j != LX.N.end(); j++)
        LX.c[*j] = 0;
    
    LX.N.insert(0);
    LX.c[0] = -1;
    
    // insert -x0 to each constraint
    for (Set::iterator i = LX.B.begin(); i != LX.B.end(); i++)
        LX.A[*i][0] = -1;
    
    return LX;
}

// Pivot operation switching roles between the leaving basic variable indexed
// by l and the entering nonbasic variable indexed by e.
LinearProgram Pivot(LinearProgram& L, int l, int e)
{
    int n = L.n;
    int m = L.m;
    Set N(L.N);
    N.erase(e);
    Set B(L.B);
    B.erase(l);
    Matrix A_;
    Vector b_;
    Vector c_;
    double v_;
    
    b_[e] = L.b[l] / L.A[l][e];
    
    // Compute coefficients for new basic var xe
    for (Set::iterator j = N.begin(); j != N.end(); j++)
        A_[e][*j] = L.A[l][*j] / L.A[l][e];
    
    A_[e][l] = 1 / L.A[l][e];
    
    // Compute coefficients for remaining constraints
    for (Set::iterator i = B.begin(); i != B.end(); i++)
    {
        b_[*i] = L.b[*i] - (L.A[*i][e] * b_[e]);
        
        for (Set::iterator j = N.begin(); j != N.end(); j++)
            A_[*i][*j] = L.A[*i][*j] - (L.A[*i][e] * A_[e][*j]);
        
        A_[*i][l] = -L.A[*i][e] * A_[e][l];
    }
    
    // Compute the objective function
    v_ = L.v + (L.c[e] * b_[e]);
    
    for (Set::iterator j = N.begin(); j != N.end(); j++)
        c_[*j] = L.c[*j] - (L.c[e] * A_[e][*j]);
    
    c_[l] = -L.c[e] * A_[e][l];
    
    // compute new sets of variables
    N.insert(l);
    B.insert(e);
    
    return LinearProgram(n, m, N, B, A_, b_, c_, v_);
}

// Given a linear program in standard form, either return an equivalent
// slack form with a feasible basic solution or indicate if the program is
// unbounded or infeasible.
LinearProgram InitiateSimplex(int n,
                              int m,
                              const vector<vector<double>>& A,
                              const vector<double>& b,
                              const vector<double>& c,
                              int& flag) {
    flag = 0;
    LinearProgram L(n, m, A, b, c);
    
    // find the index of the minimum of b
    int l = -1;
    double min_b = INFINITY;
    
    for (Set::iterator i = L.B.begin(); i != L.B.end(); i++)
    {
        if (L.b[*i] < min_b)
        {
            min_b = L.b[*i];
            l = *i;
        }
    }
    
    if (min_b > 0 && l > 0)
        return L;
    
    // Form an auxiliary program
    LinearProgram LX = Auxiliary(L);
    
    LX = Pivot(LX, l, 0);
    
    // optimize basic solution
    while (true)
    {
        // find first index e for which c[e] > 0
        int e = -1;
        for (Set::iterator j = LX.N.begin(); j != LX.N.end(); j++)
        {
            if (LX.c[*j] > 0.00001)
            {
                e = *j;
                break;
            }
        }
        
        if (e < 0)
            break;
        
        Vector delta;
        
        // Compute delta and find minimum;
        for (Set::iterator i = LX.B.begin(); i != LX.B.end(); i++)
        {
            if (LX.A[*i][e] > 0)
                delta[*i] = LX.b[*i] / LX.A[*i][e];
            else
                delta[*i] = INFINITY;
        }
        
        // Find l in B for which delta[l] is a minimum
        double min_delta = INFINITY;
        for (Set::iterator i = LX.B.begin(); i != LX.B.end(); i++)
        {
            if (delta[*i] < min_delta)
            {
                min_delta = delta[*i];
                l = *i;
            }
        }
        
        if (min_delta == INFINITY)  // Unbounded
        {
            flag = 1;
            return L;
        }
        
        LX = Pivot(LX, l, e);
    }
    
    double x0 = (LX.is_basic(0) ? LX.b[0] : 0);
    
    if (x0 != 0)    // Infeasible
    {
        flag = -1;
        return L;
    }
    
    if (LX.is_basic(0))     // One degenerate pivot to make 0 nonbasic
        Pivot(LX, 0, *LX.N.begin());
    
    // Return original objective function and variables
    LX.N.erase(0);
    LX.n--;
    LX.c.erase(0);
    
    for (Set::iterator j = LX.N.begin(); j != LX.N.end(); j++)
        LX.c[*j] = (L.is_nonbasic(*j) ? L.c[*j] : 0);
    
    for (Set::iterator i = LX.B.begin(); i != LX.B.end(); i++)
    {
        LX.A[*i].erase(0);
        
        if (L.is_nonbasic(*i))
        {
            LX.v += L.c[*i] * LX.b[*i];
            
            for (Set::iterator j = LX.N.begin(); j != LX.N.end(); j++)
                LX.c[*j] -= L.c[*i] * LX.A[*i][*j];
        }
    }
    
    return LX;
}

// An implementation of the Simplex Algorithm to optimize linear programs.
vector<double> Simplex(int n,
                       int m,
                       const vector<vector<double>>& A,
                       const vector<double>& b,
                       const vector<double>& c,
                       int& flag)
{
    vector<double> solution;
    
    LinearProgram L = InitiateSimplex(n, m, A, b, c, flag);
    
    if (flag != 0)
        return solution;
    
    Vector delta;
    
    while (true)
    {
        // find first index e for which c[e] > 0
        int e = -1;
        for (Set::iterator j = L.N.begin(); j != L.N.end(); j++)
        {
            if (L.c[*j] > 0.0001)
            {
                e = *j;
                break;
            }
        }
        
        if (e < 0)
            break;
        
        Vector delta;
        
        // Compute delta and find minimum;
        for (Set::iterator i = L.B.begin(); i != L.B.end(); i++)
        {
            if (L.A[*i][e] > 0)
                delta[*i] = L.b[*i] / L.A[*i][e];
            else
                delta[*i] = INFINITY;
        }
        
        // Find l in B for which delta[l] is a minimum
        int l = -1;
        double min_delta = INFINITY;
        
        for (Set::iterator i = L.B.begin(); i != L.B.end(); i++)
        {
            if (delta[*i] < min_delta)
            {
                min_delta = delta[*i];
                l = *i;
            }
        }
        
        if (min_delta == INFINITY)  // Unbounded
        {
            flag = 1;
            return solution;
        }
        
        L = Pivot(L, l, e);
    }
    
    for (int i = 1; i <= n; i++)
    {
        
        if (L.is_basic(i))
            solution.push_back(L.b[i]);
        else
            solution.push_back(0);
    }
    
    return solution;
}

int main()
{
    int n, m;
    cin >> m >> n;
    vector<vector<double>> A(m, vector<double>(n));
    
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cin >> A[i][j];
        }
    }
    
    vector<double> b(m);
    
    for (int i = 0; i < m; i++)
    {
        cin >> b[i];
    }
    
    vector<double> c(n);
    
    for (int i = 0; i < n; i++)
    {
        cin >> c[i];
    }
    
    int flag = 0;
    vector<double> ans = Simplex(n, m, A, b, c, flag);

    switch (flag)
    {
        case -1:
            printf("No solution\n");
            break;

        case 0:
            printf("Bounded solution\n");
            for (int i = 0; i < n; i++)
            {
                printf("%.18f%c", ans[i], " \n"[i + 1 == n]);
            }
            break;

        case 1:
            printf("Infinity\n");
            break;
    }

    return 0;
}
