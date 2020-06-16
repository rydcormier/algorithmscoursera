//
//  suffix_array.cpp
//  Programming Assignment 2 - Algorithms on Strings
//
//  Created by Ryan Cormier on 2/5/19.
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
using std::sort;
using std::string;
using std::vector;


vector<string> cyclic_rotations(const string &text)
{
    vector<string> result(text.length());
    result[0] = text;
    size_t l = text.length();
    
    for (size_t i = 1; i < l; i++)
    {
        result[i] = text.substr(l - i) + text.substr(0, l - i);
    }
    
    return result;
}

/*
 *  Build suffix array of the string text and return a vector result of the same
 *  length as the text such that the value result[i] is the index in text where
 *  the i-th lexicographically smallest suffix of text starts.
 */
vector<size_t> BuildSuffixArray(const string& text)
{
    vector<size_t> result;
    size_t length = text.length();
    vector<string> rotations = cyclic_rotations(text);
    
    sort(rotations.begin(), rotations.end());
    
    for (vector<string>::iterator it = rotations.begin();
         it != rotations.end(); ++it)
    {
        result.push_back(length - 1 - it->find('$'));
    }
    
    return result;
}

int main()
{
    string text;
    cin >> text;
    vector<size_t> suffix_array = BuildSuffixArray(text);
    
    for (int i = 0; i < suffix_array.size(); ++i)
    {
        cout << suffix_array[i] << ' ';
    }
    
    cout << endl;
    return 0;
}

