//
//  connecting_points_prim.cpp
//  Algorithms on Graphs - Programming Assignment 5
//
//  Created by Ryan Cormier on 8/16/18.
//

#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <queue>
#include <functional>
#include <limits>

using std::vector;
using std::pair;
using std::priority_queue;

typedef pair<double, int> Vertex;
typedef pair<int, int> Point;

double segment_length(Point a, Point b) {
    return std::sqrt(std::pow(a.first - b.first, 2) + std::pow(a.second - b.second, 2));
}

double minimum_distance(const vector<Point> &points) {
    // Implements Prim's algorithm
    double res = 0.0;
    vector<bool> processed(points.size(), false);
    vector<double> cost(points.size(), std::numeric_limits<double>::max());
    cost[0] = 0.0;
    priority_queue<Vertex, vector<Vertex>, std::greater<Vertex>> pq;
    for (int i = 0; i < points.size(); i++) {
        pq.push(std::make_pair(cost[i], i));
    }
    while (!pq.empty()) {
        Vertex v = pq.top();
        pq.pop();
        if (processed[v.second]) {
            continue;
        }
        processed[v.second] = true;
        res += v.first;
        for (int z = 0; z < points.size(); z++) {
            if (!processed[z]) {
                double w = segment_length(points[v.second], points[z]);
                if (cost[z] > w) {
                    pq.push(std::make_pair(w, z));
                }
            }
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

