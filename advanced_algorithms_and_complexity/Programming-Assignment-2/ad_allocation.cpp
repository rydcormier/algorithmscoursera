//
//  ad_allocation.cpp
//
//  Advanced Algorithms and Complexity: Linear Programming
//  Programming Assignment 2
//
//  Created by Ryan Cormier on 8/9/19.
//

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <cmath>
#include <cstdio>
using namespace std;

typedef map<int, double> Map;
typedef vector<vector<double>> Matrix;
typedef vector<double> Vector;
typedef set<int> Set;

const double TOLERANCE = 0.001;

class Simplex
{
public:
    int flag;
    
    Simplex() : flag(0) {}
    Vector solve(int, int, Matrix&, Vector&, Vector&);
    
private:
    void form_slack(int, int, Set&, Set&, Matrix&, Vector&, Vector&);
    void pivot(int, int, Set&, Set&, Matrix&, Vector&, Vector&, double&, int, int);
    void form_auxiliary(int, int, Set&, Set&, Matrix&, Vector&, Vector&);
    void initialize(int, int, Set&, Set&, Matrix&, Vector&, Vector&, double&);
    int blands_rule(Set&, Vector&);
};

pair<int, Vector> allocate_ads(int n, int m, Matrix A,Vector b, Vector c)
{
    
    Simplex simplex;
    
    Vector solution = simplex.solve(n, m, A, b, c);
    
    return {simplex.flag, solution};
}

int main()
{
    int n, m;
    cin >> m >> n;
    
    Matrix A(m, Vector(n));
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
            cin >> A[i][j];
    }
    
    Vector b(m);
    for (int i = 0; i < m; i++)
        cin >> b[i];
    
    Vector c(n);
    for (int i = 0; i < n; i++)
        cin >> c[i];
    
    pair<int, Vector> ans = allocate_ads(n, m, A, b, c);
    
    switch (ans.first)
    {
        case -1:
            printf("No solution\n");
            break;
            
        case 0:
            printf("Bounded solution\n");
            for (int i = 0; i < n; i++)
                printf("%.18f%c", ans.second[i], " \n"[i + 1 == n]);
            break;
            
        case 1:
            printf("Infinity\n");
            break;
    }
    return 0;
}

// given the standard form (A, b, c), formulate the
// slack form (N, B, A, b, c, v = 0).
void Simplex::form_slack(int n, int m, Set& N, Set& B, Matrix& A, Vector& b,
                         Vector& c)
{
    Matrix A_(A);
    Vector b_(b);
    Vector c_(c);
    
    N.clear();
    B.clear();
    A.clear();
    b.clear();
    c.clear();
    
    A.resize(n + m, Vector(n + m, 0));
    b.resize(n + m, 0);
    c.resize(n + m, 0);
    
    for (int i = 0; i < n; i++)
    {
        N.insert(i);
        c[i] = c_[i];
    }
    
    for (int i = n; i < n + m; i++)
    {
        B.insert(i);
        b[i] = b_[i - n];
        
        for (int j = 0; j < n; j++)
            A[i][j] = A_[i - n][j];
    }
}

// Formulate an auxiliary linear program from a given slack form.
void Simplex::form_auxiliary(int n, int m, Set& N, Set& B, Matrix& A,
                             Vector& b, Vector&c)
{
    int u = n + m;  // the new auxiliary variable
    
    N.insert(u);
    
    // add x[u] to each constraint and an aditional row and column to A
    A.push_back(Vector(u, 0));
    for (int i = 0; i <= u; i++)
        A[i].push_back((B.count(i) > 0 ? -1 : 0));
    
    // resize b
    b.push_back(0);
    
    // update c to minimize x[u]
    c = Vector(u + 1, 0);
    c[u] = -1;
}

// perform pivot operation swapping roles between x[e] and x[l].
void Simplex::pivot(int n, int m, Set& N, Set& B, Matrix& A, Vector& b, Vector&c, double& v,
                    int l, int e)
{
    Set N_(N);
    Set B_(B);
    
    N_.erase(e);
    B_.erase(l);
    
    Matrix A_(A);
    Vector b_(b);
    Vector c_(c);
    double v_(v);
    
    A.clear();
    A.resize(n + m, Vector(n + m, 0));
    b.clear();
    b.resize(n + m, 0);
    c.clear();
    c.resize(n +m, 0);
    v = 0;
    
    // Compute the coefficients of the equation for the new basic variable.
    b[e] = (b_[l] / A_[l][e]);
    for (Set::iterator j = N_.begin(); j != N_.end(); j++)
        A[e][*j] = (A_[l][*j] / A_[l][e]);
    A[e][l] = (1 / A_[l][e]);
    
    // Compute the coefficients of the remaining constraints
    for (Set::iterator i = B_.begin(); i != B_.end(); i++)
    {
        b[*i] = b_[*i] - (A_[*i][e] * b[e]);
        for (Set::iterator j = N.begin(); j != N.end(); j++)
            A[*i][*j] = A_[*i][*j] - (A_[*i][e] * A[e][*j]);
        A[*i][l] = -(A_[*i][e] * A[e][l]);
    }
    
    // Compute the objective function
    v = v_ + (c_[e] * b[e]);
    for (Set::iterator j = N_.begin(); j != N_.end(); j++)
        c[*j] = c_[*j] - (c_[e] * A[e][*j]);
    c[l] = -(c_[e] * A[e][l]);
    
    // update sets
    N_.insert(l);
    B_.insert(e);
    
    N.swap(N_);
    B.swap(B_);
}

