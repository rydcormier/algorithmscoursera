// apsp.cpp
//
// source code for apsp.h

#include "apsp.h"

using namespace std;

void PrefixTree::initialize(const vector<string>& G)
{
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

            if (_nodes[node].chain_len >= local_position)
            {
                if (c == G[_nodes[node].l_bound][j])
                {
                    local_position++;
                    path_len++;
                    j++;
                }
                else
                {
                    Node n2, n3;

                    n2.chain_len = _nodes[node].chain_len - local_position;
                    n2.l_bound = _nodes[node].l_bound;
                    n2.u_bound = _nodes[node].u_bound - 1;
                    size_t n2_id = _nodes.size();
                    char n2_char = G[_nodes[node].l_bound][j];

                    n3.chain_len = S.size() - (path_len + 1);
                    n3.l_bound = i;
                    n3.u_bound = i;
                    size_t n3_id = n2_id + 1;

                    _nodes[node].chain_len = local_position - 1;
                    _nodes[node].branches[n2_char] = n2_id;
                    _nodes[node].branches[c] = n3_id;

                    _nodes.push_back(n2);
                    _nodes.push_back(n3);

                    break;
                }
            }
            else
            {
                map<char, size_t>::iterator b = _nodes[node].branches.find(c);

                if (b != _nodes[node].branches.end())
                {
                    node = b->second;
                    local_position = 1;
                    path_len++;
                    _nodes[node].u_bound = i;
                    j++;
                }
                else
                {
                    Node n;
                    n.l_bound = i;
                    n.u_bound = i;
                    n.chain_len = S.size() - (path_len + 1);

                    _nodes[node].branches[c] = _nodes.size();
                    _nodes.push_back(n);

                    break;
                }
            }
        }
    }
}

PrefixTree::Node& PrefixTree::operator[](size_t i)
{
    assert(i < _nodes.size());
    return _nodes[i];
}

void APSP::initialize(const vector<string>& G, size_t min_weight)
{
    all_pairs.resize(G.size());
    for (size_t i = 0; i < G.size(); i++)
        all_pairs[i].resize(G.size());

    PrefixTree T(G);
    size_t max_val = 0;

    for (size_t i = 0; i < G.size(); i++)
    {
        string S(G[i]);
        size_t j = 1;

        while (j < S.size())
        {
            size_t v = j;
            size_t local_position = 1;
            size_t path_len = 0;
            size_t node = 0;

            while (true)
            {
                if (v == S.size())
                {
                    size_t d = S.size() - j;
                    for (size_t z = T[node].l_bound; z <= T[node].u_bound; z++)
                    {
                        if (all_pairs[i][z] == 0)
                        {
                            all_pairs[i][z] = d;
                            adj[i].push_back[z];

                            if (d > max_val)
                            {
                                max_val = d;
                                max_pair.first = i;
                                max_pair.second = z;
                            }
                        }
                    }

                    break;
                }

                char g1 = S[v];

                if (T[node].chain_len >= local_position)
                {
                    char g2 = G[T[node].l_bound][path_len];

                    if (g1 == g2)
                    {
                        local_position++;
                        path_len++;
                        v++;
                    }
                    else
                    {
                        break;
                    }
                }
                else
                {
                    map<char, size_t>::iterator b = T[node].branches.find(g1);

                    if (b != T[node].branches.end())
                    {
                        node = b->second;
                        local_position = 1;
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
}
