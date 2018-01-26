/**
 * fibonacci_partial_sum.cpp
 *
 * This file contains a program to find the last digit of the partial sum
 *      F[m] + F[m + 1] + ... + F[n - 1] + F[n]
 * where F[i] is the ith Fibonacci number and 0 <= m <= n <= 10^18.
 *
 * A little algebera can simplify this expression. If F[n] is the nth
 * Fibonacci number, S[n] is the sum of the first n Fibonacci numbers, and
 * P[m, n] is the sum of the mth through nth Fibonacci numbers,
 *
 *      P[m, n] = F[m] + F[m + 1] + ... + F[n - 1] + F[n]
 *              = (F[0] + F[1] + ... + F[n]) - (F[0] + F[1] + ... + F[m - 1])
 *              = S[n] - S[m - 1].
 * Since
 *      S[n] = F[n + 2] - F[2],
 * we have
 *      P[m, n] = (F[n + 2] - F[2]) - (F[m - 1 + 2] - F[2]);
 * or
 *      ==================================
 *      #  P[m,n] = F[n + 2] - F[m + 1]  #
 *      ==================================
 *
 * Using a fast algorithm to find the last digit of a specific Fibonacci
 * number, all that is left is simple subtraction.
 *
 * Author: Ryan Cormier <rydcormier@gmail.com>
 * Date: January 25, 2018
 *
 **/

#include <iostream>
#include <algorithm>
#include <cstdlib>

long long get_fibonacci_partial_sum_naive(long long from, long long to) {
    long long sum = 0;

    long long current = 0;
    long long next  = 1;

    for (long long i = 0; i <= to; ++i) {
        if (i >= from) {
            sum += current;
        }

        long long new_current = next;
        next = next + current;
        current = new_current;
    }

    return sum % 10;
}

int get_fibonacci_last_digit_fast(long long n) {
    int m = 10;
    if (n < 2) { return n; }
    
    int pisano_period = 60; // the pisano period of m = 10
    int smaller_index = n % pisano_period;
    
    if (smaller_index < 2) { return smaller_index; }
    
    int previous = 0;
    int current = 1;
    
    for (int i = 2; i <= smaller_index; ++i) {
        int tmp_previous = previous;
        previous = current;
        current = (tmp_previous + previous) % m;
    }
    
    return current;
}

int get_fibonacci_partial_sum_fast(long long from, long long to) {
    // save some work on easy cases
    if (from == to) { return get_fibonacci_last_digit_fast(to); }
    if (from == 0) {
        int c = get_fibonacci_last_digit_fast(to + 2);
        if (c == 0) { return 9; }
        return c - 1;
    }
    
    //use the formula P[m, n] = F[n + 2] - F[m + 1]
    int a = get_fibonacci_last_digit_fast(to + 2);
    int b = get_fibonacci_last_digit_fast(from + 1);
    
    if (a < b) { a += 10; }
    
    return a - b;
}

void stress_test_get_fibonacci_partial_sum(long long min_m, long long max_n) {
    while (true) {
        long long a = std::rand() % max_n + min_m;
        long long b = std::rand() % max_n + min_m;
        
        long long from = std::min(a, b);
        long long to = std::max(a, b);
        
        long long fast = get_fibonacci_partial_sum_fast(from, to);
        long long naive = get_fibonacci_partial_sum_naive(from, to);
        
        std::cout << "\nFrom: " << from << "    To: " << to << std::endl;
        std::cout << "fast: " << fast << "    naive: " << naive << std::endl;
        
        if (fast != naive) { break; }
    }
}

void test_corner_cases() {
    //case 1: m = n = 0
    int case_1 = get_fibonacci_partial_sum_fast(0, 0);
    std::cout << "m: 0    n:    0    Sum: " << case_1 << std::endl;
    
    //case 2: m = 0, n = 10^18
    int case_2 = get_fibonacci_partial_sum_fast(0, 1000000000000000000);
    std::cout << "m: 0    n:    10^18    Sum: " << case_2 << std::endl;
    
    //case 3: m = n = 10^18
    int case_3 = get_fibonacci_partial_sum_fast(1000000000000000000, 1000000000000000000);
    std::cout << "m: 10^18    n:    10^18    Sum: " << case_3 << std::endl;
}

int main() {
    long long from, to;
    std::cin >> from >> to;
//    std::cout << get_fibonacci_partial_sum_naive(from, to) << '\n';
    std::cout << get_fibonacci_partial_sum_fast(from, to) << std::endl;
    
//    test_corner_cases();
//    stress_test_get_fibonacci_partial_sum(0, 40);
    
    return 0;
}
