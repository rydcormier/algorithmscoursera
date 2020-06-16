//
//  shortest_paths.cpp
//
//  Algorithms on Graphs - Programming Assignment 4
//
//  Created by Ryan Cormier on 8/15/18.
//
#include <iostream>
#include <limits>
#include <vector>
#include <queue>
#include <functional>

using std::vector;
using std::queue;
using std::pair;
using std::priority_queue;

typedef pair<long long, int> Vertex;

void shortest_paths( vector<vector<int> > &adj,
                    vector<vector<int> > &cost,
                    int s,
                    vector<long long> &distance,
                    vector<int> &reachable,
                    vector<int> &shortest,
                    bool has_negative_weights) {
    
    distance[s] = 0;
    reachable[s] = 1;
    // if no negative weights, use dijkstra
    if (!has_negative_weights) {
        priority_queue<Vertex, vector<Vertex>, std::greater<Vertex> > pq;
        pq.push(std::make_pair(0, s));
        
        vector<bool> processed(adj.size(), false);
        
        while (!pq.empty()) {
            Vertex u = pq.top();
            pq.pop();
            
            if (processed[u.second]) {
                continue;
            }
            
            processed[u.second] = true;
            
            for (int i = 0; i < adj[u.second].size(); i++) {
                int v = adj[u.second][i];
                int w = cost[u.second][i];
                reachable[v] = 1;
                if (distance[v] > distance[u.second] + w) {
                    distance[v] = distance[u.second] + w;
                    pq.push(std::make_pair(distance[v], v));
                }
            }
        }
        return;
    }
    
    // Use bellman ford |V| - 1 times
    bool dist_updated = false;
    int counter = 1;
    while (counter < adj.size()) {
        
        queue<int> q;
        q.push(s);
        vector<bool> visited(adj.size(), false);
        visited[s] = true;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];
                int w = cost[u][i];
                reachable[v] = 1;
                if (distance[v] > distance[u] + w) {
                    distance[v] = distance[u] + w;
                    dist_updated = true;
                }
                if (!visited[v]) {
                    q.push(v);
                    visited[v] = true;
                }
            }
        }
        
        if (!dist_updated) {
            break;
        }
        counter++;
    }
    
    if (dist_updated) {
        // find updated vertices
        queue<int> q;
        q.push(s);
        vector<bool> visited(adj.size(), false);
        visited[s] = true;
        
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];
                int w = cost[u][i];
                if (distance[v] > distance[u] + w) {
                    distance[v] = distance[u] + w;
                    shortest[v] = 0;
                }
                if (!visited[v]) {
                    q.push(v);
                    visited[v] = true;
                }
            }
        }
    }
    
    // BFS on previously found vertices
    vector<bool> visited(adj.size(), false);
    for (int i = 0; i < adj.size(); i++) {
        if (!shortest[i] && !visited[i]) {
            queue<int> q;
            q.push(i);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (vector<int>::iterator v = adj[u].begin(); v != adj[u].end(); v++) {
                    shortest[*v] = 0;
                    if (!visited[*v]) {
                        q.push(*v);
                        visited[*v] = true;
                    }
                }
            }
        }
    }
}

int main() {
    int n, m, s;
    std::cin >> n >> m;
    vector<vector<int> > adj(n, vector<int>());
    vector<vector<int> > cost(n, vector<int>());
    bool has_negative_weights = false;
    for (int i = 0; i < m; i++) {
        int x, y, w;
        std::cin >> x >> y >> w;
        adj[x - 1].push_back(y - 1);
        cost[x - 1].push_back(w);
        if (w < 0 ) {
            has_negative_weights = true;
        }
    }
    std::cin >> s;
    s--;
    vector<long long> distance(n, std::numeric_limits<long long>::max());
    vector<int> reachable(n, 0);
    vector<int> shortest(n, 1);
    shortest_paths(adj, cost, s, distance, reachable, shortest, has_negative_weights);
    for (int i = 0; i < n; i++) {
        if (!reachable[i]) {
            std::cout << "*\n";
        } else if (!shortest[i]) {
            std::cout << "-\n";
        } else {
            std::cout << distance[i] << "\n";
        }
    }
}