// Return the smallest index e such that c[e] > 0 or -1 if none exists.
int Simplex::blands_rule(Set& N, Vector& c)
{
    for (Set::iterator j = N.begin(); j != N.end(); j++)
    {
        if (c[*j] > TOLERANCE)
            return *j;
    }
    
    return -1;
}

// Cycle through equivalent slack forms until a basic solution is feasible.
// If it does not exist, set the flag accordingly and return.
void Simplex::initialize(int n, int m, Set& N, Set& B, Matrix& A, Vector& b,
                         Vector& c, double& v)
{
    flag = 0;
    form_slack(n, m, N, B, A, b, c);
    
    // choose l such that b[k] is a minimum
    int l = 0;
    double b_min = INFINITY;
    for (int i = n; i < n + m; i++)
    {
        if (b[i] < (b_min - TOLERANCE))
        {
            b_min = b[i];
            l = i;
        }
    }
    Vector c_copy(c);
    
    v = 0;
    
    if (b_min > -TOLERANCE) // The initial basic solution is feasible
        return;
    
    // form an auxiliary linear program
    form_auxiliary(n, m, N, B, A, b, c);
    
    // pivot l with auxiliary
    int e = n + m;
    pivot(n + 1, m, N, B, A, b, c, v, l, e);
    
    e = blands_rule(N, c);
    
    // optimize L
    while (e >= 0)  // there exists c[e] > 0
    {
        // choose l such that x[l] is the tightest constraint
        double min_delta = INFINITY;
        for (Set::iterator i = B.begin(); i != B.end(); i++)
        {
            if (A[*i][e] > TOLERANCE &&
                (b[*i] / A[*i][e]) < (min_delta - TOLERANCE))
            {
                min_delta = (b[*i] / A[*i][e]);
                l = *i;
            }
        }
        
        if (min_delta == INFINITY)  // Unbounded
        {
            flag = 1;
            return;
        }
        
        pivot(n + 1, m, N, B, A, b, c, v, l, e);
        
        e = blands_rule(N, c);
    }
    
    if (B.count(n + m) > 0)
    {
        if (fabs(b[n + m]) > TOLERANCE) // infeasible
        {
            flag = -1;
            return;
        }
        
        // degenerate pivot to make auxiliary variable nonbasic
        pivot(n + 1, m, N, B, A, b, c, v, n + m, *N.begin());
    }
    
    // remove auxiliary variable
    N.erase(n + m);
    b.pop_back();
    c.pop_back();
    A.pop_back();
    for (int i = 0 ; i < n + m; i++)
        A[i].pop_back();
    
    // restore objective function updating any basic variables and return;
    c.clear();
    c.resize(n + m, 0);
    
    for (int i = 0; i < n; i++)
    {
        if (N.count(i) > 0)
            c[i] = c_copy[i];
        else
        {
            v += (c_copy[i] * b[i]);
            
            for (Set::iterator j = N.begin(); j != N.end(); j++)
                c[*j] -= (c_copy[i] * A[i][*j]);
        }
    }
}

// Solve a linear system.
Vector Simplex::solve(int n, int m, Matrix& A, Vector& b, Vector& c)
{
    Vector solution(n, 0);
    
    Set N, B;
    double v;
    
    initialize(n, m, N, B, A, b, c, v);
    
    if (flag != 0)  // no bounded solution exists
        return solution;
    
    //Map delta;
    int e = blands_rule(N, c);
    
    while (e >= 0)
    {
        // choose l such that x[l] is the tightest constraint
        int l = -1;
        double min_delta = INFINITY;
        for (Set::iterator i = B.begin(); i != B.end(); i++)
        {
            if (A[*i][e] > TOLERANCE)
            {
                double delta = (b[*i] / A[*i][e]);
                if (l < 0 || delta < (min_delta - TOLERANCE))
                {
                    l = *i;
                    min_delta = delta;
                }
            }
        }
        
        if (l < 0)  // Unbounded
        {
            flag = 1;
            return solution;
        }
        
        pivot(n, m, N, B, A, b, c, v, l, e);
        
        e = blands_rule(N, c);
    }
    
    for (int i = 0; i < n; i++)
        solution[i] = (B.count(i) > 0 ? b[i] : 0);
    
    return solution;
}

