/**
 *  build_heap.cpp
 *  Data Structures: Programming Assignment 2
 *
 *  Author: Ryan Cormier <rydcormier@gmail.com>
 *  Date:   May 2, 2018
 **/

#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cin;
using std::cout;
using std::swap;
using std::pair;
using std::make_pair;

class HeapBuilder {
private:
    vector<int> data_;
    vector< pair<int, int> > swaps_;

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
        if (l < data_.size() && data_[l] < data_[min_index]) {
            min_index = l;
        }
        int r = right_child(i);
        if (r < data_.size() && data_[r] < data_[min_index]) {
            min_index = r;
        }
        if (i != min_index) {
            swap(data_[i], data_[min_index]);
            swaps_.push_back(make_pair(i, min_index));
            sift_down(min_index);
        }
    }

    void WriteResponse() const {
        cout << swaps_.size() << "\n";
        for (int i = 0; i < swaps_.size(); ++i) {
            cout << swaps_[i].first << " " << swaps_[i].second << "\n";
        }
    }

    void ReadData() {
        int n;
        cin >> n;
        data_.resize(n);
        for(int i = 0; i < n; ++i)
            cin >> data_[i];
    }

    void GenerateSwaps() {
        swaps_.clear();
        // The following naive implementation just sorts
        // the given sequence using selection sort algorithm
        // and saves the resulting sequence of swaps.
        // This turns the given array into a heap,
        // but in the worst case gives a quadratic number of swaps.
        //
        // TODO: replace by a more efficient implementation
//        for (int i = 0; i < data_.size(); ++i) {
//            for (int j = i + 1; j < data_.size(); ++j) {
//                if (data_[i] > data_[j]) {
//                    swap(data_[i], data_[j]);
//                    swaps_.push_back(make_pair(i, j));
//                }
//            }
//        }
        int n ((int) data_.size());
        for (int i = (n / 2); i >= 0; i--) {
            sift_down(i);   // swaps are generated in sift_down;
        }
    }

public:
    void Solve() {
        ReadData();
        GenerateSwaps();
        WriteResponse();
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    HeapBuilder heap_builder;
    heap_builder.Solve();
    return 0;
}
