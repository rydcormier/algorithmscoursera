// apsp.h
//
// phiX174 assembly with error free reads and overlap graph

#ifndef apsp_h
#define apsp_h

#include <map>
#include <string>
#include <vector>

struct PrefixTree
{
    struct Node
    { 
        size_t l_bound = 0;
        size_t u_bound = 0;
        size_t chain_len = 0;
        std::map<char, size_t> branches;
    };
            
    std::vector<Node> _nodes; 
    
    PrefixTree(const std::vector<std::string>& G) : _nodes(1) { initialize(G); }
    
    void initialize(const std::vector<std::string>&);
    Node& operator[] (size_t);
};

struct APSP
{
    std::vector< std::vector<size_t>> all_pairs;
    std::vector< std::vector<size_t>> adj;
    std::pair<size_t, size_t> max_pair;


    void initialize(const std::vector<std::string>&, size_t min_weight = 0);
};

#endif /* apsp_h */
