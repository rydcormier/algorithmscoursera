//
//  connected_components.cpp
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

int number_of_components(const vector<vector<int> > &adj) {
    int res = 0;
    vector<bool> visited(adj.size(), false);
    for (int v = 0; v < adj.size(); v++) {
        if (! visited[v]) {
            explore(v, adj, visited);
            res++;
        }
    }
    return res;
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
    std::cout << number_of_components(adj);
}
