//
//  optimal_kmer_size.cpp
//
//  Created by Ryan Cormier on 6/26/20.
//
//  Given a list of error-free reads, this program determines the maximum
//  k such that a de Brujin graph created from the k-length fragments
//  of the reads has a single possible Eulerian cycle. A suffix tree is
//  constructed and used to find all suffix-prefix matches. Only the
//  maximum overlap for each read is tracked. The optimal k-mer size will
//  be one more than the minimum of these maximums.
//
#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;

const int NUM_READS = 400;
const int READ_SIZE = 100;
const char TERM_CHAR = '$';

// The reads
vector<string> G( NUM_READS );

// The maximum suffix length of a suffix-prefix match for each read.
int MAX_OVERLAP[ NUM_READS ];

// by solving the All-Pairs Suffix-Prefix problem, it is easy to find
// the maximum overlap for each read.
void SolveAPSP();

// Build the suffix tree for the APSP problem
void BuildTree();

int main()
{
    for ( int i = 0 ; i < NUM_READS ; i++ )
    {
        cin >> G[ i ];
        G[ i ].push_back( TERM_CHAR );
    }
    
    SolveAPSP();
    
    int min_max = 100;
    
    //  the optimal k is 1 more than the minimum of the maximum overlaps.
    for ( int i = 0 ; i < NUM_READS ; i++ )
        min_max = ( MAX_OVERLAP[ i ] < min_max ? MAX_OVERLAP[ i ] : min_max );
    
    cout << min_max + 1 << endl;
    
    return 0;
}

// The generalized suffix tree is a modified version of the work by
// Mark Nelson. See stree2006.cpp at marknelson.us.
struct Suffix
{
    int origin;
    int first;
    int last;
    int string_index;
    Suffix( int s, int n, int f, int l )
    : string_index( s ), origin( n ), first( f ), last( l ) {}
    int Explicit() { return ( first > last ); }
    int Implicit() { return ( last >= first ); }
    void Canonize();
    int span() { return ( last - first ); }
    int length() { return ( ( last - first ) + 1 ); }
};

struct Edge
{
    int start_node;
    int end_node;
    int first;
    int last;
    int string_index;
    Edge() { start_node = -1; }
    Edge( int s, int n, int a, int b );
    void Insert();
    void Remove();
    int Split( Suffix& s );
    static Edge Find( int node, int c );
    static int Hash( int node, int c );
    string label();
    int span() { return ( last - first ); }
    int length() { return ( ( last - first ) + 1 ); }
};

struct LeafIndex
{
    int string_index;
    int pos;
    LeafIndex() { pos = -1; }
    LeafIndex( int s, int p ) : string_index( s ), pos( p ) {}
};

struct Node
{
    int suffix_node;
    int path_length;
    vector<int> L;
    vector<LeafIndex> Indices;
    bool is_leaf() { return ( ! Indices.empty() ); }
    static int Count;
    Node() { suffix_node = -1; }
};

// The combined length of all the reads ( including a terminal character )
// and a very large prime number;
//
const int MAX_LENGTH = 40100;
const int HASH_TABLE_SIZE = 75011;

vector<Edge> Edges( HASH_TABLE_SIZE );
vector<Node> Nodes( MAX_LENGTH * 2 );
int Node::Count = 1;

// A recursive depth first search for SolveAPSP().
void DFS( int node, vector< stack<int>>& S )
{
    if ( ! Nodes[ node ].is_leaf() ) // Internal node
    {
        // if Incident to treminal edge, push on stack
        for ( int k = 0 ; k < Nodes[ node ].L.size() ; k++ )
        {
            int i = Nodes[ node ].L[ k ];
            S[ i ].push( node );
        }
        
        // follow all branches
        string alphabet = "ACGT";
        alphabet.push_back( TERM_CHAR );
        for ( int a = 0 ; a < alphabet.size() ; a++ )
        {
            char c = alphabet[ a ];
            Edge e = Edge::Find( node, c );
            
            if ( e.start_node == node )
            {
                DFS( e.end_node, S );
            }
        }
        
        // Moving back up
        for ( int k = 0 ; k < Nodes[ node ].L.size() ; k++ )
        {
            int i = Nodes[ node ].L[ k ];
            
            S[ i ].pop();
        }
    }
    else    // Leaf node
    {
        // check if the path to here is a full read
        for ( int k = 0 ; k < Nodes[ node ].Indices.size() ; k++ )
        {
            if ( Nodes[ node ].Indices[ k ].pos == 0 )
            {
                for ( int i = 0 ; i < S.size() ; i++ )
                {
                    if ( ! S[ i ].empty() )
                    {
                        // the top of the stack is the largest j -> i overlap
                        int v = S[ i ].top();
                        
                        // the magnitude of the overlap is the length of the
                        // path label.
                        int d = Nodes[ v ].path_length;
                        
                        // we're only tracking maximums
                        if ( d > MAX_OVERLAP[ i ] )
                            MAX_OVERLAP[ i ] = d;
                    }
                }
            }
        }
        
    }
}

/*
 *  First, create a generalized suffix tree from the set of reads and a set of
 *  stacks, one for each read. Using a depth first search, any node v incedent
 *  to a terminal edge has the indicess of the corresponding reads in a
 *  container L[ v ]. Push this node onto the i-th stack for all i  in L[ v ].
 *  When a leaf node is reached and the path label is an entire read, then the
 *  path length of the node on the top of a stack is a maximum suffix prefix
 *  match. A stack is empty when there is no overlap. When the search moves
 *  back above a node v, pop the top of the i-th stack for each i in L[ v ].
 *  See Dan Gusfield's "Algorithms on Strings, Trees, and Sequences."
 */
