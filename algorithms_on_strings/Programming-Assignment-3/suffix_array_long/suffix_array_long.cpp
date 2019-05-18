//
//  suffix_array_long.cpp
//  Programming Assignment 3 - Algorithms on Strings
//
//  Created by Ryan Cormier on 5/18/19.
//

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

using std::cin;
using std::cout;
using std::endl;
using std::make_pair;
using std::pair;
using std::string;
using std::vector;

const size_t NUM_CHARS = 5;

size_t get_char_index(char c)
{
    switch (c)
    {
        case 'A':
            return 1;
            break;
            
        case 'C':
            return 2;
            break;
            
        case 'G':
            return 3;
            break;
            
        case 'T':
            return 4;
            break;
            
        default:
            return 0;
            break;
    }
}

void print_ordered_text(const string& text, const vector<int>& order)
{
    for (size_t i = 0; i < text.length(); ++i)
    {
        cout << text[order[i]];
    }
    
    cout << endl;
}

void print_vector(const vector<int>& v)
{
    cout << "[";
    for (size_t i = 0; i < v.size(); ++i)
    {
        cout << "  " << v[i];
    }
    
    cout << "  ]" << endl;
}

string get_cyclic_shift(string text, size_t start, size_t length)
{
    while (start + length >= text.length())
    {
        text = text + text;
    }
    
    return text.substr(start, length);
}

void print_ordered_shifts(const string& text, const vector<int>& order, size_t l)
{
    for (size_t i = 0; i < order.size(); ++i)
    {
        cout << get_cyclic_shift(text, order[i], l) << '\n';
    }
    
    cout << endl;
}

// A stable sort returning indeces of texted order by the corresponding characters.
vector<int> sort_characters(const string& text)
{
    vector<int> order(text.length());
    vector<int> count(NUM_CHARS);
    
    for (size_t i = 0; i < text.length(); ++i)
    {
        count[get_char_index(text[i])] += 1;
    }
    
    for (size_t j = 1; j < NUM_CHARS; ++j)
    {
        count[j] = count[j] + count[j - 1];
    }
    
    int i = (int) text.length();
    
    while (i > 0)
    {
        --i;
        int c = get_char_index(text[i]);
        count[c] -= 1;
        order[count[c]] = i;
    }
    
    return order;
}

// Returns the equivalence classes of the characters of text.
vector<int> compute_character_classes(const string& text, const vector<int>& order)
{
    vector<int> classes(text.length());
    
    return classes;
}

// Given the order of partial cyclic shifts, return the sorted indeces of doubled cyclic
// shifts.
vector<int> sort_doubled(const string& text, size_t length, const vector<int>& order, const vector<int>& classes)
{
    vector<int> new_order(text.length());
    
    return new_order;
}

// Update the equivalence classes of ordered cyclic shifts.
vector<int> update_classes(const vector<int>& order, const vector<int> classes, size_t length)
{
    size_t n = order.size();
    vector<int> new_classes(n);
    
    return new_classes;
}

// Build suffix array of the string text and
// return a vector result of the same length as the text
// such that the value result[i] is the index (0-based)
// in text where the i-th lexicographically smallest
// suffix of text starts.
vector<int> BuildSuffixArray(const string& text) {
    vector<int> result = sort_characters(text);
    vector<int> classes = compute_character_classes(text, result);
    size_t l = 1;
    
    while (l < text.length())
    {
        result = sort_doubled(text, l, result, classes);
        classes = update_classes(result, classes, l);
        l = 2 * l;
    }
    
    return result;
}

int main() {
    string text;
    cin >> text;
    //    vector<int> suffix_array = BuildSuffixArray(text);
    //    for (int i = 0; i < suffix_array.size(); ++i) {
    //        cout << suffix_array[i] << ' ';
    //    }
    
    vector<int> order = sort_characters(text);
    print_vector(order);
    print_ordered_text(text, order);
    print_ordered_shifts(text, order, 2);
    cout << endl;
    return 0;
}

