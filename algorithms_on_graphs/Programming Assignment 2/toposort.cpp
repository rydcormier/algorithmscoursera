//
//  toposort.cpp
//  Algorithms on Graphs - Programming Assignment 2
//
//  Created by Ryan Cormier on 7/31/18.
//

#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;
using std::pair;

struct Vertex {
    int key;
    int pre;
    int post;
    
    Vertex() : key(0), pre(0), post(0) {};
    Vertex(int x, int y, int z) : key(x), pre(y), post(z) {};
    
};

bool linear_compare(Vertex v, Vertex w) { return v.post > w.post; }

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
    
    vector<Vertex> linear_order() {
        vector<Vertex> res(vector<Vertex>(this->size()));
        for (int i = 0; i < this->size(); i++) {
            res[i] = Vertex(i, pre[i], post[i]);
        }
        std::sort(res.begin(), res.end(), linear_compare);
        return res;
    }
};

void depth_first_search(Graph &graph) {
    for (int v = 0; v < graph.size(); v++) {
        if (! graph.visited[v]) {
            graph.explore(v);
        }
    }
}

vector<int> toposort(vector<vector<int> > &adj) {
    Graph graph(adj);
    depth_first_search(graph);  // this sets pre and post orders
    vector<Vertex> vertices_sorted = graph.linear_order();
    vector<int> order;
    for (vector<Vertex>::iterator it = vertices_sorted.begin();
         it < vertices_sorted.end(); it++) {
        order.push_back(it->key);
    }
    return order;
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
    vector<int> order = toposort(adj);
    for (size_t i = 0; i < order.size(); i++) {
        std::cout << order[i] + 1 << " ";
    }
}


