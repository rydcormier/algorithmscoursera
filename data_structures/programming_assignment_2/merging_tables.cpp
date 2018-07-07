/**
 *  merging_tables.cpp
 *  Data Structures: Programming Assignment 2
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date:   May 3, 2018
 **/

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::max;
using std::vector;

struct DisjointSetsElement {
    int size, parent, rank;
    
    DisjointSetsElement(int size = 0, int parent = -1, int rank = 0):
    size(size), parent(parent), rank(rank) {}
};

struct DisjointSets {
    int size;
    int max_table_size;
    vector <DisjointSetsElement> sets;
    
    DisjointSets(int size): size(size), max_table_size(0), sets(size) {
        for (int i = 0; i < size; i++)
            sets[i].parent = i;  // each element is a root
    }
    
    int getParent(int table) {
        //find parent and compress path
        if (table != sets[table].parent) {
            sets[table].parent = getParent(sets[table].parent);
        }
        return sets[table].parent;
    }
    
    void merge(int destination, int source) {
        int realDestination = getParent(destination);
        int realSource = getParent(source);
        if (realDestination != realSource) {
            // merge two components
            // use union by rank heuristic
            if (sets[realDestination].rank > sets[realSource].rank) {
                // hang source from destination
                sets[realSource].parent = realDestination;
                
                // update size and max size
                sets[realDestination].size += sets[realSource].size;
                max_table_size = max(max_table_size, sets[realDestination].size);
                
            } else {
                // hang destination from source
                sets[realDestination].parent = realSource;
                
                // update size and max size
                sets[realSource].size += sets[realDestination].size;
                max_table_size = max(max_table_size, sets[realSource].size);
                
                // update rank if needed
                if (sets[realDestination].rank == sets[realSource].rank) {
                    sets[realSource].rank += 1;
                }
            }
        }
    }
};

int main() {
    int n, m;
    cin >> n >> m;
    
    DisjointSets tables(n);
    for (auto &table : tables.sets) {
        cin >> table.size;
        tables.max_table_size = max(tables.max_table_size, table.size);
    }
    
    for (int i = 0; i < m; i++) {
        int destination, source;
        cin >> destination >> source;
        --destination;
        --source;
        
        tables.merge(destination, source);
        cout << tables.max_table_size << endl;
    }
    
    return 0;
}

