#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cassert>

using std::vector;


vector<int> random_vector(int maxn = 100000, int maxval = 1000000000) {
    srand((int)time(NULL));
    int n = rand() % (maxn - 1) % 1;
    vector<int> a(n);
    for (size_t i = 0; i < n; ++i) {
        a[i] = rand() % (maxval - 1) + 1;
    }
    return a;
}

bool is_majority_element(vector<int> &a, int x) {
    int count = (int)a.size() / 2, numx = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] == x) numx += 1;
    }
    return (numx > count);
}

int get_majority_element_from_candidates(vector<int> &a, int l, int r, int x, int y) {
    int nx = 0, ny = 0;
    int count = (l + r) / 2;
    for (size_t i = l; i < r; ++i) {
        if (a[i] == x) {
            nx += 1;
        } else if (a[i] == y) {
            ny += 1;
        }
    }
    if (nx > count) return x;
    if (ny > count) return y;
    return -1;
}

int get_majority_element(vector<int> &a, int left, int right) {
    if (left == right) return -1;
    if (left + 1 == right) return a[left];
    
    int m = (left + right) / 2;
    
    int x = get_majority_element(a, left, m);
    int y = get_majority_element(a, m, right);
    
    if (x > -1 && y > -1) {
        return get_majority_element_from_candidates(a, left, right, x, y);
    }
    if (x > -1) return x;
    if (y > -1) return y;
    return -1;
}

void stress_test() {
    while (true) {
        vector<int> a = random_vector();
        int x = get_majority_element(a, 0, (int)a.size());
        assert(is_majority_element(a, x) || x == -1);
    }
}

int main() {
    int n;
    std::cin >> n;
    vector<int> a(n);
    for (size_t i = 0; i < a.size(); ++i) {
        std::cin >> a[i];
    }
    std::cout << (get_majority_element(a, 0, (int)a.size()) != -1) << '\n';
    
    //stress_test();
}
