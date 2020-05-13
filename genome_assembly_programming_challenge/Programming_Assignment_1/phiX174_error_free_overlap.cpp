//
//  phiX174_error_free_overlap.cpp
//  Genome Assembly Programming Challenge - Programming Assignment 1
//
//  Created by Ryan Cormier on 5/10/20.
//

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int NUM_READS = 1168;
const int READ_SIZE = 100;

typedef vector< vector<int>> Graph;

// Find maximum n such that the last n characters of a and the first n
// characters of b are the same.
int overlap(const string &a, const string &b) {
    int n = (int) b.size();
    for (int i = 1; i < n; i++) {
        if (a.substr(i) == b.substr(0, n - i))
            return n - i;
    }
    return 0;
}

// Returns true if a and b share a substring of size k, false otherwise.
bool share_kmer(const string &a, const string &b, int k) {
    int n = (int) a.size();
    
    for (int i = 0; i <= n - k; i++) {
        if (b.find(a.substr(i, k)) != string::npos)
            return true;
    }
    
    return false;
}

// Create an overlap graph from the given reads.
Graph overlap_graph(const vector<string> &reads) {
    Graph G(reads.size(), vector<int>(reads.size(), 0));
    
    // first only find overlaps for pairs of reads that share a 12-mer
    vector< pair<int, int>> pairs;
    
    for (int i = 0; i < NUM_READS - 1; i++) {
        for (int j = i + 1; j < NUM_READS; j++) {
            if (share_kmer(reads[i], reads[j], 12)) {
                pairs.push_back(make_pair(i, j));
            }
        }
    }
    
    // connect pairs of vertices with edge weight equal to overlap
    for (int i = 0; i < pairs.size(); i++) {
        int u = pairs[i].first;
        int v = pairs[i].second;
        
        G[u][v] = overlap(reads[u], reads[v]);
        G[v][u] = overlap(reads[v], reads[u]);
    }
    
    return G;
}

// Recursive DFS like search looking for a hamiltonian path in G.
void visit(const Graph &G, int u, vector<bool> &visited, vector<int> &path) {
    visited[u] = 1;
    path.push_back(u);
    if (path.size() == NUM_READS)   // we're done
        return;
    vector<pair<int, int>> outgoing_edges;  // store {weight, vertex} pairs
    for (int i = 0; i < NUM_READS; i++) {
        if (G[u][i] > 0)
            outgoing_edges.push_back(make_pair(G[u][i], i));
    }
    sort(outgoing_edges.begin(), outgoing_edges.end());
    reverse(outgoing_edges.begin(), outgoing_edges.end()); // now sorted by
                                                           // decreasing weight.
    for (vector< pair<int, int>>::iterator e = outgoing_edges.begin();
         e != outgoing_edges.end(); e++) {
        if (path.size() != NUM_READS && ! visited[e->second])
            visit(G, e->second, visited, path);
    }
    
    if (path.size() != NUM_READS)   // reached a dead end => backtrack
        path.pop_back();
}

// Returns a sequence of vertcices corresponding to a hamiltonian path on G.
vector<int> find_path(const Graph &G) {
    vector<int> path;
    
    for (int i = 0; i < NUM_READS; i++) {
        if (path.size() == NUM_READS)
            break;
        path.clear();
        vector<bool> visited(NUM_READS, 0);
        visit(G, i, visited, path);
    }
    return path;
}

int main() {
    vector<string> reads(NUM_READS);
    
    for (int i = 0; i < NUM_READS; i++) {
        cin >> reads[i];
    }
    
    Graph g = overlap_graph(reads);
    
    vector<int> path = find_path(g);
    int u, v, w;
    for (int i = 0; i < NUM_READS - 1; i++) {
        u = path[i];
        v = path[i + 1];
        w = g[u][v];
        
        cout << reads[path[i]].substr(0, READ_SIZE - w);
    }
    
    u = path.back();
    v = path.front();
    w = g[u][v];
    
    cout << reads[u].substr(0, READ_SIZE - w) << endl;
    
    return 0;
}
