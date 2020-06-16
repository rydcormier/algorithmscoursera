//
//  trie_matching.cpp
//  Algorithms on Strings - Programming Assignment 1
//
//  Created by Ryan Cormier on 1/14/19.
//
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int const Letters =    4;
int const NA      =   -1;

struct Node
{
    size_t key;
    int next [Letters];
    bool pattern_end;
    
    Node ()
    {
        fill (next, next + Letters, NA);
        pattern_end = false;
    }
    
    Node (size_t k)
    {
        key = k;
        fill (next, next + Letters, NA);
        pattern_end = false;
    }
};

bool good_letter(char letter)
{
    switch (letter)
    {
        case 'A': return true; break;
        case 'C': return true; break;
        case 'G': return true; break;
        case 'T': return true; break;
        default :  return false;
    }
}

int letter_to_index (char letter)
{
    switch (letter)
    {
        case 'A': return 0; break;
        case 'C': return 1; break;
        case 'G': return 2; break;
        case 'T': return 3; break;
        default: assert (false); return -1;
    }
}

typedef vector<Node> Trie;

Trie build_trie(const vector<string> & patterns)
{
    Trie trie;
    trie.push_back(Node(0));
    
    for (size_t i = 0; i < patterns.size(); ++i)
    {
        Node current_node = trie[0];
        string pattern = patterns[i];
        
        for (size_t j = 0; j < pattern.size(); ++j)
        {
            int current_letter = letter_to_index(pattern[j]);
            
            if (current_node.next[current_letter] != NA)
            {
                current_node = trie[current_node.next[current_letter]];
                
                if (j == pattern.size() - 1) // end of pattern
                {
                    trie[current_node.key].pattern_end = true;
                }
            }
            else
            {
                Node new_node((int) trie.size());
                
                if (j == pattern.size() - 1) // end of pattern
                {
                    new_node.pattern_end = true;
                }
                
                trie.push_back(new_node);
                trie[current_node.key].next[current_letter] = (int) new_node.key;
                
                current_node = new_node;
            }
        }
    }
    return trie;
}

bool prefix_trie_match(const string &text, const Trie &trie)
{
    size_t pos = 0;
    Node current_node = trie[0];
    
    while (true)
    {
        if (current_node.pattern_end)
        {
            return true;
        }
        else if (!good_letter(text[pos]) ||
                 current_node.next[letter_to_index(text[pos])] == NA)
        {
            return false;
        }
        else
        {
            current_node = trie[current_node.next[letter_to_index(text[pos])]];
            ++pos;
        }
    }
}

vector <size_t> solve (const string& text, int n, const vector <string>& patterns)
{
    vector<size_t> result;
    Trie trie = build_trie(patterns);
    size_t pos = 0;
    
    while (pos < text.size())
    {
        if (prefix_trie_match(text.substr(pos), trie))
        {
            result.push_back(pos);
        }
        
        ++pos;
    }
    
    return result;
}

int main (void)
{
    string text;
    cin >> text;
    
    int n;
    cin >> n;
    
    vector <string> patterns (n);
    for (int i = 0; i < n; i++)
    {
        cin >> patterns[i];
    }
    
    vector <size_t> ans;
    ans = solve (text, n, patterns);
    
    for (int i = 0; i < (int) ans.size (); i++)
    {
        cout << ans[i];
        if (i + 1 < (int) ans.size ())
        {
            cout << " ";
        }
        else
        {
            cout << endl;
        }
    }
    
    return 0;
}
