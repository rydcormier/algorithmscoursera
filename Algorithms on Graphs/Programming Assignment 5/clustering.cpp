//
//  clustering.cpp
//  Algorithms on Graphs - Programming Assignment 5
//
//  Created by Ryan Cormier on 8/19/18.
//

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <cmath>
#include <limits>

using std::vector;
using std::pair;

typedef pair<int, int> Point;

double segment_square(Point a, Point b) {
    return (std::pow(a.first - b.first, 2) + std::pow(a.second - b.second, 2));
}

struct Disjoint_Set {
    vector<size_t> parent;
    vector<size_t> rank;
    size_t count;
    
    Disjoint_Set(size_t n) : parent(n), rank(n), count(0) {
        for (int i = 0; i < n; i++) {
            make_set(i);
        }
    }
    
    void make_set(size_t i) {
        parent[i] = i;
        rank[i] = 0;
        this->count += 1;
    }
    
    size_t find(size_t i) {
        // returns the index of the root for the set containing i; uses path compression
        if (i != parent[i])
            parent[i] = find(parent[i]);
        return parent[i];
    }
    
    void join(size_t i, size_t j) {
        // union by rank heuristic
        size_t i_id = this->find(i);
        size_t j_id = this->find(j);
        if (i_id == j_id)
            return;
        if (rank[i_id] > rank[j_id]) {
            parent[j_id] = i_id;
        } else {
            parent[i_id] = j_id;
            if (rank[i_id] == rank[j_id]) {
                rank[j_id] = rank[j_id] + 1;
            }
        }
        this->count -= 1;
    }
};

struct Vertex {
    size_t key;
    Point value;
    
    Vertex(size_t k, Point &v) : key(k), value(v) {}
};

struct Edge {
    Vertex start;
    Vertex end;
    double weight;
    
    Edge(Vertex &u, Vertex &v, double w) : start(u), end(v), weight(w) {}
    
    bool operator < (const Edge &other) const {
        return (this->weight < other.weight);
    }
};

double clustering(vector<Point> &points, size_t k) {
    // returns the minimum distance between clusters such that the given points are
    // seperated into k partitions.
    vector<Edge> edges;
    for (size_t i = 0; i < points.size() - 1; i++) {
        for (size_t j = 1; j < points.size(); j++) {
            Vertex u(i, points[i]);
            Vertex v(j, points[j]);
            double w = segment_square(u.value, v.value);
            edges.push_back(Edge(u, v, w));
        }
    }
    Disjoint_Set ds(points.size());
    std::sort(edges.begin(), edges.end());
    size_t idx = 0;
    
    // join closest points until there are k sets
    while (ds.count > k) {
        // cluster two closest points
        size_t i = edges[idx].start.key;
        size_t j = edges[idx].end.key;
        ds.join(i, j);
        idx++;
    }
    
    // the next edge with vertices in different clusters gives the min dist
    while (ds.find(edges[idx].start.key) == ds.find(edges[idx].end.key)) {
        idx++;
    }
    return std::sqrt(edges[idx].weight);
}


int main() {
    size_t n;
    size_t k;
    std::cin >> n;
    vector<Point> points(n);
    for (size_t i = 0; i < n; i++) {
        std::cin >> points[i].first >> points[i].second;
    }
    std::cin >> k;
    std::cout << std::setprecision(10) << clustering(points, k) << std::endl;
}
