//
//  phiX174_error_free_overlap.cpp
//
//  Assembles a phiX174 phage virus genome from the given reads.
//
//  Created by Ryan Cormier on 5/15/20.
//
#include <algorithm>
#include <map>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const size_t NUM_READS = 1168;
const size_t READ_SIZE = 100;

struct PrefixTreeNode
{
    size_t l_bound = 0;
    size_t u_bound = 0;
    size_t chain_len = 0;
    std::map<char, size_t> branches;
};

// Modified b-tree as a compact prefix tree.
// See https://www.ncbi.nlm.nih.gov/pmc/articles/PMC4417569/
struct PrefixTree
{
    std::vector<PrefixTreeNode> nodes;
    
    PrefixTree(const std::vector<std::string>&);
    PrefixTreeNode operator [](size_t i) { return nodes[i]; }
};

// Uses read strings as nodes and weighted edges representing the overlap
// between two reads.
struct OverlapGraph
{
    std::vector< std::vector<size_t>> adj;
    std::vector< std::vector<size_t>> weights;
    
    OverlapGraph(size_t n) : adj(n), weights(n, std::vector<size_t>(n, 0)) {}
    OverlapGraph(const std::vector<std::string>&);
    void initialize(const std::vector<std::string>&, PrefixTree&);
    void print();
    size_t size() { return adj.size(); }
};

// DFS to find a hamiltonian path.
vector<size_t> hamiltonian_path(OverlapGraph&);

int main()
{
    vector<string> G(NUM_READS);
    
    for (size_t i = 0; i < NUM_READS; i++)
        cin >> G[i];
    
    OverlapGraph g(G);
    
    vector<size_t> path = hamiltonian_path(g);
    
    size_t u, v, l;
    
    for (size_t i = 0; i < path.size(); i++)
    {
        u = path[i];
        v = (i < (path.size() - 1) ? path[i + 1] : path[0]);
        l = READ_SIZE - g.weights[u][v];
        
        cout << G[u].substr(0, l);
    }
    
    cout << endl;
    
    return 0;
}

PrefixTree::PrefixTree(const vector<string> &G)
{
    PrefixTreeNode root;
    nodes.push_back(root);
    
    for (size_t i = 0; i < G.size(); i++)
    {
        string S(G[i]);
        size_t j = 0;
        size_t local_position = 1;
        size_t node = 0;
        size_t path_len = 0;
        
        while (j < S.size())
        {
            char c = S[j];
            
            if (nodes[node].chain_len >= local_position) // still w/in node
            {
                if (c == G[nodes[node].l_bound][j]) // match: keep going
                {
                    local_position++;
                    path_len++;
                    j++;
                }
                else    // split node
                {
                    PrefixTreeNode n2, n3;
                    
                    n2.chain_len = nodes[node].chain_len - local_position;
                    n2.l_bound = nodes[node].l_bound;
                    n2.u_bound = nodes[node].u_bound - 1;
                    size_t n2_id = nodes.size();
                    char n2_char = G[nodes[node].l_bound][j];
                    
                    n3.chain_len = S.size() - (path_len + 1);
                    n3.l_bound = i;
                    n3.u_bound = i;
                    size_t n3_id = n2_id + 1;
                    
                    nodes[node].chain_len = local_position - 1;
                    nodes[node].branches[n2_char] = n2_id;
                    nodes[node].branches[c] = n3_id;
                    
                    nodes.push_back(n2);
                    nodes.push_back(n3);
                    
                    break;
                }
            }
            else        // look for the next node or create one if needed.
            {
                map<char, size_t>::iterator b = nodes[node].branches.find(c);
                
                if (b != nodes[node].branches.end())
                {
                    node = b->second;
                    local_position = 1;
                    path_len++;
                    nodes[node].u_bound = i;
                    j++;
                }
                else
                {
                    PrefixTreeNode n;
                    n.l_bound = i;
                    n.u_bound = i;
                    n.chain_len = S.size() - (path_len + 1);
                    
                    nodes[node].branches[c] = nodes.size();
                    nodes.push_back(n);
                    
                    break;
                }
            }
        }
    }
}

OverlapGraph::OverlapGraph(const vector<string> &G)
{
    adj.resize(G.size());
    weights.resize(G.size(), vector<size_t>(G.size(), 0));
    
    PrefixTree T(G);
    
    for (size_t i = 0; i < G.size(); i++)
    {
        string S(G[i]);
        size_t j = 1;
        
        while (j < S.size())
        {
            size_t v = j;   // current position in S
            size_t local_positon = 1;   // current position in Node
            size_t path_len = 0;    // Overall length if current match
            PrefixTreeNode node = T[0];
            while (true)
            {
                if (v == S.size())
                {
                    // any string index in the current nodes interval is
                    // a match from j.
                    for (size_t z = node.l_bound; z <= node.u_bound; z++)
                    {
                        if (weights[i][z] == 0 && i != z)
                        {
                            weights[i][z] = S.size() - j;
                            adj[i].push_back(z);
                        }
                    }
                    
                    break;
                }
                
                char g1 = S[v];
                
                if (node.chain_len >= local_positon) // still in node
                {
                    char g2 = G[node.l_bound][path_len];
                    
                    if (g1 == g2)
                    {
                        local_positon++;
                        path_len++;
                        v++;
                    }
                    else
                    {
                        break;
                    }
                }
                else    // end of node. find next or quit.
                {
                    map<char, size_t>::iterator b = node.branches.find(g1);
                    
                    if (b != node.branches.end())
                    {
                        node = T[b->second];
                        local_positon = 1;
                        path_len++;
                        v++;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            
            j++;
        }
    }
    // sort by decreasing weight. Should speed up any DFS
    for (size_t i = 0; i < adj.size(); i++)
    {
        sort(adj[i].begin(),
             adj[i].end(),
             [&, i](const size_t a, const size_t b) -> bool { return weights[i][a] > weights[i][b]; });
    }
}

// Recusive call for DFS finding hamiltonian path
void visit(OverlapGraph &g, size_t u, vector<bool> &visited, vector<size_t> &path)
{
    visited[u] = 1;
    path.push_back(u);
    
    if (path.size() == g.size())
        return;
    
    for (size_t i = 0; i < g.adj[u].size(); i++)
    {
        size_t v = g.adj[u][i];
        if (! visited[v])
            visit(g, v, visited, path);
    }
    
    if (path.size() != g.size()) {
        path.pop_back();
        visited[u] = 0;
    }
    
}

vector<size_t> hamiltonian_path(OverlapGraph &g)
{
    vector<size_t> path;
    
    for (size_t u = 0; u < g.size(); u++)
    {
        if (path.size() == g.size())
            break;
        
        path.clear();
        vector<bool> visited(g.size(), 0);
        visit(g, u, visited, path);
        u++;
    }
    
    return path;
}
