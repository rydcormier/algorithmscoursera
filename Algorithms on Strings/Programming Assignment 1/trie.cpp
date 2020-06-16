//
//  trie.cpp
//  Algorithms on Strings - Programming Assignment 1
//
//  Created by Ryan Cormier on 1/10/19.
//

#include <string>
#include <iostream>
#include <vector>
#include <map>

using std::map;
using std::vector;
using std::string;

typedef map<char, int> edges;
typedef vector<edges> trie;

trie build_trie(vector<string> & patterns) {
    trie t;
    for (size_t i = 0; i < patterns.size(); ++i) {
        int current_node_key = 0;
        
        for (size_t j = 0; j < patterns[i].size(); ++j) {
            
            char current_char = patterns[i][j];
            
            if (t.empty()) {
                
                edges e;
                e[current_char] = 1;
                t.push_back(e);
                t.push_back(edges());
                current_node_key = 1;
                
            } else {
                
                // check if there is a matching edge
                bool match = false;
                
                for (map<char, int>::iterator iter = t[current_node_key].begin();
                     iter != t[current_node_key].end(); ++iter) {
                    
                    if (iter->first == current_char) {
                        
                        current_node_key = iter->second;
                        match = true;
                        break;
                        
                    }
                }
                
                if (!match) {
                    
                    // add new node to trie;
                    t.push_back(edges());
                    int new_node_key = (int)t.size() - 1;
                    t[current_node_key][current_char] = new_node_key;
                    
                    current_node_key = new_node_key;
                }
            }
        }
    }
    return t;
}

int main() {
    size_t n;
    std::cin >> n;
    vector<string> patterns;
    for (size_t i = 0; i < n; i++) {
        string s;
        std::cin >> s;
        patterns.push_back(s);
    }
    
    trie t = build_trie(patterns);
    for (size_t i = 0; i < t.size(); ++i) {
        for (const auto & j : t[i]) {
            std::cout << i << "->" << j.second << ":" << j.first << "\n";
        }
    }
    
    return 0;
}
