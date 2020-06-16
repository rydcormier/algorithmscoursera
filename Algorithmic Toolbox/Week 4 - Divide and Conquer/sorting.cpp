/**
 *  sorting.cpp
 *
 *  This file implements a variation of quick sort using a three partition scheme
 *  when splitting the initial array.
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date:   February 11, 2018
 *
 **/

#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <ctime>

using std::vector;
using std::swap;

struct Partition {
    int begin;
    int end;
};

Partition partition3(vector<int> &a, int l, int r) {
    Partition p;
    p.begin = l; p.end = l;
    if (l == r) return p;
    int x = a[l];
    for (int i = l + 1; i <= r; ++i) {
        if (a[i] < x) {
            p.begin += 1;
            p.end += 1;
            swap(a[i], a[p.end]);
            swap(a[p.begin], a[p.end]);
        } else if (a[i] == x) {
            p.end += 1;
            swap(a[i], a[p.end]);
        }
    }
    swap(a[l], a[p.begin]);
    return p;
}

void randomized_quick_sort3(vector<int> &a, int l, int r) {
    if (l >= r) {
        return;
    }
    //srand((int)time(NULL));
    int k = l + rand() % (r - l + 1);
    swap(a[l], a[k]);
    Partition p = partition3(a, l, r);
    
    randomized_quick_sort3(a, l, p.begin - 1);
    randomized_quick_sort3(a, p.end + 1, r);
}

int partition2(vector<int> &a, int l, int r) {
    int x = a[l];
    int j = l;
    for (int i = l + 1; i <= r; i++) {
        if (a[i] <= x) {
            j++;
            swap(a[i], a[j]);
        }
    }
    swap(a[l], a[j]);
    return j;
}

void randomized_quick_sort(vector<int> &a, int l, int r) {
    if (l >= r) {
        return;
    }
    
    int k = l + rand() % (r - l + 1);
    swap(a[l], a[k]);
    int m = partition2(a, l, r);
    
    randomized_quick_sort(a, l, m - 1);
    randomized_quick_sort(a, m + 1, r);
}

bool equal_vectors(vector<int> &a, vector<int> &b) {
    for (int i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            std::cout << "NOT EQUAL: i = " << i << "   a[i] = " << a[i] << "    b[i] = " << b[i] << '\n';
            return false;
        }
    }
    return true;
}

void stress_test() {
    while (true) {
        srand((int)time(NULL));
        
        int n = (rand() % 99) + 1;
        
        vector<int> a(n), b(n);
        for (int i = 0; i < n; ++i) {
            a[i] = rand() % 8 + 1;
            b[i] = a[i];
        }
        
        randomized_quick_sort(a, 0, n - 1);
        randomized_quick_sort3(b, 0, n - 1);
        
        assert(equal_vectors(a, b));
    }
}

int main() {
    int n;
    std::cin >> n;
    vector<int> a(n); //, b(n);
    for (size_t i = 0; i < a.size(); ++i) {
        std::cin >> a[i];
        //b[i] = a[i];
    }
    
    randomized_quick_sort3(a, 0, n - 1);
    
    for (size_t i = 0; i < a.size(); ++i) {
        std::cout << a[i] << ' ';
    }
    //std::cout << '\n';
    //for (size_t i = 0; i < a.size(); ++i) {
    //    std::cout << b[i] << ' ';
    //}
    //    stress_test();
}
