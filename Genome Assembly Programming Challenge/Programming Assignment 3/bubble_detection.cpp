//
//  bubble_detection.cpp
//
//  Created by Ryan Cormier on 6/26/20
//
//  Given a Kmer size, a path threshold, and a set of error-prone reads,
//  this program finds and returns the number of pairs of disjoint paths on
//  the corresponding De Bruijn graph.
//
#include <functional>
#include <iostream>
#include <string>
#include <set>
#include <vector>

using namespace std;

const int READ_SIZE = 100;
const int HASH_TABLE_SIZE = 35023;
hash<string> hash_string;

//  The input : the reads, the K in Kmer, and the path length threshold.
vector<string> READS;
int K, T;

//
//  The node for the De Bruijn graph. The node label is a (k - 1)mer from
//  some read[ i ]. the label is stored by the index of the read in
//  READs and by the position of the label's first caracter. Two nodes are
//  equivalent if their labels are equivalent, ensuring the De Bruign graph
//  will only contain unique (k - 1)mers. To assist in detecting bubbles, the
//  degree of each Node is tracked.
//
struct Node
{
    int id, pos, in, out;
    Node( int i, int p ) : id( i ), pos( p ), in( 0 ), out( 0 ) {}
    Node() : Node( -1, -1 ) {}
    static int Hash( const string& );
    string Label() const;
    int Insert();
    bool operator==( const Node& n ) const { return ( Label() == n.Label() ); }
};

//
//  A path on the graph: a starting node, an ending node, and interior nodes-
//  if there are any. Paths are disjoint if the share only the start and
//  end nodes.
//
struct Path
{
    int start_node, end_node;
    vector<int> interior_nodes;
    Path( int u ) : start_node( u ), end_node( -1 ) {}
    Path() : Path( -1 ) {}
    Path( const vector<int>& );
    int back() const { return ( end_node < 0 ? start_node : end_node ); }
    int front() const { return start_node; }
    int size() const;
    void push_back( int );
    void pop_back();
};

//
//  The De Bruign graph: a Hash table for the nodes and a set of adjacencies
//  for each node.
//
Node Nodes[ HASH_TABLE_SIZE ];
set<int> Adj[ HASH_TABLE_SIZE ];

//  Once the reads are stored, Each read is broken down by Kmers. For each
//  Kmer, a directed edge is added to the graph from the prefix-(K - 1)mer
//  to the suffix-(K - 1)mer.
//
void BuildGraph();

//
//  Walk a path from each node with out-degree greater than 1 using a depth-
//  first search. When a path comes to a node with in-degree greater than 1,
//  add it to a list. When all the paths from the start node within the path
//  length threshold are found, for each pair of paths, determine if it is
//  dijsoint. Each disjoint pair increases the bubble count by 1.
//
int CountBubbles();
void DFS( int, vector<Path>&, Path&, vector<bool>&);
bool Disjoint( const Path&, const Path& );

//
//  Pretty straight forward: read the input; build the graph; count the
//  bubbles.
//
int main()
{
    cin >> K >> T;
    for ( string s ; cin >> s ; )
        READS.push_back( s );
    
    BuildGraph();
    
    cout << CountBubbles() << endl;
    
    return 0;
}

int CountBubbles()
{
    int count = 0;
    
    // Walk a path from every node with out-degree greater than 1
    for ( int i = 0 ; i < HASH_TABLE_SIZE ; i++ )
    {
        if ( Nodes[ i ].out > 1 ) // candidate bubble start
        {
            vector<Path> paths;
            Path path;
            vector<bool> visited( HASH_TABLE_SIZE, 0 );
            
            DFS( i, paths, path, visited );
            
            // Any disjoint pair increases count
            if ( ! paths.empty() )
                for ( int i = 0 ; i < paths.size() - 1 ; i++ )
                    for ( int j = i + 1 ; j < paths.size() ; j++ )
                        if ( Disjoint( paths[ i ], paths[ j ] ) )
                            count++;
        }
    }
    return count;
}

