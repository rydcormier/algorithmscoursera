//
//  kmp.cpp
//  Programming Assignment 3 - Algorithms on Strings
//
//  Created by Ryan Cormier on 2/5/19.
//

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::string;
using std::vector;

// Compute a prefix function for the given string
vector<int> compute_prefix_function(const string& text)
{
    vector<int> result(text.length());
    result[0] = 0;
    int border = 0;
    
    for (int i = 1; i < text.length(); ++i)
    {
        while (border > 0 && text[i] != text[border])
        {
            border = result[border - 1];
        }
        
        if (text[i] == text[border])
        {
            border++;
        }
        
        else
        {
            border = 0;
        }
        
        result[i] = border;
    }
    
    return result;
}

// Find all occurrences of the pattern in the text and return a
// vector with all positions in the text (starting from 0) where
// the pattern starts in the text.
vector<int> find_pattern(const string& pattern, const string& text)
{
    vector<int> result;
    int p = (int) pattern.length();
    string str = pattern + '$' + text;
    vector<int> prefix_function = compute_prefix_function(str);
    
    for (int i = p + 1; i < str.length(); ++i)
    {
        if (prefix_function[i] == p)
        {
            result.push_back(i - (2 * p));
        }
    }
    
    return result;
}

int main()
{
    string pattern, text;
    cin >> pattern;
    cin >> text;
    vector<int> result = find_pattern(pattern, text);
    
    for (int i = 0; i < result.size(); ++i)
    {
        printf("%d ", result[i]);
    }
    
    printf("\n");
    return 0;
}

