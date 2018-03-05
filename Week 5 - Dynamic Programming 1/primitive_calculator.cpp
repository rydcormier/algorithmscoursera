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

typedef vector<int> Sequence;

Sequence gen_basic_sequence(int n) {
    Sequence res(n);
    for (int i = 1; i <= n; ++i) {
        res[i - 1] = i;
    }
    return res;
}

int num_ops(Sequence seq) {
    return seq.size() - 1;
}

Sequence optimal_sequence_greedy(int n) {
  Sequence sequence;
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

Sequence optimal_sequence(int num) {
    vector<Sequence> min_num_seq(num + 1);
    min_num_seq[1] = Sequence(1, 1);

    for (int n = 2; n <= num; ++n) {
        min_num_seq[n] = gen_basic_sequence(n);
        min_num_seq[0] = Sequence();
        min_num_seq[1] = Sequence(1,1);
        Sequence seq;

        for (int n = 2; n <= num; ++n) {
            // define longest sequence for n
            min_num_seq[n] = gen_basic_sequence(n);

            //+1 op
            seq = min_num_seq[n - 1];
            seq.push_back(n);
            if (num_ops(seq) < num_ops(min_num_seq[n])) {
                min_num_seq[n] = seq;
            }

            //*2 op
            if (n % 2 == 0) {
                seq = min_num_seq[n%2];
                seq.push_back(n);
                if (num_ops(seq) < num_ops(min_num_seq[n])) {
                    min_num_seq[n] = seq;
                }
            }

            //*3 op
            if (n % 2 == 0) {
                seq = min_num_seq[n%2];
                seq.push_back(n);
                if (num_ops(seq) < num_ops(min_num_seq[n])) {
                    min_num_seq[n] = seq;
                }
            }
        }
    }

    return min_num_seq[num];
}

int main() {
    int n;
    while (n != 0) {
      std::cin >> n;
      Sequence sequence = optimal_sequence(n);
      std::cout << sequence.size() - 1 << std::endl;
      for (size_t i = 0; i < sequence.size(); ++i) {
        std::cout << sequence[i] << " ";
      }
      std::cout << std::endl;
  }
}
