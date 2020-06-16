#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>

using std::vector;

bool is_majority_element(vector<int> &a, int left, int right, int x) {
    int maj_count = (right - left) / 2, num_x = 0;
    for (int i = left; i < right; ++i) {
        if (a[i] == x) {
            num_x += 1;
        }
    }
    
    return (num_x > maj_count);
}

int get_majority_element(vector<int> &a, int left, int right) {
    if (left == right) return -1;
    if (left + 1 == right) return a[left];
    
    int m = (left + right) / 2;
    
    int x = get_majority_element(a, left, m);
    int y = get_majority_element(a, m, right);
    
    if (x != -1 && is_majority_element(a, left, right, x)) return x;
    if (y != -1 && is_majority_element(a, left, right, y)) return y;
    return -1;
}

int main() {
    int n;
    std::cin >> n;
    vector<int> a(n);
    for (size_t i = 0; i < a.size(); ++i) {
        std::cin >> a[i];
    }
    std::cout << (get_majority_element(a, 0, (int)a.size()) != -1) << '\n';
}
