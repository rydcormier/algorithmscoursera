//  plan_party.cpp
//
//  Advanced Algorithms and Complexity
//  Programming Assignment 4: Problem 2
//
//  Created by Ryan Cormier on 4/29/20.
//

#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// store the tree as an undirected graph
struct Graph {
    int V;
    vector< vector<int>> adj;
    vector<int> weights;
    
    Graph(int n) : V(n), adj(n + 1), weights(n + 1) {}
    
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
};

// Recursive function to find an optimized weighted independent set on a tree.
int max_weighted_ind_set(Graph &T, vector<int> &D, vector<bool> &on_stack, int u) {
    if (D[u] < 0) {
        vector<int> children;
        for (int i = 0; i < T.adj[u].size(); i++) {
            int v = T.adj[u][i];
            if (! on_stack[v])
                children.push_back(v);
        }
        if (children.empty()) {     // leaf
            D[u] = T.weights[u];
        } else {
            on_stack[u] = true;
            
            int m1 = T.weights[u]; // add u and grandchildren
            for (int i = 0; i < children.size(); i++) {
                int v = children[i];
                on_stack[v] = true;
                for (int j = 0; j < T.adj[v].size(); j++) {
                    int w = T.adj[v][j];
                    if (! on_stack[w])
                        m1 += max_weighted_ind_set(T, D, on_stack, w);
                }
                on_stack[v] = false;
            }
            
            int m0 = 0; // skip u and add children
            for (int i = 0; i < children.size(); i++) {
                m0 += max_weighted_ind_set(T, D, on_stack, children[i]);
            }
            on_stack[u] = false;
            D[u] = max(m1, m0);
        }
    }
    return D[u];
}

int main() {
    int n, u, v;
    cin >> n;
    
    Graph T(n);
    
    for (int i = 1; i <= n; i++) {
        cin >> T.weights[i];
    }
    
    for (int i = 1; i < n; i++) {
        cin >> u >> v;
        T.add_edge(u, v);
    }
    
    int opt = 0;
    vector<int> D(n + 1, -1);
    vector<bool> on_stack(n + 1, false);
    for (int i = 1; i <= n; i++) {
        opt = max(opt, max_weighted_ind_set(T, D, on_stack, i));
    }
    
    cout << opt << endl;
}
