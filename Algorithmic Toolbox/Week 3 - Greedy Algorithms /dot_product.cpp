/**
 *  dot_product.cpp
 *
 *  This file contains a program to find the maximum dot product of all
 *  possible permutations of two sequences of equal length.
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date: February 2, 2018
 **/

#include <algorithm>
#include <iostream>
#include <vector>
#include <random>

#include <cassert>
using std::vector;

// constraints
const int N_MIN = 0;
const int N_MAX = 1000;
const int VAL_MIN = -100000;
const int VAL_MAX = 100000;


long long dot_product(vector<int> a, vector<int> b) {
    long long result = 0;
    for (size_t i = 0; i < a.size(); i++) {
        result += ((long long) a[i]) * b[i];
    }
    return result;
}

/**
 *  Function max_dot_product
 *
 *  Finds the maximum value of the dot product of permutations of two given
 *  sequences of equal length.
 *
 *  Arguments:
 *  a:  a vector of integers.
 *  b:  a vector of integers.
 *
 *  Returns:
 *      The maximum dot product as type long long.
 **/
long long max_dot_product(vector<int> a, vector<int> b) {
    // sort a and b from largest to smallest
    std::sort(a.begin(), a.end());
    std::reverse(a.begin(), a.end());

    std::sort(b.begin(), b.end());
    std::reverse(b.begin(), b.end());

    // make sure a and b are same size
    if (a.size() != b.size()) return 0;

    return dot_product(a, b);
}

void test_corner_cases() {
    //srand(time(NULL));

    vector<int> a_min(1, -100000), b_min(1, -100000);
    vector<int> a_max(1000, 100000), b_max(1000, 100000);

    std::cout << "n = 1,  ai = bi = -100000:  " << max_dot_product(a_min, b_min) << '\n';
    std::cout << "n = 1000, ai = bi = 100000:  " << max_dot_product(a_max, b_max) << '\n';

    // make sure it doesn't crash
    std::cout << "a.size() = 1, b.size() = 1000:  " << max_dot_product(a_min, b_max) << std::endl;
}

void stress_test() {
    // creating random vectors, we assert that our maximum is >= a standard dot product

    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni_v(VAL_MIN,VAL_MAX);
    std::uniform_int_distribution<int> uni_n(N_MIN,N_MAX);

    while (true) {
        int random_n = uni_n(rng);
        vector<int> a, b;
        for (size_t i = 0; i < random_n; ++i) {
            a.push_back(uni_v(rng));
            b.push_back(uni_v(rng));
        }

        long long dp = dot_product(a, b);
        long long mdp = max_dot_product(a, b);

        assert(mdp >= dp);
        std::cout << mdp << " >= " << dp << '\n' << std::endl;
    }


}

int main() {
    size_t n;
    std::cin >> n;
    vector<int> a(n), b(n);
    for (size_t i = 0; i < n; i++) {
        std::cin >> a[i];
    }
    for (size_t i = 0; i < n; i++) {
        std::cin >> b[i];
    }
    std::cout << max_dot_product(a, b) << std::endl;

//    test_corner_cases();

//    stress_test();
    return 0;
}
