//
//  bwmatching.cpp
//  Programming Assignment 2 - Algorithms on Strings
//
//  Created by Ryan Cormier on 2/4/19.
//

#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::istringstream;
using std::map;
using std::string;
using std::vector;

const string Chars = "$ACGT";

// Preprocess the Burrows-Wheeler Transform bwt of some text
// and compute as a result:
//   * starts - for each character C in bwt, starts[C] is the first position
//       of this character in the sorted array of
//       all characters of the text (aka. the first column).
//   * occ_count_before - for each character C in bwt and each position P in bwt,
//       occ_count_before[C][P] is the number of occurrences of character C in bwt
//       from position 0 to position P inclusive.
void PreprocessBWT(const string& bwt,
                   map<char, int>& starts,
                   map<char, vector<int> >& occ_count_before)
{
    size_t length = bwt.length();
    map<char, int> counts;
    
    // Initialize counts
    for (size_t i = 0; i < 5; ++i)
    {
        occ_count_before[Chars[i]] = vector<int>(length + 1, 0);
        counts[Chars[i]] = 0;
    }
    
    
    // parse bwt and update counts
    for (size_t pos = 0; pos < bwt.length(); ++pos)
    {
        char c = bwt[pos];
        counts[c] += 1;
        
        for (map<char, vector<int> >::iterator it = occ_count_before.begin();
             it != occ_count_before.end(); ++it)
        {
            it->second[pos + 1] = counts[it->first];
        }
    }
    
    // remove unused characters
    string erase_chars;
    for (auto it = occ_count_before.begin(); it != occ_count_before.end(); ++it)
    {
        if (it->second.back() == 0)
            erase_chars.push_back(it->first);
    }
    
    for (auto it = erase_chars.begin(); it != erase_chars.end(); ++it)
    {
        occ_count_before.erase(*it);
    }
    
    // the start position of a character in a sorted array is the sum of the counts of
    // of the preceeding (alphabetically) characters
    int sum = 0;
    
    for (auto it = occ_count_before.begin(); it != occ_count_before.end(); ++it)
    {
        starts[it->first] = sum;
        sum += it->second.back();
    }
}

// Compute the number of occurrences of string pattern in the text
// given only Burrows-Wheeler Transform bwt of the text and additional
// information we get from the preprocessing stage - starts and occ_counts_before.
int CountOccurrences(const string& pattern,
                     const string& bwt,
                     const map<char, int>& starts,
                     const map<char, vector<int> >& occ_count_before)
{
    size_t length = bwt.length();
    int top = 0;
    int bottom = (int) length - 1;
    size_t pattern_length = pattern.length();
    
    while (top <= bottom)
    {
        bool found = false;
        
        if (pattern_length > 0)
        {
            char c = pattern.substr(0, pattern_length).back();
            --pattern_length;
            
            for (size_t pos = top; pos <= bottom; ++pos)
            {
                if (bwt[pos] == c)
                {
                    top = starts.at(c) + occ_count_before.at(c)[top];
                    bottom = starts.at(c) + occ_count_before.at(c)[bottom + 1] - 1;
                    found = true;
                    break;
                }
            }
            
            if (!found)
            {
                return 0;
            }
        }
        
        else
        {
            return bottom - top + 1;
        }
    }
    
    return 0;
}


int main() {
    string bwt;
    cin >> bwt;
    int pattern_count;
    cin >> pattern_count;
    
    // Start of each character in the sorted list of characters of bwt,
    // see the description in the comment about function PreprocessBWT
    map<char, int> starts;
    
    // Occurrence counts for each character and each position in bwt,
    // see the description in the comment about function PreprocessBWT
    map<char, vector<int> > occ_count_before;
    
    // Preprocess the BWT once to get starts and occ_count_before.
    // For each pattern, we will then use these precomputed values and
    // spend only O(|pattern|) to find all occurrences of the pattern
    // in the text instead of O(|pattern| + |text|).
    PreprocessBWT(bwt, starts, occ_count_before);
    
    for (int pi = 0; pi < pattern_count; ++pi)
    {
        string pattern;
        cin >> pattern;
        int occ_count = CountOccurrences(pattern, bwt, starts, occ_count_before);
        printf("%d ", occ_count);
    }
    printf("\n");
    return 0;
}
