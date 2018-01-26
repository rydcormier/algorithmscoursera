/**
 * Fibonacci_huge.cpp
 *
 * This file contains a program to compute the nth Fibonacci number taken
 * modulo m. The integers n and m are greater than or equal to 1 and are less
 * less than or equal to 10^18. By finding the pisano period of Fn mod m for
 * a given m, one can easily compute Fn modulo m for even large values of n
 * and m.  Finding the pisano period is made easier by the fact that if F[n]
 * is the nth Fibonacci number and P[n] is F[n] modulo m, then:
 *      P[0] = 0
 *      P[1] = 1
 * and
 *      P[n] = (P[n-2] + P[n-1]) modulo m.
 * Since every cycle of this sequence starts with 0 and 1, one can determine
 * the size of the period just by finding the next occurence of a 0 followed
 * by a 1.  if Pi is the pisano period, then F[n] modulo m is given by
 *      F[n modulo Pi] modulo m.
 *
 * Author: Ryan Cormier <rydcormier@gmail.com>
 * Date: Junuary 25, 2018
 *
 **/

#include <iostream>
#include <cstdlib>

/**
 * Function get_pisano_period
 *
 * Given an integer, return the period of the sequence given by F[n] modulo m.
 **/
long long get_pisano_period(long long m) {

    if (m == 1) { return m; }

    long long current_fib_remainder = 1;
    long long previous_fib_remainder = 0;
    long long i = 0;

    while (true) {
        i++;
        long long temp_fib_remainder = \
            (previous_fib_remainder + current_fib_remainder) % m;
        previous_fib_remainder = current_fib_remainder;
        current_fib_remainder = temp_fib_remainder;

        // 0 and 1 means we're done
        if (previous_fib_remainder == 0 && current_fib_remainder == 1) {
            return i;
        }
    }
}

/**
 * Function get_fibonacci_number
 *
 * Given n, return the nth Fibonacci number.
 **/
long long get_fibonacci_number(long long n) {
    if (n < 2) { return n; }

    long long previous_fib = 0;
    long long current_fib = 1;

    for (long long i = 2; i <= n; ++i) {
        long long temp_fib = previous_fib + current_fib;
        previous_fib = current_fib;
        current_fib = temp_fib;
    }

    return current_fib;
}

long long get_fibonacci_huge_naive(long long n, long long m) {
    if (n <= 1)
        return n;

    long long previous = 0;
    long long current  = 1;

    for (long long i = 0; i < n - 1; ++i) {
        long long tmp_previous = previous;
        previous = current;
        current = tmp_previous + current;
    }

    return current % m;
}

/**
 * Given n and m, return the nth Fibonacci number taken modulo m.
 **/
long long get_fibonacci_huge_fast(long long n, long long m) {
    if (m == 1) { return 0; }
    if (n < 2) { return n; }

    long long pisano_period = get_pisano_period(m);
    long long smaller_index = n % pisano_period;

    if (smaller_index < 2) { return smaller_index; }

    long long previous = 0;
    long long current = 1;

    for (long long i = 2; i <= smaller_index; ++i) {
        long long tmp_previous = previous;
        previous = current;
        current = (tmp_previous + previous) % m;
    }

    return current;
}

void stress_test_get_fibonacci_huge(long long max_num) {

    while (true) {
        long long n = (rand() % max_num);
        long long m = (rand() % max_num);
        std::cout << "n = " << n << "   m = " << m << std::endl;
        long long fast = get_fibonacci_huge_fast(n, m);
        std::cout << "fast: " << fast << std::endl;
        long long naive = get_fibonacci_huge_naive(n, m);
        std::cout << "naive: " << naive << std::endl;
        std::cout << "===============================" << std::endl;

        if (fast != naive) {
            std::cout << "Error: n = " << n << "   m = " << m << std::endl;
            std::cout << "(fast) " << fast << " != (naive) " << naive;
            std::cout << std::endl;
            break;
        }
    }
}


int main() {
    long long n, m;
    std::cin >> n >> m;

    // std::cout << get_fibonacci_huge_naive(n, m) << std::endl;

    std::cout << get_fibonacci_huge_fast(n, m) << std::endl;

    return 0;
}
