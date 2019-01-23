//
//  non_shared_substring.cpp
//  Algorithms on Strings - Programming Assignment 1
//
//  Created by Ryan Cormier on 1/23/19.
//

#include <algorithm>
#include <cassert>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

const bool Debug = false;
const size_t NA = 4002;

struct Node
{
    size_t key;
    size_t pos;
    size_t length;
    size_t spos = NA;
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
    cout << "(key: " << node.key << ", spos: ";
    
    if (node.spos != NA)
    {
        cout << node.spos;
    }
    else
    {
        cout << "NA";
    }
    
    cout << ", " << "pos: " << node.pos << ", length: " <<
    node.length << ", label: " << node.label(text) << ") [";
    
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
void split_node(Tree &tree, size_t node, size_t spos, size_t pos, size_t k, const string &text)
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
    
    if (new_node1.label(text).back() == '$')
    {
        new_node1.spos = tree[node].spos;
        tree[node].spos = NA;
    }
    
    if (new_node2.label(text).back() == '$')
    {
        new_node2.spos = spos;
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
                    split_node(tree, child_node.key, pos, pos + suffix_pos, k, text);
                    match = true;
                    processed = true;
                    break;
                }
            }
            
            if (!match) //  Add a new node
            {
                Node new_node(tree.size(), pos + suffix_pos, text.size() - pos - suffix_pos);
                
                if (new_node.label(text).back() == '$')
                {
                    new_node.spos = pos;
                }
                
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

vector<size_t> combine(vector<size_t> v1, vector<size_t> v2)
{
    vector<size_t> v;
    v.reserve(v1.size() + v2.size());
    v.insert(v.end(), v1.begin(), v1.end());
    v.insert(v.end(), v2.begin(), v2.end());
    
    return v;
}

vector<size_t> find_spos(const Tree &tree, const Node &node, vector<size_t> result = vector<size_t>())
{
    if (node.spos != NA)
    {
        result.push_back(node.spos);
        return result;
    }
    
    for (size_t i = 0; i < node.children.size(); ++i)
    {
        Node child_node = tree[node.children[i]];
        result = combine(result, find_spos(tree, child_node));
    }
    
    return result;
}

vector<size_t> find_substring(const Tree &tree, const string &text, string s)
{
    vector<size_t> result;
    
    Node current_node = tree[0];
    size_t pos = 0;
    bool match = false;
    
    while (pos < s.size())
    {
        string search_string = s.substr(pos);
        for (size_t i = 0; i < current_node.children.size(); ++i)
        {
            Node child_node = tree[current_node.children[i]];
            string label = child_node.label(text);
            size_t max = min(child_node.length, search_string.size());
            size_t k = 0;
            while (k < max  && search_string[k] == label[k])
            {
                ++k;
            }
            
            if (k == 0)
            {
                continue;
            }
            
            if (k == search_string.size() &&
                search_string.size() <= child_node.length)
            {
                return find_spos(tree, child_node);
            }
            
            else if (k == child_node.length &&
                     child_node.length < search_string.size())
            {
                current_node = child_node;
                pos += k;
                match = true;
                break;
            }
            
        }
        
        if (!match)
        {
            break;
        }
    }
    
    return result;
}

int char_to_index(char c)
{
    switch (c) {
        case 'A':
            return 0;
            break;
            
        case 'C':
            return 1;
            break;
            
        case 'G':
            return 2;
            break;
            
        case 'T':
            return 3;
            break;
            
        default:
            return -1;
            break;
    }
}

string solve (string p, string q)
{
    string result;
    
    // combine strings and build suffix tree
    string text = p + "#" + q + "$";
    size_t hinge = p.size();
    Tree tree = build_suffix_tree(text);
    
    size_t len = 1;
    
    while (len <= hinge)
    {
        size_t pos = 0;
        
        while (pos + len <= hinge)
        {
            string substr = p.substr(pos, len);
            vector<size_t> occurences = find_substring(tree, text, substr);
            bool rhs = false;
            
            for (size_t i = 0; i < occurences.size(); ++i)
            {
                if (occurences[i] > hinge)
                {
                    rhs = true;
                    break;
                }
            }
            
            if (!rhs)
            {
                result = substr;
                len = hinge;
            }
            
            ++pos;
        }
        
        ++len;
    }
    
    return result;
}

string solve_naive(string p, string q)
{
    string result;
    
    size_t n = p.length();
    
    size_t len = 1;
    
    while (len <= n)
    {
        size_t pos = 0;
        
        while (pos <= n - len)
        {
            string substr = p.substr(pos, len);
            
            if (q.find(substr) == string::npos)
            {
                result = substr;
                len = n;
                break;
            }
            
            ++pos;
        }
        
        ++len;
    }
    
    return result;
}

void print_vector(const vector<size_t> &v)
{
    cout << "< ";
    size_t i = 0;
    while (i < v.size())
    {
        if (i > 0)
        {
            cout << ", ";
        }
        
        cout << v[i];
        ++i;
    }
    
    cout << " >" << endl;
}

string random_string(int len)
{
    mt19937 rng;
    rng.seed(random_device()());
    uniform_int_distribution<mt19937::result_type> dist(0, 3);
    
    const string letters("ACTG");
    string rnd_str;
    int count = 0;
    
    while (count < len)
    {
        rnd_str.push_back(letters.at(dist(rng)));
        ++count;
    }
    
    return rnd_str;
}

void stress_test()
{
    mt19937 rng;
    rng.seed(random_device()());
    uniform_int_distribution<mt19937::result_type> dist(1,1000);
    long long count = 0;
    while (true)
    {
        ++count;
        int n = dist(rng);
        string p;
        string q;
        
        while (p == q)
        {
            p = random_string(n);
            q = random_string(n);
        }
        
        string res = solve(p, q);
        string res_naive = solve_naive(p, q);
        cout << "\nTest " << count << "\n==========" << endl;
        cout << "p:\n==\n" << p << endl;
        cout << "q:\n==\n" << q << endl;
        cout << "\nsolve:\n======\n" << res << endl;
        cout << "naive:\n======\n" << res_naive << endl;
        
        assert(res.length() == res_naive.length());
        
        assert(q.find(res) == string::npos);
    }
}

int main (void)
{
    string p;
    cin >> p;
    string q;
    cin >> q;
    
    string ans = solve (p, q);
    
    cout << ans << endl;
    //    stress_test();
    
    return 0;
}
