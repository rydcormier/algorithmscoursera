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

using std::vector;
using std::queue;
using std::pair;
using std::priority_queue;

struct Node {
    int key;
    int dist;
    
    Node(int k, int d) : key(k), dist(d) {}
    
    bool operator< (const Node &rhs) const {
        if (dist == -1) {
            return false;
        } else if (dist == 0) {
            return true;
        }
        return dist < rhs.dist;
    }
    
    bool operator> (const Node &rhs) const {
        return !operator<(rhs);
    }
};

int distance(vector<vector<int> > &adj, vector<vector<int> > &cost, int s, int t) {
    vector<int> dist(adj.size(), -1);
    dist[s] = 0;
    vector<bool> processed(adj.size(), false);
    priority_queue<Node, vector<Node>, std::greater<Node>> pq;
    for (int k = 0; k < adj.size(); k++) {
        pq.push(Node(k, dist[k]));
    }
    while (! pq.empty()) {
        Node u = pq.top();
        pq.pop();
        if (processed[u.key]) {
            continue;
        }
        processed[u.key] = true;
        // iterate through edges;
        for (int i = 0; i < adj[u.key].size(); i++) {
            int v = adj[u.key][i];
            int w = cost[u.key][i];
            if (dist[v] > dist[u.key] + w || dist[v] == -1) {
                dist[v] = dist[u.key] + w;
                pq.push(Node(v, dist[v]));
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
    s--, t--;
    std::cout << distance(adj, cost, s, t);
}
