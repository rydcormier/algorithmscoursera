//
//  strongly_connected.cpp
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
    vector<vector<int> > adjacency_list;
    vector<bool> visited;
    vector<int> pre;
    vector<int> post;
    int clock;
    
    Graph(vector<vector<int>> &adj) : adjacency_list(adj) {
        clock = 1;
        visited = vector<bool>(adj.size(), false);
        pre = vector<int>(adj.size(), 0);
        post = vector<int>(adj.size(), 0);
    }
    
    size_t size() { return adjacency_list.size(); }
    
    void print_adjacency_list() {
        for (int i = 0; i < this->size(); i++) {
            std::cout << i + 1 << ": ";
            for (int j = 0; j < adjacency_list[i].size(); j++) {
                if (j > 0) {
                    std::cout << ", ";
                }
                std::cout << adjacency_list[i][j] + 1;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    
    void reverse() {
        // reverses all the edges
        vector<vector<int> > new_adj(this->size(), vector<int>());
        for (int i = 0; i < this->size(); i++) {
            for (int j = 0; j < adjacency_list[i].size(); j++) {
                new_adj[adjacency_list[i][j]].push_back(i);
            }
        }
        adjacency_list.swap(new_adj);
    }
    
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
        for (size_t i = 0; i < adjacency_list[v].size(); i++) {
            int w = adjacency_list[v][i];
            if (! visited[w]) {
                explore(w);
            }
        }
        postvisit(v);
    }
    
    vector<Vertex> linear_order() {
        vector<Vertex> res(this->size());
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

int number_of_strongly_connected_components(vector<vector<int> > adj) {
    int result = 0;
    Graph graph(adj);
    
    // reverse and run dfs
    graph.reverse();
    depth_first_search(graph);
    
    // linear order is reverse post order
    vector<Vertex> vertices = graph.linear_order();
    
    // reverse again and reset visited to compute sccs
    graph.reverse();
    graph.visited = vector<bool>(graph.size(), false);
    
    // run expolore by the reverse linear order
    for (vector<Vertex>::iterator it = vertices.begin(); it < vertices.end(); it++) {
        int v = it->key;
        if (! graph.visited[v]) {
            graph.explore(v);
            result++;
        }
    }
    return result;
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
    std::cout << number_of_strongly_connected_components(adj);
}
