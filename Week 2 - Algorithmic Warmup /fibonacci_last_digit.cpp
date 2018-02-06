/**
 * This file contains a program to find the last digit of the summation of
 * the first n Fibonacci numbers.  This can be reduced to a formula only
 * involving single digits:
 *   
 *   Sum(F[n]) modulo 10 = (Sum(F[n - 1]) modulo m + F[n] modulo m) modulo m.
 *
 * Author: Ryan Cormier <rydcormier@gmail.com>
 * Date: January 25, 2018
 *
 **/

#include <iostream>
#include <cstdlib>

// F[n] mod 10
int get_fibonacci_last_digit_fast(long long n) { //, long long m) {
    //if (m == 1) { return 0; }
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

int get_fibonacci_last_digit_naive(int n) {
    if (n <= 1)
        return n;

    int previous = 0;
    int current  = 1;

    for (int i = 0; i < n - 1; ++i) {
        int tmp_previous = previous;
        previous = current;
        current = tmp_previous + current;
    }

    return current % 10;
}


void stress_test_get_fibonacci_last_digit(int max_n) {
    
    while (true) {
        int n = rand() % max_n;
        
        int naive = get_fibonacci_last_digit_naive(n);
        int fast = get_fibonacci_last_digit_fast(n);
        
        std::cout << "n: " << n << "  naive: " << naive << "  fast: " << fast;
        std::cout << "\n===============================" << std::endl;
        
        if (naive != fast) {
            std::cout << "ERROR" << std::endl;
            break;
        }
    }
}

int main() {
    int n;
    std::cin >> n;

//    int c = get_fibonacci_last_digit_naive(n);
    int c = get_fibonacci_last_digit_fast(n);
    std::cout << c << '\n';

//    stress_test_get_fibonacci_last_digit(20);

}
