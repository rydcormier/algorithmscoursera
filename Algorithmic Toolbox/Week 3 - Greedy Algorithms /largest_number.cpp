/**
 *  largest_number.cpp
 *
 *  This file contains a program for the "Maximum Salary" problem in the week 3 problem
 *  set of the Algorithmic Toolbox course.  The solution is pretty straightforward: 
 *  by implementing a comparative function we can then use the standard sort function 
 *  and then concatenate the sorted vector (of strings) into the final solution.
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date:   Februrary 5, 2018
 *
 **/

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

// Basic representation of the input, integer values as strings. Conversion between
// integer and string are provided and a comparator (<) for sorting.
class IntString {
    string str;
    int num;
    
public:
    IntString (int a) : str(std::to_string(a)), num(a) {};
    IntString (string s) : str(s), num(std::stoi(s)) {};
    string as_string() const { return this->str; }
    int as_int() const { return this->num; }
    bool operator< (const IntString&) const;
};

bool IntString::operator< (const IntString &other) const {
    int lhs = std::stoi(this->str + other.str);
    int rhs = std::stoi(other.str + this->str);
    return (lhs < rhs);
}

string concatenate_vector(vector<string> a) {
    string result = "";
    for (size_t i = 0; i < a.size(); ++i) {
        result += a[i];
    }
    return result;
}

string largest_number(vector<string> &a) {
    // use the IntString class to sort
    vector<IntString> ais;
    for (size_t i = 0; i < a.size(); ++i) {
        ais.push_back(IntString(a[i]));
    }
    std::sort(ais.begin(), ais.end());
    std::reverse(ais.begin(), ais.end());
    
    // reuse the original string vector
    for (size_t i = 0; i < a.size(); ++i) {
        a[i] = ais[i].as_string();
    }
    return concatenate_vector(a);
}

int main() {
    int n;
    std::cin >> n;
    vector<string> a(n);
    for (size_t i = 0; i < a.size(); i++) {
        std::cin >> a[i];
    }
    std::cout << largest_number(a);
}
