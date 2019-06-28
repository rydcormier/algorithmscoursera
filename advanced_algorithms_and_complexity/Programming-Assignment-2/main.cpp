//
//  diet.cpp
//  Advanced Algorithms and Complexity - Programming Assignment 2
//
//  Created by Ryan Cormier on 6/20/19.
//

#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>
#include <cstdio>
using namespace std;

typedef vector<vector<double>> Matrix;
typedef vector<double> Row;
typedef vector<double> Column;

struct LinearProgram
{
    int n, m;
    set<int> N;
    set<int> B;
    Matrix A;
    Column b;
    Row c;
    double v;
    
    LinearProgram(int n_,
                  int m_,
                  const Matrix& A_,
                  const Column& b_,
                  const Row& c_) :
    n(n_),
    m(m_),
    N(set<int>()),
    B(set<int>()),
    A(Matrix(n_ + m_, Row(n_ + m_, 0))),
    b(Column(n_ + m_, 0)),
    c(Row(n_ + m_, 0)),
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
    
    LinearProgram(const LinearProgram& L) :
    n(L.n), m(L.m), N(L.N), B(L.B), A(L.A), b(L.b), c(L.c), v(L.v) {}
    
    Matrix get_A() const
    {
        Matrix A_(m, Row(n));
        set<int>::iterator itB = B.begin();
        for (int i = 0; i < m; ++i)
        {
            set<int>::iterator itN = N.begin();
            for (int j = 0; j < n; ++j)
            {
                A_[i][j] = A[*itB][*itN];
                itN++;
            }
            itB++;
        }
        
        return A_;
    }
    
    Column get_b() const
    {
        Column b_(m);
        set<int>::iterator it = B.begin();
        for (int i = 0; i < m; ++i)
        {
            b_[i] = b[*it];
            it++;
        }
        
        return b_;
    }
    
    Row get_c() const
    {
        Column c_(n);
        set<int>::iterator it = N.begin();
        
        for (int j = 0; j < n; ++j)
        {
            c_[j] = c[*it];
            it++;
        }
        
        return c_;
    }
    
    bool is_basic(int i) const
    {
        for (set<int>::iterator it = B.begin(); it != B.end(); ++it)
        {
            if (i == *it)
                return true;
        }
        return false;
    }
    
    bool is_nonbasic(int i) const
    {
        for (set<int>::iterator it = N.begin(); it != N.end(); ++it)
        {
            if (i == *it)
                return true;
        }
        return false;
    }
    
    void print_slack() const
    {
        printf("z   =  %5.2f", v);
        for (set<int>::iterator j = N.begin(); j != N.end(); j++)
            printf(" + %5.2fx%d", c[*j], *j + 1);
        
        for (set<int>::iterator i = B.begin(); i != B.end(); ++i)
        {
            printf("\nx%d  =  %5.2f", *i + 1, b[*i]);
            
            for (set<int>::iterator j = N.begin(); j != N.end(); ++j)
                printf(" + %5.2fx%d", -A[*i][*j], *j + 1);
        }
        printf("\n");
    }
};


LinearProgram Pivot(const LinearProgram& L, int l, int e)
{
    int n = L.n; int m = L.m;
    Matrix A(m, Row(n, 0));
    Column b(m, 0);
    Row c(n, 0);
    double v(L.v);
    set<int> N(L.N);
    set<int> B(L.B);
    
    N.erase(e);
    B.erase(l);
    
    LinearProgram LP(n, m, A, b, c);
    
    A = L.A;
    b = L.b;
    c = L.c;
    
    LP.b[e] = b[l] / A[l][e];
    
    for (set<int>::iterator j = N.begin(); j != N.end(); ++j)
        LP.A[e][*j] = A[l][*j] / A[l][e];
    
    LP.A[e][l] = 1 / A[l][e];
    
    for (set<int>::iterator i = B.begin(); i != B.end(); ++i)
    {
        LP.b[*i] = b[*i] - (A[*i][e] * LP.b[e]);
        
        for(set<int>::iterator j = N.begin(); j != N.end(); ++j)
            LP.A[*i][*j] = A[*i][*j] - (A[*i][e] * LP.A[e][*j]);
        
        LP.A[*i][l] = (-1) * (A[*i][e] * LP.A[e][l]);
    }
    
    LP.v = v + (c[e] * LP.b[e]);
    
    for (set<int>::iterator j = N.begin(); j != N.end(); ++j)
        LP.c[*j] = c[*j] - (c[e] * LP.A[e][*j]);
    
    LP.c[l] = (-1) * (c[e] * LP.A[e][l]);
    N.insert(l);
    B.insert(e);
    
    LP.N = N;
    LP.B = B;
    
    return LP;
}

LinearProgram AuxiliaryLinearProgram(const LinearProgram& L)
{
    LinearProgram LX(L);
    
    // Add an auxiliary nonbasic variable x0 and add -x0 to each constraint
    LX.n++;
    
    LX.A.insert(LX.A.begin(), Row(LX.n, 0));
    for (int i = 1; i < LX.n + LX.m; ++i)
    {
        Row::iterator it = LX.A[i].begin();
        if (LX.is_basic(i))
            LX.A[i].insert(it, -1);
        else
            LX.A[i].insert(it, 0);
    }
    
    // Adjuxt indexes
    set<int> N, B;
    N.insert(0);
    for (set<int>::iterator i = LX.N.begin(); i != LX.N.end(); ++i)
        N.insert(*i + 1);
    
    for (set<int>::iterator j = LX.B.begin(); j != LX.B.end(); ++j)
        B.insert(*j + 1);
    
    LX.N = N;
    LX.B = B;
    
    LX.b.insert(LX.b.begin(), 0);
    
    // change the objective function to -x0
    LX.c = Row(LX.n + LX.m, 0);
    LX.c[0] = -1;
    
    
    return LX;
}

