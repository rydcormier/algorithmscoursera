/**
 *  covering_segments.cpp
 *
 *  This file is for problem 4 of the week 3 problem set for the coursera course
 *  Algorithmic Toolbox.
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date:   February 2, 2018
 **/

#include <algorithm>
#include <iostream>
#include <climits>
#include <vector>

using std::vector;

/**
 *  Class Segment
 *
 *  Very simple class representing line segments (or closed intervals).
 **/
class Segment {
public:
    int start, end;
    
    Segment (int a, int b) : start(a), end(b) {};
    bool covers(int x) const { return (x >= this->start && x <= this->end); }
    bool operator< (const Segment&) const;
};

bool Segment::operator<(const Segment &other) const {
    // first compare ends
    if (this->end < other.end) return true;
    
    // if ends are the same - the smaller start is less
    if (this->end == other.end) return (this->start < other.start);
    
    return false;
}

/**
 *  Function optimal_points
 *
 *  Finds a sequence of integers such that every line segment in the given set covers
 *  at least one point and the number of points is a minimum.
 *
 **/
vector<int> optimal_points(vector<Segment> &segments) {

    // if there is only one segment, return its start point.
    if (segments.size() == 1) return vector<int>(1, segments[0].start);
    
    vector<int> points;
    
    // order segments
    std::sort(segments.begin(), segments.end());

    // gready choice start at first end
    int x = segments.front().end;
    
    vector<Segment>::iterator iter = segments.begin();
    
    while (iter != segments.end()) {

        if (points.size() > 0 && iter->covers(points.back())) {
            ++iter;
        } else {
            // if we're at end of segment, add x to points
            if (x == iter->end) {
                points.push_back(x);
            } else {
                x = iter->end;
            }
            
        }
    }
    return points;
}

int main() {
    int n;
    std::cin >> n;
    vector<Segment> segments(n, Segment(0, 0));
    for (size_t i = 0; i < segments.size(); ++i) {
        std::cin >> segments[i].start >> segments[i].end;
    }
    
    vector<int> points = optimal_points(segments);
    
    std::cout << points.size() << "\n";
    for (size_t i = 0; i < points.size(); ++i) {
        std::cout << points[i] << " ";
    }
    return 0;
}

