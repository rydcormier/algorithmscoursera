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
    if (lhs < rhs || lhs == rhs) return true;
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
    return 0;
}