void SolveAPSP()
{
    // Construct the suffix tree and stacks;
    BuildTree();
    
    vector< stack<int>> S( NUM_READS );
    
    // DFS from root; and that's it
    DFS( 0, S );
}

// Suffix Tree implementation.
Edge::Edge( int s, int n, int a, int b )
{
    string_index = s;
    start_node = n;
    end_node = Node::Count++;
    first = a;
    last = b;
}

int Edge::Hash(int node, int c)
{
    return ( ( node << 8 ) + c ) % HASH_TABLE_SIZE;
}

void Edge::Insert()
{
    int i = Hash( start_node, G[ string_index ][ first ] );
    
    while ( Edges[ i ].start_node != -1 )
        i = ++i % HASH_TABLE_SIZE;
    Edges[ i ] = *this;
}

void Edge::Remove()
{
    int i = Hash( start_node, G[ string_index ][ first ] );
    while ( Edges[ i ].start_node != start_node ||
           Edges[ i ].first != first )
        i = ++i % HASH_TABLE_SIZE;
    
    while ( true )
    {
        Edges[ i ].start_node = -1;
        int j = i;
        
        while ( true )
        {
            i = ++i % HASH_TABLE_SIZE;
            if ( Edges[ i ].start_node == -1 )
                return;
            int r = Hash( Edges[ i ].start_node, Edges[ i ].label()[ 0 ] );
            
            if ( i >= r && r > j )
                continue;
            if ( r > j && j > i )
                continue;
            if ( j > i && i >= r )
                continue;
            break;
        }
        
        Edges[ j ] = Edges[ i ];
    }
}

Edge Edge::Find( int node, int c )
{
    int i = Hash( node, c );
    while ( true )
    {
        if ( Edges[ i ].start_node == node )
            if ( c == Edges[ i ].label()[ 0 ] )
                return Edges[ i ];
        
        if ( Edges[ i ].start_node == -1 )
            return Edges[ i ];
        i = ++i % HASH_TABLE_SIZE;
    }
}

int Edge::Split( Suffix &s )
{
    Remove();
    Edge *new_edge = new Edge( s.string_index, s.origin,
                              s.first, s.first + s.span() );
    
    new_edge->Insert();
    first += s.length();
    start_node = new_edge->end_node;
    
    Insert();
    
    return new_edge->end_node;
}

string Edge::label()
{
    if ( start_node == -1 )
        return string();
    return G[ string_index ].substr( first, length() );
}

void Suffix::Canonize()
{
    if ( !Explicit() )
    {
        Edge edge = Edge::Find( origin,  G[ string_index ][ first ] );
        
        while ( edge.span() <= this->span() )
        {
            first = first + edge.length();
            origin = edge.end_node;
            
            if ( first <= last )
                edge = Edge::Find( edge.end_node, G[ string_index ][ first ] );
        }
    }
}

void AddPrefix( Suffix &active, int last_char_index )
{
    int parent_node;
    int last_parent_node = -1;
    string T( G[ active.string_index ]);
    
    while ( true )
    {
        Edge edge;
        parent_node = active.origin;
        
        if ( active.Explicit() )
        {
            edge = Edge::Find( active.origin, T[ last_char_index ] );
            if ( edge.start_node != -1 )
                break;
        }
        else
        {
            edge = Edge::Find( active.origin, T[ active.first ] );
            if ( G[ edge.string_index ][ edge.first + active.length() ] ==
                G[ active.string_index ][ last_char_index ] )
                break;
            
            parent_node = edge.Split( active );
        }
        Edge *new_edge = new Edge( active.string_index, parent_node,
                                  last_char_index, READ_SIZE );
        new_edge->Insert();
        
        if ( last_parent_node > 0 )
            Nodes[ last_parent_node ].suffix_node = parent_node;
        last_parent_node = parent_node;
        
        if ( active.origin == 0 )
            active.first++;
        else
            active.origin = Nodes[ active.origin ].suffix_node;
        active.Canonize();
    }
    if ( last_parent_node > 0 )
        Nodes[ last_parent_node ].suffix_node = parent_node;
    active.last++;
    active.Canonize();
}

void Walk()
{
    for ( int j = 0 ; j < NUM_READS ; j++ )
    {
        string s( G[ j ] );
        
        for ( int k = 0 ; k <= READ_SIZE ; k++ )
        {
            int i = k;
            int node = 0;
            int path_len = 0;
            
            while ( true )
            {
                Nodes[ node ].path_length = path_len;
                if ( i > READ_SIZE )
                    break;
                char c = s[ i ];
                Edge e = Edge::Find( node , c);
                
                if ( e.label().back() == TERM_CHAR )
                {
                    Nodes[ e.end_node ].Indices.push_back( LeafIndex( j, k ) );
                    
                    if ( e.length() == 1 )
                        Nodes[ e.start_node ].L.push_back( j );
                }
                path_len += e.length();
                i += e.length();
                node = e.end_node;
            }
        }
    }
}

void BuildTree()
{
    for ( int i = 0 ; i < NUM_READS ; i++ )
        if ( G[ i ].back() != TERM_CHAR )
            G[ i ].push_back( TERM_CHAR );
    
    for ( int i = 0 ; i < NUM_READS ; i++ )
    {
        Suffix active( i, 0, 0, -1 );
        
        for ( int j = 0 ; j <= READ_SIZE ; j++ )
            AddPrefix( active, j );
    }
    Walk();
}
