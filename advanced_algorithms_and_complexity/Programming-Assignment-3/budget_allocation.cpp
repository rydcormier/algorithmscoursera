//
//  budget_allocation.cpp
//
//  Advanced Algorithms and Complexity: NP-Complete Problems
//  Programming Assignment 3
//
//  Created by Ryan Cormier on 4/11/2020
//

#include <ios>
#include <iostream>
#include <vector>

using namespace std;

typedef vector<int> Vector;
typedef vector<Vector> Matrix;

const bool TESTING = false;

const vector<Matrix> Binary_Combinations = {
    {{}},
    {{0}, {1}},
    {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
    {{0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1},
        {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}}
};

struct ConvertILPToSAT {
    int n, m;
    Matrix A;
    Vector b;
    
    ConvertILPToSAT(int n_, int m_) :
        n(n_),
        m(m_),
        A(m + 1, Vector(n + 1)),
        b(n + 1) {}
    
    Matrix getEquisatisfiableSATFormula();
    void printEquisatisfiableSATFormula();
};

int main() {
    ios::sync_with_stdio(false);

    int n, m;
    cin >> m >> n;
    ConvertILPToSAT converter(n, m);
    
    for (int i = 1; i <= m; i++) {
      for (int j = 1; j <= n; j++) {
        cin >> converter.A[i][j];
      }
    }
    for (int i = 1; i <= m; i++) {
      cin >> converter.b[i];
    }

    converter.printEquisatisfiableSATFormula();
    
    return 0;
}

Vector hadamard_product(const Vector&, const Vector&);

int summation(const Vector&);

int dot_product(const Vector&, const Vector&);

Vector logical_negate(const Vector&, const Vector&);

Matrix ConvertILPToSAT::getEquisatisfiableSATFormula() {
    Matrix formula;
      
    // Constraint:
    //      Any combination of the nonzero coefficients of an inequality must
    //      satisfy that inequality. For example, if a_p, a_q, and a_r are the
    //      nonzero coefficients of an inequality and b_i is the constraining
    //      value, if setting (x_p, x_q, and x_r) to (1, 0, 1) violates the
    //      inequality, add a clause for the negation of this combination.
    //          -(x_p && -x_q && x_r) == (-x_p || x_q || -x_r)
    //      Since there are at most 3 nonzero coefficients at a time, checking
    //      all combinations isn't cost prohibitive.
    for (int i = 1; i <= m; i++) {
        Vector a = A[i];
        Vector nonzero_values, nonzero_indeces;
        
        for (int j = 1; j < a.size(); j++) {
            if (nonzero_values.size() == 3)
                break;
            if (a[j] != 0) {
                nonzero_values.push_back(a[j]);
                nonzero_indeces.push_back(j);
            }
        }
        size_t k = nonzero_indeces.size();
        // sanity check
        if (k == 0 && b[i] < 0) {
            return {{1}, {-1}};
        }
        Matrix binary_combos = Binary_Combinations[k];
        
        for (size_t l = 0; l < binary_combos.size(); l++) {
            if (summation(
                          hadamard_product(binary_combos[l],
                                           nonzero_values)) > b[i]) {
                formula.push_back(
                                  logical_negate(binary_combos[l],
                                                 nonzero_indeces));
            }
        }
    }
    
    // if formula is empty, then there were no violations, and it is satisfiable.
    if (formula.empty()) {
        formula.push_back({1, -1});
    }
    
    return formula;
}

void ConvertILPToSAT::printEquisatisfiableSATFormula() {
    Matrix formula = getEquisatisfiableSATFormula();
    if (TESTING) {
        cout << "p cnf " << n << " " << formula.size() << endl;
    } else {
        cout << formula.size() << " " << n << endl;
    }
    
    for (int i = 0; i < formula.size(); i++) {
        Vector clause = formula[i];
        for (int j = 0; j < clause.size(); j++) {
            cout << (j > 0 ? " " : "") << clause[j];
        }
        cout << " 0" << endl;
    }
}

Vector hadamard_product(const Vector &a, const Vector &b) {
    size_t n = a.size();
    if (b.size() != n)
        return Vector();
    Vector res(n);
    
    for (size_t i = 0; i < n; i++) {
        res[i] = (a[i] * b[i]);
    }
    
    return res;
}

int summation(const Vector& v) {
    int res = 0;
    for (int i = 0; i < v.size(); i++) {
        res += v[i];
    }
    return res;
}

int dot_product(const Vector &a, const Vector &b) {
    return summation(hadamard_product(a, b));
}

Vector logical_negate(const Vector &t, const Vector &v) {
    size_t n = t.size();
    if (v.size() != n)
        return Vector();
    
    Vector res(n);
    
    for (size_t i = 0; i < n; i++) {
        res[i] = (t[i] ? (v[i]) : -v[i]);
        
    }
    
    return res;
}
