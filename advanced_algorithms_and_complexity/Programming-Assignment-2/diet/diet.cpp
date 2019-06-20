//
//  diet.cpp
//  Advanced Algorithms and Complexity - Programming Assignment 2
//
//  Created by Ryan Cormier on 6/20/19.
//

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
#include <cstdio>
using namespace std;

typedef vector<vector<double>> Matrix;
typedef vector<double> Row;
typedef vector<double> Column;

const double INF = 1000001;

Matrix negative(const Matrix& A)
{
    Matrix a(A);
    for (size_t i = 0; i < a.size(); ++i)
    {
        for (size_t j = 0; j < a[i].size(); ++j)
        {
            a[i][j] *= -1;
        }
    }
    
    return a;
}

struct LinearProgram
{
    Matrix  A;
    Column  b;
    Row     c;
    int     n;
    int     m;
    
    LinearProgram(Matrix A_, Column b_, Row c_) : A(A_), b(b_), c(c_) {}
};

struct SlackForm
{
    LinearProgram   L;
    Matrix          A;
    Column          b;
    Row             c;
    set<size_t>     N;
    set<size_t>     B;
    double          v;
    
    SlackForm(const LinearProgram& L_) :    L(L_),
    A(Matrix(L_.m, Row(L_.n + L_.m))),
    b(L_.b),
    c(L_.c),
    N(set<size_t>()),
    B(set<size_t>()),
    v(0)
    {
        for (size_t i = 0; i < L.n; ++i)
        {
            N.insert(i);
            for (size_t k = 0; k < A[i].size(); ++k)
            {
                if (k < L_.n)
                    A[i][k] = (-1 * L.A[i][k]);
                else
                    A[i][k] = 0;
            }
        }
        
        for (size_t j = L.n; j < L_.n + L_.m; j++)
        {
            B.insert(j);
            c.push_back(0);
        }
    }
    
    SlackForm(const SlackForm& other) : L(other.L),
    A(other.A),
    b(other.b),
    c(other.c),
    N(other.N),
    B(other.B),
    v(other.v)
    {}
};

SlackForm Auxiliary(const SlackForm& LP)
{
    SlackForm L_aux(LP);
    
    // add x0
    int n = (int) LP.c.size() + 1;
    int m = LP.L.m;
    
    Row c(n, 0);
    c.back() = -1;
    
    L_aux.c = c;
    Matrix A(m, Row(n));
    for (size_t i = 0; i < m; ++i)
    {
        Row a(n);
        a.back() = -1;
        for (size_t j = 0; j < n - 1; ++j)
        {
            a[j] = L_aux.A[i][j];
        }
        A[i] = a;
    }
    
    L_aux.A = A;
    L_aux.B.insert(n - 1);
    
    return L_aux;
}

SlackForm Pivot(const SlackForm& L, size_t l, size_t e)
{
    SlackForm L_new(L);
    
    // TODO
    
    return L_new;
}

pair<int, SlackForm> InitializeSimplex(LinearProgram& LP)
{
    SlackForm L(LP);
    int flag = 0;
    
    size_t k = 0;
    double min_b = L.b[k];
    
    for (size_t i = 1; i < L.b.size(); ++i)
    {
        if (L.b[i] < min_b)
        {
            min_b = L.b[i];
            k = i;
        }
    }
    
    if (min_b >= 0)
        return {0, L};
    
    SlackForm L_aux = Auxiliary(L);
    size_t n = L_aux.c.size();
    
    size_t l = LP.n + k;
    
    L_aux = Pivot(L_aux, l, 0);
    
    while (true)
    {
        size_t e = n;
        
        for (set<size_t>::iterator it = L.N.begin(); it != L.N.end(); ++it)
        {
            size_t i = *it;
            
            if (L_aux.c[i] > 0)
            {
                e = i;
                break;
            }
        }
        
        if (e == n)
            break;
        
        vector<double> del(n, INF);
        
        for (set<size_t>::iterator it = L.B.begin(); it != L.B.end(); ++it)
        {
            size_t i = *it;
            
            if (L.A[i][e] > 0)
                del[i] = (L.b[i] / L.A[i][e]);
        }
        // find index of del minimum
        size_t l = 0;
        double min = del[0];
        for (size_t j = 1; j < del.size(); ++j)
        {
            if (del[j] < min)
            {
                min = del[j];
                l = j;
            }
        }
        
        if (del[l] == INF)
            return {1, L_aux};
        
        L_aux = Pivot(L_aux, l, e);
    }
    vector<double> solution(n);
    
    for (size_t i = 0; i < n; ++i)
    {
        if (L_aux.B.count(i) > 0)
            solution[i] = L_aux.b[i];
        else
            solution[i] = 0;
    }
    
    if (solution[0] != 0)
        return {-1, L};
    
    // TODO : modify and return
    
    return {flag, L};
}

pair<int, vector<double>> solve_diet_problem(int n,
                                             int m,
                                             Matrix A,
                                             vector<double> b,
                                             vector<double> c)
{
    
    LinearProgram LP(A, b, c);
    pair<int, SlackForm> init = InitializeSimplex(LP);
    
    int flag = init.first;
    SlackForm L(init.second);
    
    if (flag == -1)
        return {-1,  vector<double>(m, 0)};
    
    vector<double> del(n);
    
    while (true)
    {
        size_t e = n + m;
        
        for (set<size_t>::iterator it = L.N.begin(); it != L.N.end(); ++it)
        {
            if (c[*it] > 0)
            {
                e = *it;
                break;
            }
        }
        
        if (e == n + m)
            break;
        
        for (set<size_t>::iterator it = L.B.begin(); it != L.B.end(); ++it)
        {
            size_t i = *it;
            
            if (L.A[i][e] > 0)
                del[i] = (L.b[i] / L.A[i][e]);
            else
                del[i] = INF;
            
            size_t l = 0;
            double min = del[0];
            for (size_t j = 1; j < del.size(); ++j)
            {
                if (del[j] < min)
                {
                    min = del[j];
                    l = j;
                }
            }
            
            if (del[l] == INF)
                return {1, vector<double>(n, 0)};
            
            L = Pivot(L, l, e);
        }
    }
    vector<double> solution(n);
    
    for (size_t i = 0; i < n; ++i)
    {
        if (L.B.count(i) > 0)
            solution[i] = L.b[i];
        else
            solution[i] = 0;
    }
    
    return {0, solution};
}

int main()
{
    int n, m;
    cin >> m >> n;
    Matrix A(m, vector<double>(n));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
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
    
    pair<int, vector<double>> ans = solve_diet_problem(n, m, A, b, c);
    
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

