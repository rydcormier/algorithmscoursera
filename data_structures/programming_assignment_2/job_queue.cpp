/**
 *  job_queue.cpp
 *  Data Structures: Programming Assignment 2
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date:   May 2, 2018
 **/

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using std::string;
using std::swap;
using std::vector;
using std::cin;
using std::cout;

struct Worker {
    // Store a workers index and next available start time in one object.  Also, define
    // comparators for use in the heap. Comparisons are based on next start time and
    // then index when equal.
    int index;
    long long next_start_time;
    
    Worker(int i, long long j = 0) : index(i), next_start_time(j) {}
    
    Worker(const Worker& other) :   index(other.index),
                                    next_start_time(other.next_start_time) {}
    
    bool operator < (const Worker& other) const {
        if (next_start_time < other.next_start_time) {
            return true;
        } else if (next_start_time > other.next_start_time) {
            return false;
        }
        return (index < other.index);
    }
    
    bool operator > (const Worker &other) const {
        if (next_start_time > other.next_start_time) {
            return true;
        } else if (next_start_time < other.next_start_time) {
            return false;
        }
        return (index > other.index);
    }
    
    string to_string() const {
        return ("Index: " + std::to_string(index) + "  Next start time: "
                + std::to_string(next_start_time));
    }
};

struct WorkerHeap {
    // Priority queue for workers using a binary min-heap.
    vector<Worker> workers;
    
    WorkerHeap(int numWorkers) {
        for (int i = 0; i < numWorkers; i++) {
            workers.push_back(Worker(i));
        }
    }
    
    int size() const { return (int) workers.size(); }
    
    int parent(int i) {
        return (i > 0) ? (i - 1) / 2 : i;
    }
    
    int left_child(int i) {
        return 2 * i + 1;
    }
    
    int right_child(int i) {
        return 2 * i + 2;
    }
    
    void sift_down(int i) {
        // restore heap condition for subtree with root at i
        int min_index = i;
        int l = left_child(i);
        if (l < workers.size() && workers[l] < workers[min_index]) {
            min_index = l;
        }
        int r = right_child(i);
        if (r < workers.size() && workers[r] < workers[min_index]) {
            min_index = r;
        }
        if (i != min_index) {
            swap(workers[i], workers[min_index]);
            sift_down(min_index);
        }
    }
    
    void sift_up(int i) {
        while (i > 0 && workers[parent(i)] > workers[i]) {
            swap(workers[parent(i)], workers[i]);
            i = parent(i);
        }
    }
    
    Worker get_next() {
        Worker next_worker(workers.front());
        swap(workers.front(), workers.back());
        workers.pop_back();
        sift_down(0);
        return next_worker;
    }
    
    void insert(const Worker& worker) {
        workers.push_back(worker);
        sift_up(workers.size() - 1);
    }
};

class JobQueue {
private:
    int num_workers_;
    vector<int> jobs_;
    
    vector<int> assigned_workers_;
    vector<long long> start_times_;
    
    void WriteResponse() const {
        for (int i = 0; i < jobs_.size(); ++i) {
            cout << assigned_workers_[i] << " " << start_times_[i] << "\n";
        }
    }
    
    void ReadData() {
        int m;
        cin >> num_workers_ >> m;
        jobs_.resize(m);
        for(int i = 0; i < m; ++i)
            cin >> jobs_[i];
    }
    
    void AssignJobs() {
        // TODO: replace this code with a faster algorithm.
        assigned_workers_.resize(jobs_.size());
        start_times_.resize(jobs_.size());
        vector<long long> next_free_time(num_workers_, 0);
        
        WorkerHeap heap(num_workers_);
        for (size_t i = 0; i < jobs_.size(); i++) {
            int duration = jobs_[i];
            Worker worker = heap.get_next();
            assigned_workers_[i] = worker.index;
            start_times_[i] = worker.next_start_time;
            worker.next_start_time += duration;
            heap.insert(worker);
        }
    }
    
public:
    void Solve() {
        ReadData();
        AssignJobs();
        WriteResponse();
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    JobQueue job_queue;
    job_queue.Solve();
    return 0;
}