void DFS( int u, vector<Path>& paths, Path& path, vector<bool>& visited )
{
    // check the threshold
    if ( path.size() >= T )
        return;
    
    // add u to the path
    visited[ u ] = 1;
    path.push_back( u );
    
    // if u is a possible bubble end node, add it to paths
    if ( path.size() > 0 && Nodes[ u ].in > 1 )
        paths.push_back( path );
    
    // Continue on every unused edge.
    for ( auto v : Adj[ u ] )
        if ( ! visited[ v ] )
            DFS( v, paths, path, visited );
    
    // Backtrack
    visited[ u ] = 0;
    path.pop_back();
}

//  Two paths are disjoint if the share starting and end nodes and do not
//  share any interior nodes.
bool Disjoint( const Path& p, const Path& q )
{
    if ( p.start_node != q.start_node || p.end_node != q.end_node )
        return false;
    for ( auto u : p.interior_nodes )
        for ( auto v : q.interior_nodes )
            if ( u == v )
                return false;
    return true;
}

// Build the De Bruijn graph
void BuildGraph()
{
    // Each read in reads
    for ( int i = 0 ; i < READS.size() ; i++ )
    {
        // each Kmer in read
        for ( int j = 0 ; j <= READ_SIZE - K ; j++ )
        {
            // each (K - 1)mer in Kmer
            int u = Node( i, j ).Insert();
            int v = Node( i, j + 1 ).Insert();
            
            auto p = Adj[ u ].insert( v );
            if ( p.second && u != v )
            {
                Nodes[ u ].out++;
                Nodes[ v ].in++;
            }
        }
    }
}

//
//  Node and Path implementations.
//
int Node::Hash( const string& s ) {
    return hash_string( s ) % HASH_TABLE_SIZE; }

string Node::Label() const
{
    string res;
    
    if ( 0 <= id && id < READS.size() )
        if ( 0 <= pos <= READ_SIZE - K )
            res = READS[ id ].substr( pos, K - 1 );
    return res;
}

//
//  The hash table uses a linear probe technique. After hashing the nodes label,
//  we either have an unused node, an equivalent node, or a different node.
//  for the first, copy the node into the table. For the second, do nothing.
//  For the third, step forward until one of the first two conditions are true.
//  When done, return the table index from where we stoppem.
//
int Node::Insert()
{
    int i = Hash( Label() );
    
    for ( ; ; )
    {
        if ( Nodes[ i ].id < 0 )
        {
            Nodes[ i ] = *this;
            break;
        }
        else if ( Nodes[ i ] == *this )
            break;
        
        i = ++i % HASH_TABLE_SIZE;
    }
    
    return i;
}

Path::Path( const vector<int>& v )
{
    start_node = ( v.empty() ? -1 : v.front() );
    for ( int i = 1 ; i < v.size() - 1 ; i++ )
        interior_nodes.push_back( v[ i ] );
    end_node = ( v.size() > 1 ? v.back() : -1 );
}

int Path::size() const
{
    int sz = 0;
    if ( 0 <= start_node && start_node < HASH_TABLE_SIZE )
    {
        if ( 0 <= end_node && end_node < HASH_TABLE_SIZE )
            sz++;
        sz += interior_nodes.size();
    }
    return sz;
}

void Path::push_back( int x )
{
    if ( start_node < 0 )
        start_node = x;
    else if ( end_node < 0 )
        end_node = x;
    else
    {
        interior_nodes.push_back( end_node );
        end_node = x;
    }
}

void Path::pop_back()
{
    if ( ! interior_nodes.empty() )
    {
        end_node = interior_nodes.back();
        interior_nodes.pop_back();
    }
    else if ( end_node >= 0 )
        end_node = -1;
    else
        start_node = -1;
}
