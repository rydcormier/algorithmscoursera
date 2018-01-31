/**
 *  fractional_knapsack.cpp
 *
 *  This file implements a greedy algorithm to find the maximum value of "loot" that
 *  can fit in a knapsack.
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date:   January 31, 2018
 *
 **/

#include <iostream>
#include <vector>

using std::vector;

/**
 *  class Item
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
    double ratio() {return ((this->weight != 0) ? (double) this->value / this->weight : 0.0);}
    bool operator == (Item& other) {return (this->ratio() == other.ratio());}
    bool operator != (Item& other) {return (this->ratio() != other.ratio());}
    bool operator <= (Item& other) {return (this->ratio() <= other.ratio());}
    bool operator < (Item& other) {return (this->ratio() < other.ratio());}
    bool operator >= (Item& other) {return (this->ratio() <= other.ratio());}
    bool operator > (Item& other) {return (this->ratio() > other.ratio());}
};

double get_optimal_value(int capacity, vector<int> weights, vector<int> values) {
    double val = 0.0;
    
    if (weights.size() != values.size()) return 0.0;
    
    // make use of Item class
    vector<Item> items;
    for (int i = 0; i < weights.size(); ++i) {
        items.push_back(Item(weights[i], values[i]));
    }
    
    // sort items
    std::sort(items.begin(), items.end());
    
    for (vector<Item>::iterator iter = items.begin(); iter != items.end(); ++iter) {
        if (capacity == 0) break;
        if (iter->weight <= capacity) {
            capacity -= iter->weight;
            val += iter->value;
        }
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
    std::cout << optimal_value << std::endl;
    return 0;
}
