//
//  reachability.cpp
//  Algorithms on Graphs - Programming Assignment 1
//
//  Created by Ryan Cormier on 7/30/18.
//

#include <iostream>
#include <vector>

using std::vector;
using std::pair;

void explore(int v, const vector<vector<int>> &adj, vector<bool> &visited) {
    // Given an adjaceny list representation of a Graph, explore from vertice v.
    visited[v] = true;
    vector<int> neighbors = adj[v];
    for (size_t i = 0; i < neighbors.size(); i++) {
        int neighbor = neighbors[i];
        if (! visited[neighbor]) {
            explore(neighbor, adj, visited);
        }
    }
}

int reach(const vector<vector<int> > &adj, int x, int y) {
    // Simply explore and see if y is visited.
    vector<bool> visited(adj.size(), false);
    explore(x, adj, visited);
    return visited[y];
}

int main() {
    size_t n, m;
    std::cin >> n >> m;
    vector<vector<int> > adj(n, vector<int>());
    for (size_t i = 0; i < m; i++) {
        int x, y;
        std::cin >> x >> y;
        adj[x - 1].push_back(y - 1);
        adj[y - 1].push_back(x - 1);
    }
    int x, y;
    std::cin >> x >> y;
    std::cout << reach(adj, x - 1, y - 1);
}

