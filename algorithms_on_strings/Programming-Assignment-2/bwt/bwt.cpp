//
//  bwt.cpp
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
using std::string;
using std::vector;

vector<string> cyclic_rotations(const string &text)
{
    vector<string> result(text.length());
    result.push_back(text);
    size_t l = text.length();
    
    for (size_t i = 1; i < l; i++)
    {
        result.push_back(text.substr(l - i) + text.substr(0, l - i));
    }
    
    return result;
}

string BWT(const string& text)
{
    string result = "";
    
    // create cyclic rotations and sort
    vector<string> rotations = cyclic_rotations(text);
    std::sort(rotations.begin(), rotations.end());
    
    for (size_t i = 0; i < rotations.size(); ++i)
    {
        result += rotations[i].back();
    }
    
    return result;
}

int main()
{
    string text;
    cin >> text;
    cout << BWT(text) << endl;
    return 0;
}
