/**
 *  fractional_knapsack.cpp
 *
 *  This file implements a greedy algorithm to find the maximum value of
 *  "loot" that can fit in a knapsack.
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date:   January 31, 2018
 *
 **/

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

//using std::rand;
//using std::time;
//using std::srand;
using std::vector;

/**
 *  Class Item
 *
 *  Very simple class storing the value and weight of an item that can possibly go in
 *  our knapsack.  Comparison operators are implemented in order to use standard sort
 *  functions.
 *
 **/
class Item {
public:
    int value, weight;
    Item (int v, int w) : value(v), weight(w) {}
    double ratio() const;
    bool operator< (const Item&) const;
};

double Item::ratio() const {
    return ((this->weight != 0) ? (double) this->value / this->weight : 0.0);
}

bool Item::operator< (const Item &other) const {
    double lhs = this->ratio(), rhs = other.ratio();
    if (lhs < rhs) return true;
    return false;
}

/**
 *  Function get_optimal_value
 *
 *  Find the maximum value of loot that can fit in a knapsack.
 *
 *  Arguments:
 *      int capacity: maximum weight the knapsack can hold.
 *      vector<int> values: The values of each item.
 *      vector<int> weights: the weights of each item.
 *
 *  Return:
 *      The maximum value of the loot in a full knapsack.
 **/
double get_optimal_value(int capacity, vector<int> weights, vector<int> values) {
    double val = 0.0;
    // avoid any runtime errors
    if (weights.size() != values.size()) return 0.0;

    // make use of Item class
    vector<Item> items;
    for (int i = 0; i < weights.size(); ++i) {
        items.push_back(Item(values[i], weights[i]));
    }

    // sort items
    std::sort(items.begin(), items.end());
    std::reverse(items.begin(), items.end());

    for (vector<Item>::iterator iter = items.begin(); iter != items.end(); ++iter) {
        if (capacity == 0) break;
        int a = std::min(iter->weight, capacity);
        val += a * iter->ratio();
        capacity -= a;
    }
    return val;
}

//void try_to_produce_segmentation_fault() {
//    // initialize random seed
//    srand(time(NULL));
//    int n = rand() % 1000;
//    int W = rand() % 2000000000;
//    vector<int> weights(n, 0);
//    vector<int>values(n, 0);
//    for (int i = 0; i < n; i++) {
//        weights[i] = rand() % 2000000;
//        values[i] = rand() % 2000000;
//    }
//    std::cout.precision(10);
//    std::cout << std::fixed << "n: " << n << '\n'
//             << "capacity: " << W << '\n'
//             << "optimal_value: " << get_optimal_value(W, weights, values)
//             << std::endl;
//
//}
//
//void test_corner_cases() {
//    vector<int> empty_weights, empty_values;
//    vector<int> max_weights (1000, 2000000);
//    vector<int> max_values (1000, 2000000);
//    std::cout.precision(10);
//    std::cout << std::fixed << "n = 0, capacity = 0: " << get_optimal_value(0, empty_weights, empty_values) << '\n';
//    std::cout << "n = 1000, capacity = 0:  " << get_optimal_value(0, max_weights, max_values) << '\n';
//    std::cout << "n = 0, capacity = 2000000:  " << get_optimal_value(2000000, empty_weights, empty_values) << '\n';
//    std::cout << "n = 1000, capacity = 2000000:  " << get_optimal_value(200000, max_weights, max_values) << '\n';
//    std::cout << std::endl;

//}

int main() {
    int n;
    int capacity;
    std::cin >> n >> capacity;
    vector<int> values(n);
    vector<int> weights(n);
    for (int i = 0; i < n; i++) {
        std::cin >> values[i] >> weights[i];
    }

    double optimal_value = get_optimal_value(capacity, weights, values);
    std::cout.precision(10);
    std::cout << std::fixed << optimal_value << std::endl;


//    while (true) {
//        try_to_produce_segmentation_fault();
//    }

//    test_corner_cases();

    return 0;
}
