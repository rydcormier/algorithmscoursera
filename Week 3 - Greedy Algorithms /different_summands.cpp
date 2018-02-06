/**
 *  different_summands.cpp
 *
 *  This file includes a solution to problem 5 of the week 3 problem set
 *  in coursera course Algorithmic Toolbox.
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date:   February 3, 2018
 *
 **/

#include <iostream>
#include <vector>
#include <cmath>

using std::vector;
using std::sqrt;

/**
 *  Function incremental_sum
 *
 *  Finds the sum of a natural number and all preceeding natural numbers.
 *
 *  Argument:
 *      An integer n, 1 <= n <= 10^9.
 *
 *  Returns:
 *      The sum 1 + ... + n.
 *
 **/
long long incremental_sum(long long n) {
    // I used a trick from Gauss to come up with this formula.
    return ((n*n + n)/2);
}

/**
 *  Function icremental_sum_inverse
 *
 *  An inverse to the incremental_sum function.
 *
 *  Using some algebra and rounding k down to an integer, one can show:
                                                        ________
 *      1 + ... + k <= n <= 1 + ... + (k + 1)  | k = (\/ 8n + 1  - 1) / 2.
 *
 *  Therefore, given any natural number n, this function finds an integer k such that
 *  1 + 2 + ... + k is the closest to n (and less then) for any integer k.
 *
 **/
long long incremental_sum_inverse(long long n) {
    return (long long)((sqrt(8 * n + 1) - 1) / 2);
}

vector<int> optimal_summands(int n) {
    vector<int> summands;
    
    // greedy choice: {1, 2, ... , k -1, k} or as close to as possible
    long long k = incremental_sum_inverse(n);
    long long sum = incremental_sum(k);

    // we'll try <1, 2, ... , k>
    for (int i = 1; i <= k; ++i) { summands.push_back(i); }
    
    // we know sum <= n. If less than, just replace k with (k + the difference)
    if (sum != n) { summands.back() += (int) (n - sum); }
    
    return summands;
}

int main() {
    int n;
    std::cin >> n;
    vector<int> summands = optimal_summands(n);
    std::cout << summands.size() << '\n';
    for (size_t i = 0; i < summands.size(); ++i) {
        std::cout << summands[i] << ' ';
    }
}