pair<int, LinearProgram> InitializeSimplex(int n, int m, const Matrix& A,
                                           const Column& b, const Row& c)
{
     LinearProgram L(n, m, A, b, c);
    
    int k = 0;
    double b_min = b[0];
    
    // find index k for which b[k] is a minimum
    for (int i = 0; i < m; ++i)
    {
        if (b[i] < b_min)
        {
            b_min = b[i];
            k = i;
        }
    }
    
    if (b_min > 0)
        return {0, L};
    
    // Create Auxiliary program
    LinearProgram LX = AuxiliaryLinearProgram(L);
    
    int l = n + k;
    int e = 0;
    
    LX = Pivot(LX, l, e);
    
    // Run while loop from simplex
    while (e >= 0)
    {
        e = -1;
        
        // find lowest indexed e for which c[e] > 0
        for (set<int>::iterator j = LX.N.begin(); j != LX.N.end(); ++j)
        {
            if (LX.c[*j] > 0)
            {
                e = *j;
                break;
            }
        }
        
        if (e < 0) // No positive coefficients in objective function
            break;
        
        Row delta(LX.n + LX.m, INFINITY);
        double delta_min = INFINITY;
        l = -1;
        
        // Set delta and find minimizing index from B
        for (set<int>::iterator i = LX.B.begin(); i != LX.B.end(); ++i)
        {
            if (LX.A[*i][e] > 0)
            {
                delta[*i] = LX.b[*i] / LX.A[*i][e];
                if (delta[*i] < delta_min)
                {
                    delta_min = delta[*i];
                    l = *i;
                }
            }
        }
        
        if (delta_min == INFINITY)
            return {1, L}; // Unbounded
        
        LX = Pivot(LX, l, e);
    }
    
    // Set the optimal solution to LX
    Row x(LX.n);
    for (int j = 0; j < LX.n; ++j)
    {
        if (LX.is_basic(j))
            x[j] = LX.b[j];
        else
            x[j] = 0;
    }
    
    if (x[0] != 0) // Infeasible
        return {-1, L};
    
    if (LX.is_basic(0)) // Perform one degenerate pivot to make x0 nonbasic
        LX = Pivot(LX, *LX.B.begin(), 0);
    
    // Remove x0 from constraints and restore original objective function
    for (int i = 0; i < LX.n + LX.m; ++i)
    {
        LX.A[i].erase(LX.A[i].begin());
    }
    LX.A.erase(LX.A.begin());
    LX.n--;
    
    LX.b.erase(LX.b.begin());
    LX.c = L.c;
    
    // adjust indexes
    LX.N.erase(0);
    set<int> N, B;
    for (set<int>::iterator j = LX.N.begin(); j != LX.N.end(); ++j)
        N.insert(*j + 1);
    for (set<int>::iterator i = LX.B.begin(); i != LX.B.end(); ++i)
        B.insert(*i + 1);
    LX.N = N;
    LX.B = B;
    
    // replace nonbasic variables from L.c that are now basic
    for (set<int>::iterator i = L.N.begin(); i != L.N.end(); ++i)
    {
        if (LX.is_basic(*i))
        {
            LX.v += L.c[*i] * LX.b[*i];
            for (set<int>::iterator j = LX.N.begin(); j != LX.N.end(); ++j)
            {
                LX.c[*j] += L.c[*i] * ((-1) * LX.A[*i][*j]);
            }
        }
    }
    
    return {0, LX};
}

pair<int, Row> Simplex(int n, int m, const Matrix& A,
                       const Column& b, const Row& c)
{
    LinearProgram L(n, m, A, b, c);
    Row solution(n);
    
    // TODO
    
    return {0, solution};
}

int main()
{
    Matrix A = {{2, -1}, {1, -5}};
    Column b = {2, -4};
    Row c = {2, -1};
    
    LinearProgram L(2, 2, A, b, c);
    L.print_slack();
    printf("\n\n");
    
    LinearProgram LX = InitializeSimplex(2, 2, A, b, c).second;
    
    LX.print_slack();
    printf("\n");
    return 0;
}

//int main()
//{
//    int n, m;
//    cin >> m >> n;
//    Matrix A(m, vector<double>(n));
//    for (int i = 0; i < n; i++)
//    {
//        for (int j = 0; j < m; j++)
//        {
//            cin >> A[i][j];
//        }
//    }
//    Column b(m);
//    for (int i = 0; i < m; i++)
//    {
//        cin >> b[i];
//    }
//    Row c(n);
//    for (int i = 0; i < n; i++)
//    {
//        cin >> c[i];
//    }
//
//    pair<int, vector<double>> ans = Simplex(n, m, A, b, c);
//
//    switch (ans.first)
//    {
//        case -1:
//            printf("No solution\n");
//            break;
//
//        case 0:
//            printf("Bounded solution\n");
//            for (int i = 0; i < n; i++)
//            {
//                printf("%.18f%c", ans.second[i], " \n"[i + 1 == n]);
//            }
//            break;
//
//        case 1:
//            printf("Infinity\n");
//            break;
//    }
//
//    return 0;
//}
