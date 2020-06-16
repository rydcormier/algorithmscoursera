=======
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
    return 0;
}
