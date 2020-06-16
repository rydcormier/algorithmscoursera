//
//  is_bst.cpp
//  Data Structures
//
//  Created by Ryan Cormier on 6/6/18.
//
#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::stack;

struct Node {
    int key;
    int left;
    int right;
    
    Node() : key(0), left(-1), right(-1) {}
    Node(int key_, int left_, int right_) : key(key_), left(left_), right(right_) {}
};

void print(vector <int> a) {
    for (size_t i = 0; i < a.size(); i++) {
        if (i > 0) {
            cout << ' ';
        }
        cout << a[i];
    }
    cout << '\n';
}

void recursive_in_order(int index, const vector<Node> &tree, vector<int> &v, bool &is_good) {
    const Node* current = &tree[index];
    if (is_good) {
        if (current->left != -1) {
            recursive_in_order(current->left, tree, v, is_good);
        }
        if (!v.empty() && current->key < v.back()) {
            is_good =  false;
        }
        v.push_back(current->key);
        if (current->right != -1) {
            recursive_in_order(current->right, tree, v, is_good);
        }
    }
}

bool IsBinarySearchTree(const vector<Node>& tree) {
    if (tree.empty()) {
        return true;
    }
    bool is_good = true;
    vector<int> v;
    recursive_in_order(0, tree, v, is_good);
    return is_good;
}

int main() {
    int nodes;
    cin >> nodes;
    vector<Node> tree;
    for (int i = 0; i < nodes; ++i) {
        int key, left, right;
        cin >> key >> left >> right;
        tree.push_back(Node(key, left, right));
    }
    if (IsBinarySearchTree(tree)) {
        cout << "CORRECT" << endl;
    } else {
        cout << "INCORRECT" << endl;
    }
    return 0;
}


