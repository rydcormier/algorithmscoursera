//
//  dijkstra.cpp
//  Algorithms on Graphs - Programming Assignment 4
//
//  Created by Ryan Cormier on 8/12/18.
//

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <limits>

using std::vector;
using std::queue;
using std::pair;
using std::priority_queue;

int distance(vector<vector<int> > &adj, vector<vector<int> > &cost, int s, int t) {
    // Initialize all distances: use -1 as infinity and set source to 0
    vector<int> dist(adj.size(), -1);
    dist[s] = 0;
    
    // Priority queue for vertices, which are pairs with dist being first
    priority_queue<pair<int, int>, vector<pair<int, int> >, std::greater<pair<int, int> >> pq;
    for (int i = 0; i < adj.size(); i++) {
        if (i == s) {
            pq.push(std::make_pair(0, i));
        } else {
            pq.push(std::make_pair(std::numeric_limits<int>::max(), i));
        }
    }
    
    // Track processed vertices to allow multiple entries in queue
    vector<bool> processed(adj.size(), false);
    
    // iterate through priority queue
    while (!pq.empty()) {
        pair<int, int> u = pq.top();
        pq.pop();
        
        if (processed[u.second]) {  // redundant entry
            continue;
        }
        
        if (dist[u.second] == -1) { // remaing vertices are unreachable
            break;
        }
        
        processed[u.second] = true;
        
        for (int i = 0; i < adj[u.second].size(); i++) {
            int v = adj[u.second][i];
            int w = cost[u.second][i];
            
            // relax edge
            if (dist[v] > dist[u.second] + w || dist[v] == -1) {
                dist[v] = dist[u.second] + w;
                pq.push(std::make_pair(dist[v], v));
            }
        }
    }
    
    return dist[t];
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
    int s, t;
    std::cin >> s >> t;
    s--; t--;
    std::cout << distance(adj, cost, s, t);
}
