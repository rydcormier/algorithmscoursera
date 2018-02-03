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
    int length() const { return this->end - this->start; }
    bool covers(int x) const { return (x >= this->start && x <= this->end); }
    bool intersects(const Segment&) const;
    Segment* intersection(const Segment&) const;
    bool operator< (const Segment&) const;
};

bool Segment::operator<(const Segment &other) const {
    // first compare starts
    if (this->start < other.start) return true;
    
    // if starts are the same - the shorter segment is less then
    if (this->start == other.start) return (this->length() < other.length());
    
    return false;
}

bool Segment::intersects(const Segment &other) const {
    const Segment *smaller;
    const Segment *larger;
    
    if (this->operator<(other)) {
        smaller = this;
        larger = &other;
    }
    else {
        smaller = &other;
        larger = this;
    }
    
    // intersection implies the end of the smaller segment is covered by the larger
    return larger->covers(smaller->end);
}

Segment* Segment::intersection(const Segment &other) const {
    Segment *result = NULL;
    if (this->intersects(other)) {
        // the start is the smaller end
        int s = std::min(this->end, other.end);
        
        // the end is the larger start
        int e = std::max(this->start, other.start);
        
        result = new Segment(s, e);
    }
    
    return result;
}

/**
 *  Function optimal_points
 *
 *  Finds a sequence of integers such that every line segment in the given set covers
 *  at least one point and the number of points is a minimum.
 *
 **/
vector<int> optimal_points(vector<Segment> &segments) {
    
    // if there is only one segment, return its start.
    if (segments.size() == 1) return vector<int>(1, segments[0].start);
    
    vector<int> points;
    
    // order segments
    std::sort(segments.begin(), segments.end());
    
    // gready choice start at first end and end at last start
    int lbound = segments.front().end;
    int rbound = segments.back().start;
    
    // definitely need lbound
    points.push_back(lbound);
    
    // move from left to right
    vector<Segment>::iterator iter = segments.begin();
    int x = lbound;
    
    //bool current_segment_covers = true;
    while (iter != segments.end() && x <= rbound) {
        if (iter->covers(x)) {
            if (points.back() != x) {
                points.push_back(x);
            }
            // current segment is covering, move to the next segment
            ++iter;
        }
        else {
            // current segment is not covering, move to next x
            ++x;
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
}

