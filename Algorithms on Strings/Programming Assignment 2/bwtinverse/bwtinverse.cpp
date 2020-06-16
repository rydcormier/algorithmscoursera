//
//  bwtinverse.cpp
//  Programming Assignment 2 - Algorithms on Strings
//
//  Created by Ryan Cormier on 1/31/19.
//

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::sort;
using std::string;
using std::vector;

size_t char_to_index(const char &c)
{
    switch (c)
    {
        case 'A':
            return 0;
            break;
            
        case 'C':
            return 1;
            break;
            
        case 'G':
            return 2;
            break;
            
        case 'T':
            return 3;
            break;
            
        default:
            return 4;
            break;
    }
}

vector<size_t> make_last_to_first(const string &first, const string &bwt)
{
    size_t l = bwt.length();
    vector<size_t> result(l);
    
    int last_occurrence [] = {-1, -1, -1, -1, -1};
    
    for (size_t i = 0; i < l; ++i)
    {
        char c = first[i];
        size_t index = char_to_index(c);
        
        int pos = last_occurrence[index] + 1;
        
        
        while (bwt[pos] != c)
        {
            ++pos;
        }
        
        last_occurrence[index] = pos;
        result[pos] = i;
    }
    
    return result;
    
}

string BWTInverse(const string &bwt)
{
    size_t length = bwt.length();
    string result(length, '\0');
    string first(bwt);
    
    sort(first.begin(), first.end());
    
    vector<size_t> last_to_first = make_last_to_first(first, bwt);
    
    size_t current_row = 0;
    size_t pos = length - 1;
    
    while (true)
    {
        result[pos] = first[current_row];
        current_row = last_to_first[current_row];
        
        if (pos == 0)
        {
            break;
        }
        
        --pos;
    }
    
    return result;
}

int main() {
    string bwt;
    cin >> bwt;
    cout << BWTInverse(bwt) << endl;
    return 0;
}

