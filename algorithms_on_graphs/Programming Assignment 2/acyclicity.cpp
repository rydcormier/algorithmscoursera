//
//  acyclicity.cpp
//  Algorithms on Graphs - Programming Assignment 2
//
//  Created by Ryan Cormier on 7/31/18.
//

#include <iostream>
#include <vector>

using std::vector;
using std::pair;

struct Graph {
    vector<vector<int> > *adjacency_list;
    vector<bool> visited;
    vector<int> pre;
    vector<int> post;
    int clock;
    
    Graph(vector<vector<int>> &adj) {
        clock = 1;
        adjacency_list = &adj;
        visited = vector<bool>(adj.size(), false);
        pre = vector<int>(adj.size(), 0);
        post = vector<int>(adj.size(), 0);
    }
    
    size_t size() { return adjacency_list->size(); }
    
    void previsit(int v) {
        pre[v] = clock;
        clock = clock + 1;
    }
    
    void postvisit(int v) {
        post[v] = clock;
        clock = clock + 1;
    }
    
    void explore(int v) {
        visited[v] = true;
        previsit(v);
        for (size_t i = 0; i < adjacency_list->at(v).size(); i++) {
            int w = adjacency_list->at(v)[i];
            if (! visited[w]) {
                explore(w);
            }
        }
        postvisit(v);
    }
};

void depth_first_search(Graph &graph) {
    for (int v = 0; v < graph.size(); v++) {
        if (! graph.visited[v]) {
            graph.explore(v);
        }
    }
}

int acyclic(vector<vector<int> > &adj) {
    // run depth first search and check post orders
    Graph graph(adj);
    depth_first_search(graph);
    for (int u = 0; u < adj.size(); u++) {
        for (int i = 0; i < adj[u].size(); i++) { // not a dag
            int v = adj[u][i];
            if (graph.post[u] < graph.post[v]) {
                return 1;
            }
        }
    }
    return 0;
}

int main() {
    size_t n, m;
    std::cin >> n >> m;
    vector<vector<int> > adj(n, vector<int>());
    for (size_t i = 0; i < m; i++) {
        int x, y;
        std::cin >> x >> y;
        adj[x - 1].push_back(y - 1);
    }
    std::cout << acyclic(adj);
}
