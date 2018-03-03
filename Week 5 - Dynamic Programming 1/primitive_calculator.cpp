#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;

vector<int> gen_basic_sequence(int n) {
    vector<int> res(n);
    for (int i = 1; i <= n; ++i) {
        res[i - 1] = i;
    }
    return res;
}

int num_ops(vector<int> seq) {
    return seq.size() - 1;
}

vector<int> optimal_sequence_greedy(int n) {
  std::vector<int> sequence;
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

vector<int> optimal_sequence(int num) {
    vector<int> min_num_seq[num + 1];
    min_num_seq[1] = vector<int>(1, 1);
    
    for (int n = 2; n <= num; ++n) {
        min_num_seq[n] = gen_basic_sequence(n);
        min_num_seq[0] = vector<int>();
        min_num_seq[1] = vector<int>(1,1);
        vector<int> seq;
        
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
    std::cin >> n;
    vector<int> sequence = optimal_sequence_greedy(n);
    std::cout << sequence.size() - 1 << std::endl;
    for (size_t i = 0; i < sequence.size(); ++i) {
        std::cout << sequence[i] << " ";
    }
    vector<int> sequence1 = optimal_sequence(n);
    std::cout << sequence1.size() - 1 << std::endl;
    for (size_t i = 0; i < sequence1.size(); ++i) {
        std::cout << sequence1[i] << " ";
    }
}
