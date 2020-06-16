/**
 *  binary_search.cpp
 *
 *  This file implements a binary search algorithm to find the index of a given integer
 *  in a sequence of integers. This is for problem 1 of the Algorithmic Toolbox week 4
 *  problem set.
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date:   February 12, 2018
 *
 **/

#include <iostream>
#include <cassert>
#include <vector>
//#include <cstdlib>
//#include <string>
//#include <algorithm>

using std::vector;

int binary_search(const vector<int> &a, int x) {
    int left = 0, right = (int)a.size(), mid = 0;
    while (left < right) {
        mid = (left + right) / 2;
        
        if (mid == left) return (a[mid] == x) ? mid : -1;
        int y = a[mid];
        
        if (y == x) {
            return mid;
        } else if (y < x) {
            left = mid;
        } else {
            right = mid;
        }
    }
    return -1;
}

int linear_search(const vector<int> &a, int x) {
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i] == x) return (int)i;
    }
    return -1;
}

//std::string vector_string(const vector<int> &a) {
//    std::string res;
//    for (size_t i = 0; i < a.size(); ++i) {
//        if (i != 0) res += " ";
//        res += std::to_string(a[i]);
//    }
//    return res;
//}

//void stress_test() {
//    while (true) {
//        int n = (std::rand() % 9999) + 1;
//        vector<int> a(n, 0), b(n, 0);
//        for (size_t i = 0; i < n; ++i) {
//            a[i] = (std::rand() % 999999999) + 1;
//            b[i] = (std::rand() % 999999999) + 1;
//        }
//        std::sort(a.begin(), a.end());
//        std::cout << vector_string(a) << std::endl;
//        for (size_t j = 0; j < n; ++j) {
//            int x = binary_search(a, b[j]), y = linear_search(a, b[j]);
//            std::cout << "target: " << b[j] << "\tbinary: " << x << "\tlinear: " << y << std::endl;
//            assert(x == y || a[x] == a[y]);
//        }
//    }
//}

int main() {
    int n;
    std::cin >> n;
    vector<int> a(n);
    for (size_t i = 0; i < a.size(); i++) {
        std::cin >> a[i];
    }
    int m;
    std::cin >> m;
    vector<int> b(m);
    for (int i = 0; i < m; ++i) {
        std::cin >> b[i];
    }
    for (int i = 0; i < m; ++i) {
        //replace with the call to binary_search when implemented
        //std::cout << linear_search(a, b[i]) << ' ';
        std::cout << binary_search(a, b[i]) << ' ';
    }
    
    //stress_test();
}

