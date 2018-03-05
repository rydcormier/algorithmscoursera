/**
*  primitive_calculator.cpp
*
*  Author: Ryan Cormier <rydcormier@gmail.com>
*  Date:   March 4, 2018
**/
#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

int min_count(int n, const vector<int> &dp_table) {
    int res = std::min(n - 1, dp_table[n - 1] + 1);
    if (n % 2 == 0) {
        res = std::min(res, dp_table[n / 2] + 1);
    }
    if (n % 3 == 0) {
        res = std::min(res, dp_table[n / 3] + 1);
    }
    return res;
}

vector<int> optimal_sequence_greedy(int n) {
    vector<int> sequence;
    while (n >= 1) {
        sequence.push_back(n);
        if (n % 3 == 0) {
            n /= 3;
        } else if (n % 2 == 0) {
            n /= 2;
        } else {
            n = n - 1;
        }
    }
    reverse(sequence.begin(), sequence.end());
    return sequence;
}

int primitive_calculator_steps(int num) {
    vector<int> min_num_steps(num + 1, 0);
    min_num_steps[1] = 0;
    for (int n = 2; n <= num; ++n) {
        min_num_steps[n] = n - 1;
        int num_steps = min_num_steps[n - 1] + 1;
        num_steps = (n % 2 == 0 ? std::min(num_steps, min_num_steps[n / 2] + 1) : num_steps);
        num_steps = (n % 3 == 0 ? std::min(num_steps, min_num_steps[n / 3] + 1) : num_steps);
        if (num_steps < min_num_steps[n]) {
            min_num_steps[n] = num_steps;
        }
    }
    return min_num_steps[num];
}

int num_steps(const vector<int>& sequence, int empty_vector_value = 1000000) {
    return (sequence.size() > 0 ? sequence.size() - 1 : empty_vector_value);
}

vector<int> optimal_sequence(int num) {
    vector<vector<int>> dp_table(num + 1, vector<int>());
    dp_table[1] = vector<int>(1, 1);
    for (int n = 2; n <= num; ++n) {
        vector<int> sequence = dp_table[n - 1];
        sequence.push_back(n); // + 1 op
        // *2
        if (n % 2 == 0 && num_steps(dp_table[n / 2]) + 1 < num_steps(sequence)) {
            sequence = dp_table[n / 2];
            sequence.push_back(n);
        }
        // *3
        if (n % 3 == 0 && num_steps(dp_table[n / 3]) + 1 < num_steps(sequence)) {
            sequence = dp_table[n / 3];
            sequence.push_back(n);
        }

        if (num_steps(sequence) < num_steps(dp_table[n])) {
            dp_table[n] = sequence;
        }
    }
    return dp_table[num];
}

int main() {
    int n = 1;
    std::cin >> n;
    vector<int> sequence = optimal_sequence(n);
    std::cout << sequence.size() - 1 << std::endl;
    for (size_t i = 0; i < sequence.size(); ++i) {
        std::cout << sequence[i] << " ";
    }
}
