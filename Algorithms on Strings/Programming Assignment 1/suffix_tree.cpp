//
//  suffix_tree.cpp
//  Algorithms on Strings - Programming Assignment 1
//
//  Created by Ryan Cormier on 1/17/19.
//

#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

bool Debug = false;

struct Node
{
    size_t key;
    size_t pos;
    size_t length;
    vector<size_t> children;
    
    Node(size_t k, size_t p, size_t l) : key(k), pos(p), length(l) {}
    
    bool is_leaf() const
    {
        return (children.empty());
    }
    
    string label(const string &text) const
    {
        string label;
        
        if (text.size() - length >= pos)
        {
            label =  text.substr(pos, length);
        }
        
        return label;
    }
};

void print_node(const Node &node, const string &text)
{
    cout << "(" << node.key << ", " << node.label(text) << ") [";
    
    for (size_t i = 0; i < node.children.size(); ++i)
    {
        if (i > 0)
        {
            cout << ", ";
        }
        
        cout << node.children[i];
    }
    
    cout << "]" << endl;
}

typedef vector<Node> Tree;

void print_tree(const Tree &tree, const string &text)
{
    for (size_t i = 0; i < tree.size(); ++i) {
        cout << i << ".  ";
        print_node(tree[i], text);
    }
}

// called by build_suffix_tree
void split_node(Tree &tree, size_t node, size_t pos, size_t k, const string &text)
{
    if (Debug)
    {
        cout << "split_node(node = " << node << ", pos = " << pos << ", i = " <<
        k << ")" << endl;
    }
    
    // create two new nodes from where node label and substring diverge
    Node new_node1(tree.size(), tree[node].pos + k, tree[node].length - k);
    Node new_node2(tree.size() + 1, pos + k, text.size() - pos - k);
    
    if (!tree[node].is_leaf())  //  move children down tree
    {
        new_node1.children = tree[node].children;
        tree[node].children.clear();
    }
    
    tree[node].length = k;
    
    tree.push_back(new_node1);
    tree.push_back(new_node2);
    
    tree[node].children.push_back(new_node1.key);
    tree[node].children.push_back(new_node2.key);
    
    if (Debug)
    {
        cout << "New nodes: ";
        print_node(tree[node], text);
        cout << "           ";
        print_node(tree[new_node1.key], text);
        cout << "           ";
        print_node(tree[new_node2.key], text);
    }
}

Tree build_suffix_tree(const string &text)
{
    Tree tree;
    
    // add root node
    tree.push_back(Node(0, 0, 0));
    
    size_t pos = 0;
    
    // iterate through all suffixes
    while (pos < text.size())
    {
        // start with the root and continue until we either add a new node or split
        // a current node.
        Node current_node = tree[0];
        string suffix = text.substr(pos);
        bool processed = false;
        bool match = false;
        size_t suffix_pos = 0;
        
        while (!processed)
        {
            if (!match)
            {
                suffix_pos = 0;
            }
            else
            {
                match = false;
            }
            
            if (Debug)
            {
                cout << "\nTree:" << endl;
                print_tree(tree, text);
                cout << "Suffix: " << suffix << ", pos: " << pos <<
                        ", suffix_pos: " << suffix_pos << ", current node: ";
                print_node(current_node, text);
            }
            
            for (size_t i = 0; i < current_node.children.size(); ++i)
            {
                // compare suffix to child node labels.
                Node child_node = tree[current_node.children[i]];
                string label = child_node.label(text);
                size_t k = 0;
                
                while (k < child_node.length && suffix[k] == label[k])
                {
                    ++k;
                }
                
                if (Debug)
                {
                    cout << "Child node: ";
                    print_node(child_node, text);
                    cout << "k: " << k << endl;
                }
                
                // Three possible cases
                if (k == 0)   //  No match, move to next child
                {
                    continue;
                }
                else if (k == label.length())  //  move to child node with new suffix
                {
                    current_node = child_node;
                    suffix_pos += k;
                    suffix.erase(0, k);
                    match = true;
                    break;
                }
                else    //  partial match - split current node
                {
                    split_node(tree, child_node.key, pos + suffix_pos, k, text);
                    match = true;
                    processed = true;
                    break;
                }
            }
            
            if (!match) //  Add a new node
            {
                Node new_node(tree.size(), pos + suffix_pos, text.size() - pos - suffix_pos);
                tree.push_back(new_node);
                tree[current_node.key].children.push_back(new_node.key);
                processed = true;
                
                if (Debug)
                {
                    cout << "Added new node: ";
                    print_node(new_node, text);
                }
            }
        }
        
        ++pos;
    }
    
    return tree;
}

int main()
{
    string text;
    cin >> text;
    Tree tree = build_suffix_tree(text);
    
    for (size_t i = 1; i < tree.size(); ++i)
    {
        cout << tree[i].label(text) << endl;
    }
    
    return 0;
}
