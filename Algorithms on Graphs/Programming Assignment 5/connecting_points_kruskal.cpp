//
//  connecting_points_kruskal.cpp
//  Algorithms on Graphs - Programming Assignment 5
//
//  Created by Ryan Cormier on 8/16/18.
//


#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <vector>
#include <cmath>

using std::vector;
using std::pair;

typedef pair<int, int> Point;

double segment_length(Point a, Point b) {
    return std::sqrt(std::pow(a.first - b.first, 2) +
                     std::pow(a.second - b.second, 2));
}

struct Disjoint_Set {
    vector<int> parent;
    vector<int> rank;
    
    Disjoint_Set(size_t n) : parent(n), rank(n) {
        for (int i = 0; i < n; i++) {
            make_set(i);
        }
    }
    
    void make_set(int i) {
        parent[i] = i;
        rank[i] = 0;
    }
    
    int find(int i) {
        // returns the index of the root for the set containing i; uses path compression
        if (i != parent[i])
            parent[i] = find(parent[i]);
        return parent[i];
    }
    
    void join(int i, int j) {
        // union by rank heuristic
        int i_id = this->find(i);
        int j_id = this->find(j);
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
    }
};

template <class T>
class Vertex {
public:
    int key;
    T value;
    
    Vertex(int k, T v) : key(k), value(v) {};
};

template <class T>
class Edge {
public:
    Vertex<T> start;
    Vertex<T> end;
    double weight;
    
    Edge(Vertex<T> u, Vertex<T> v, double w) : start(u), end(v), weight(w) {}
    
    bool operator < (const Edge &other) const {
        return (this->weight < other.weight);
    }
};

double minimum_distance(const vector<Point> &points) {
    // Implements Kruskal's algorithm
    vector<Edge<Point>> edges;
    double res = 0.0;
    for (int i = 0; i < points.size() - 1; i++) {
        for (int j = i + 1; j < points.size(); j++) {
            Vertex<Point> u(i, points[i]);
            Vertex<Point> v(j, points[j]);
            double w = segment_length(points[i], points[j]);
            edges.push_back(Edge<Point>(u, v, w));
        }
    }
    Disjoint_Set ds(points.size());
    std::sort(edges.begin(), edges.end());
    for (size_t i = 0; i < edges.size(); i++) {
        if (ds.find(edges[i].start.key) != ds.find(edges[i].end.key)) {
            res += edges[i].weight;
            ds.join(edges[i].start.key, edges[i].end.key);
        }
    }
    return res;
}

int main() {
    size_t n;
    std::cin >> n;
    vector<Point> points(n);
    for (size_t i = 0; i < n; i++) {
        std::cin >> points[i].first >> points[i].second;
    }
    std::cout << std::setprecision(10) << minimum_distance(points) << std::endl;
}
