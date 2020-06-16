//
//  negative_cycle.cpp
//
//  Algorithms on Graphs - Programming Assignment 4
//
//  Created by Ryan Cormier on 8/15/18.
//

#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using std::vector;
using std::queue;

int negative_cycle(vector<vector<int> > &adj, vector<vector<int> > &cost) {
    // Run |V| iterations of Bellman-Ford algorithm on an unexplored vertex, and if a
    // distance is updated on last iteration, a negative cycle exists.
    vector<int> dist(adj.size(), std::numeric_limits<int>::max());
    
    for (int s = 0; s < adj.size(); s++) {
        // check if we've been here
        if (dist[s] < std::numeric_limits<int>::max()) {
            continue;
        }
        
        dist[s] = 0;
        bool dist_updated = false;
        
        for (int i = 0; i < adj.size(); i++) {
            dist_updated = false;
            queue<int> q;
            q.push(s);
            vector<bool> visited(adj.size(), false);
            visited[s] = true;
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (int j = 0; j < adj[u].size(); j++) {
                    int v = adj[u][j];
                    int w = cost[u][j];
                    if (!visited[v]) {
                        q.push(v);
                    }
                    if (dist[v] > dist[u] + w) {
                        dist[v] = dist[u] + w;
                        dist_updated = true;
                    }
                    visited[v] = true;
                }
            }
            if (!dist_updated) {  // no change means we can break early
                break;
            }
        }
        // if an edge was relaxed, we have a negative cycle
        if (dist_updated) {
            return 1;
        }
    }
    return 0;
}

int main() {
    int n, m;
    std::cin >> n >> m;
    vector<vector<int> > adj(n, vector<int>());
    vector<vector<int> > cost(n, vector<int>());
    for (int i = 0; i < m; i++) {
        int x, y, w;
        std::cin >> x >> y >> w;
        adj[x - 1].push_back(y - 1);
        cost[x - 1].push_back(w);
    }
    std::cout << negative_cycle(adj, cost);
}
