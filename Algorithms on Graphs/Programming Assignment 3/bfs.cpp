//
//  bfs.cpp
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
};

int distance(vector<vector<int> > &adj, int s, int t) {
    return Graph(adj).distances_from_source(s)[t];
}

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
    int s, t;
    std::cin >> s >> t;
    s--; t--;
    std::cout << distance(adj, s, t);
}
