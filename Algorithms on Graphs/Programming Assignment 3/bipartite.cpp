//
//  bipartite.cpp
//  Algorithms on Graphs - Programming Assignment 3
//
//  Created by Ryan Cormier on 8/4/18.
//

#include <iostream>
#include <vector>
#include <queue>

using std::vector;
using std::queue;

struct Graph {
    vector<vector<int> > adjacency_list;
    Graph(vector<vector<int> > &adj) : adjacency_list(adj) {}
    
    vector<int> distances_from_source(int s) {
        // returns a vector of the distances from s for all nodes in graph.
        // The distance for unreachable nodes is given as -1.
        vector<int> dist(adjacency_list.size(), -1);
        dist[s] = 0;
        queue<int> q;
        q.push(s);
        while (! q.empty()) {
            int u = q.front(); q.pop();
            for (vector<int>::iterator v = adjacency_list[u].begin();
                 v != adjacency_list[u].end(); v++) {
                if (dist[*v] == -1) {  // not discovered
                    q.push(*v);
                    dist[*v] = dist[u] + 1;
                }
            }
        }
        return dist;
    }
    
    bool is_bipartite() {
        // uses a similar algorithm to a breadth-first search with distances.
        // instead of tracking distances, track color and make sure they are
        // oposite.
        vector<bool> color(adjacency_list.size());
        vector<bool> visited(adjacency_list.size(), false);
        color[0] = false;
        visited[0] = true;
        queue<int> q;
        q.push(0);
        while (! q.empty()) {
            int u = q.front(); q.pop();
            for (vector<int>::iterator v = adjacency_list[u].begin();
                 v != adjacency_list[u].end(); v++) {
                if (!visited[*v]) {
                    q.push(*v);
                    visited[*v] = true;
                    color[*v] = !color[u];
                } else {
                    if (color[u] == color[*v]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
};

int main() {
    int n, m;
    std::cin >> n >> m;
    vector<vector<int> > adj(n, vector<int>());
    for (int i = 0; i < m; i++) {
        int x, y;
        std::cin >> x >> y;
        adj[x - 1].push_back(y - 1);
        adj[y - 1].push_back(x - 1);
    }
    std::cout <<  Graph(adj).is_bipartite();
}
