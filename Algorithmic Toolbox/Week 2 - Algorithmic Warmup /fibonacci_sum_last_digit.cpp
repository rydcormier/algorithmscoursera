#include <iostream>
#include <cstdlib>

// F[n] mod 10
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

int fibonacci_sum_naive(long long n) {
    if (n <= 1)
        return n;

    long long previous = 0;
    long long current  = 1;
    long long sum      = 1;

    for (long long i = 0; i < n - 1; ++i) {
        long long tmp_previous = previous;
        previous = current;
        current = tmp_previous + current;
        sum += current;
    }

    return sum % 10;
}

int fibonacci_sum_fast(long long n) {
    // F[0] + F[1] + ... + F[n] = F[n + 2] - F[2]
    if (n <= 2) { return n; }
    int d = get_fibonacci_last_digit_fast(n + 2);
    if (d == 0) { d += 10; }
    return (d - 1);
}

void stress_test_fibonacci_sum(int max_n) {
    
    while (true) {
        long long n = std::rand() % max_n;
        int naive = fibonacci_sum_naive(n);
        int fast = fibonacci_sum_fast(n);
        std::cout << "\nn: " << n
                  << "    Naive: " << naive
                  << "    Fast: " << fast << std::endl;
        
        if (fast != naive) { break; }
    }
    
}

int main() {
    long long n = 0;
    std::cin >> n;
//    std::cout << fibonacci_sum_naive(n);
    std::cout << fibonacci_sum_fast(n);
    
//    stress_test_fibonacci_sum(50);
    
    return 0;
    
}
