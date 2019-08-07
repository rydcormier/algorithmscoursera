//
//  diet.cpp
//  Advanced Algorithms and Complexity - Programming Assignment 2
//
//  Created by Ryan Cormier on 8/7/19.
//

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
#include <cstdio>
#include <cmath>

using namespace std;

typedef vector<vector<double>> Matrix;
typedef vector<double> Vector;

// Used in LinearSolve for numeric stability.
vector<size_t> LUPDecomposition(Matrix& A)
{
    size_t n = A.size();
    vector<size_t> P(n, 0);
    
    for (size_t i = 0; i < n; i++)
        P[i] = i;
    
    for (size_t k = 0; k < n; k++)
    {
        // find pivot
        double pmax = 0;
        size_t pidx = k;
        for (size_t i = k; i < n; i++)
        {
            if (fabs(A[i][k]) > pmax)
            {
                pmax = fabs(A[i][k]);
                pidx = i;
            }
        }
        
        if (pmax == 0)  // singular matrix
            return vector<size_t>();
        
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

// Solve the linear system Ax = b for x.
Vector LinearSolve(Matrix A, Vector b)
{
    size_t n = A.size();
    Vector x(n), y(n);
    
    vector<size_t> P = LUPDecomposition(A);  //  L and U computed in A
    
    if (P.empty()) // singular matrix
        return x;
    
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

// Recursive call for GenSubsets
void gen(size_t i,
         size_t n,
         size_t r,
         vector<size_t> combo,
         set<vector<size_t>>& C)
{
    if (combo.size() > r)
        return;
    
    if (combo.size() == r)
    {
        C.insert(combo);
        return;
    }
    
    if (i >= n)
        return;
    
    gen(i + 1, n, r, combo, C);
    
    combo.push_back(i);
    gen(i + 1, n, r, combo, C);
    
}

// Returns all subsets of {0, 1, ..., n - 1} of size r.
set<vector<size_t>> GenSubsets(size_t n, size_t r)
{
    set<vector<size_t>> S;
    vector<size_t> combo;
    
    gen(0, n, r, combo, S);
    
    return S;
}

// Returns inner product of two vectors.
double InnerProduct(Vector x, Vector y)
{
    double ans = 0.0;
    
    size_t n = x.size();
    
    for (size_t i = 0; i < n; i++)
        ans += (x[i] * y[i]);
    
    return ans;
}

// Checks if the solution x is valid for all constraints in A and b.
bool CheckSolution(const Matrix& A, const Vector& x, const Vector& b)
{
    for (size_t i = 0; i < A.size(); i++)
    {
        double z = InnerProduct(A[i], x);
        if (z > b[i] && fabs(z - b[i]) > 0.001) // room for rounding error
            return false;
    }
    
    return true;
}

// Uses LinearSolve on all combinations of n rows from A to find an optimum, if
// it exists. If an optimum does not exist, it determines if there is no
// feasible solution or if it is infinite.
pair<int, Vector> solve_diet_problem(int n,
                                     int m,
                                     Matrix A,
                                     Vector b,
                                     Vector c)
{
    Vector solution;
    
    int flag = 0;
    
    // add non zero constraints to A and b
    for (int i = 0; i < n; i++)
    {
        Vector row(n, 0.0);
        row[i] = -1;
        A.push_back(row);
        b.push_back(0);
    }
    
    // add upper bound to A and b
    A.push_back(Vector(n, 1));
    b.push_back(1000000000.0);
    
    // Find all combinations of n indices from A.
    set<vector<size_t>> S = GenSubsets(A.size(), n);
    
    double z = -INFINITY;
    bool unbounded = false;
    
    for (set<vector<size_t>>::iterator it = S.begin(); it != S.end(); it++)
    {
        vector<size_t> s(*it);
        Matrix A_(n);
        Vector b_(n);
        for (size_t j = 0; j < n; j++)
        {
            A_[j] = A[s[j]];
            b_[j] = b[s[j]];
        }
        
        Vector x = LinearSolve(A_, b_);
        
        if (x.empty() || !CheckSolution(A, x, b))
            continue;
        
        double z_ = InnerProduct(x, c);
        
        if (z_ > z)
        {
            z = z_;
            solution = x;
            
            // if upper bound constraint is present, solution is unbounded
            unbounded = (s.back() == A.size() - 1);
        }
    }
    
    if (solution.empty()) // no solution
        flag = -1;
    else if (unbounded) // infinity
        flag = 1;
    
    return {flag, solution};
}

int main(){
    int n, m;
    cin >> m >> n;
    Matrix A(m, Vector(n));
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            cin >> A[i][j];
        }
    }
    Vector b(m);
    for (int i = 0; i < m; i++) {
        cin >> b[i];
    }
    Vector c(n);
    for (int i = 0; i < n; i++) {
        cin >> c[i];
    }
    
    pair<int, Vector> ans = solve_diet_problem(n, m, A, b, c);
    
    switch (ans.first) {
        case -1:
            printf("No solution\n");
            break;
        case 0:
            printf("Bounded solution\n");
            for (int i = 0; i < n; i++) {
                printf("%.18f%c", ans.second[i], " \n"[i + 1 == n]);
            }
            break;
        case 1:
            printf("Infinity\n");
            break;
    }
    return 0;
}
