//
//  main.cpp
//  graphs
//
//  Created by Ryan Cormier on 8/13/18.
//  Copyright © 2018 Ryan Cormier. All rights reserved.
//

#include <iostream>
#include <vector>
#include <queue>
#include <functional>
#include <ctime>
#include <cstdlib>


using std::vector;
using std::queue;
using std::pair;
using std::priority_queue;

const int MAX_WEIGHT = 10;
const int MAX_VERTICES = 5;
const int MAX_EDGES = 10;
bool debug = true;

void print_dist(const vector<int> &dist) {
    for (int i = 0; i < dist.size(); i++) {
        if (i > 0) {
            std::cout << '\t';
        }
        std::cout << (i + 1);
    }
    std::cout << std::endl;
    for (int i = 0; i < dist.size(); i++) {
        if (i > 0) {
            std::cout << '\t';
        }
        std::cout << dist[i];
    }
    std::cout << std::endl;
}

void print_graph(const vector<vector<int>> &adj, const vector<vector<int>> &cost) {
    for (int i = 0; i < adj.size(); i++) {
        for (int j = 0; j < adj[i].size(); j++) {
            std::cout << (i + 1) << " " << (adj[i][j] + 1) << " " << cost[i][j] << std::endl;
        }
    }
}

struct Vertex {
    int key, dist;
    
    Vertex(int k, int d) : key(k), dist(d) {}
    
    bool operator< (const Vertex &other) const {
        if (this->dist < 0) {
            return (this->dist < other.dist);
        }
        return (this->dist > other.dist);
    }
    
    bool operator> (const Vertex &other) const {
        if (this->dist < 0) {
            return (this->dist > other.dist);
        }
        return (this->dist < other.dist);
    }
};

class DistCompare {
public:
    bool operator() (const Vertex &lhs, const Vertex &rhs) {
        // negative dist is actually the largest or infinite dist
        if (lhs.dist < 0) {
            return (lhs.dist < rhs.dist);
        }
        return (lhs.dist > rhs.dist);
    }
};

void print_queue(priority_queue<Vertex> pq) {
    std::cout << "{ ";
    bool first = true;
    while(!pq.empty()) {
        Vertex u = pq.top();
        pq.pop();
        if (!first) {
            std::cout << ", ";
        } else {
            first = false;
        }
        std::cout << "(" << (u.key + 1) << ", " << u.dist << ")";
    }
    std::cout << " }" << std::endl;
}

int distance_naive(const vector<vector<int> > &adj, const vector<vector<int> > &cost, int s, int t) {
    vector<int> dist(adj.size(), -1);
    dist[s] = 0;
    bool edge_change;
    do {
        edge_change = false;
        // relax source edges
        for (int u = 0; u < adj.size(); u++) {
            if (dist[u] < 0) {
                continue;
            }
            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];
                int w = cost[u][i];
                // relax edge;
                if (dist[v] == -1 || dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    edge_change = true;
                }
            }
        }
    } while (edge_change);
    
    return dist[t];
}

int distance(const vector<vector<int> > &adj, const vector<vector<int> > &cost, int s, int t) {
    vector<int> dist(adj.size(), -1);
    dist[s] = 0;
    if (debug) {
        std::cout << "Distances:" << std::endl;
        print_dist(dist);
    }
    vector<bool> processed(adj.size(), false);
    priority_queue<Vertex> pq;
    for (int k = 0; k < adj.size(); k++) {
        pq.push(Vertex(k, dist[k]));
    }
    while (! pq.empty()) {
        if (debug) {
            std::cout << "Priority queue: ";
            print_queue(pq);
        }
        Vertex u = pq.top();
        if (debug) {
            std::cout << "Popped Vertex: " << (u.key + 1) << ", dist = " << u.dist << std::endl;
        }
        pq.pop();
        if (u.dist < 0) {
            if (debug) {
                std::cout << "Vertex has infinite distance, skipping." << std::endl;
            }
            break;
        }
        if (processed[u.key]) {
            if (debug) {
                std::cout << "Vertex already processed. Skipping." << std::endl;
            }
            continue;
        }
        processed[u.key] = true;
        // iterate through edges;
        for (int i = 0; i < adj[u.key].size(); i++) {
            int v = adj[u.key][i];
            int w = cost[u.key][i];
            if (debug) {
                std::cout << "Checking edge: (" << (u.key + 1) << ", " << (v + 1) << ") - " << "weight: " << w << "\ndist[" << (v + 1) << "] = " << dist[v] << std::endl;
            }
            // relax edge
            if (dist[v] == -1 || dist[v] > dist[u.key] + w) {
                if (debug) {
                    std::cout << "Relaxing: dist[" << (v + 1) << "] = " << dist[u.key] + w << std::endl;
                }
                dist[v] = dist[u.key] + w;
                pq.push(Vertex(v, dist[v]));
            }
        }
    }
    if (debug) {
        std::cout << "Returning: " << dist[t] << std::endl << std::endl;
    }
    return dist[t];
    
}

bool is_in(const vector<int> &v, int x) {
    for (int i = 0; i < v.size(); i++) {
        if (x == v[i]) {
            return true;
        }
    }
    return false;
}

bool stress_test() {
    srand((int)time(NULL));
    int n = (rand() % (MAX_VERTICES - 1)) + 1;
    int m = rand() % MAX_EDGES;
    int s = rand() % n;
    int t = s;
    
    vector<vector<int> > adj(n, vector<int>());
    vector<vector<int> > cost(n, vector<int>());
    
    if (n > 1) {
        do {
            t = rand() % n;
        } while (t == s);
        for (int i = 0; i < m; i++) {
            int u = rand() % n;
            int v;
            do {
                v = rand() % n;
            } while (v == u || is_in(adj[u], v));
            int w = rand() % MAX_WEIGHT;
            adj[u].push_back(v);
            cost[u].push_back(w);
        }
    }
    std::cout << "\n\n";
    std::cout << n << " " << m << std::endl;
    print_graph(adj, cost);
    std::cout << s + 1 << " " << t + 1 << std::endl;
    int x = distance_naive(adj, cost, s, t);
    int y = distance(adj, cost, s, t);
    std::cout << n << " " << m << std::endl;
    print_graph(adj, cost);
    std::cout << s + 1 << " " << t + 1 << std::endl;
    std::cout << "Naive: " << x << "\nDikstra: " << y << std::endl;
    return (x == y);
}


int main() {
    if (debug) {
        while (stress_test()) {
            continue;
        }
        return 1;
    }
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
