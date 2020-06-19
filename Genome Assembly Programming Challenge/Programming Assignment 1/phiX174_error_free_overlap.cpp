//
//  phiX174_error_free_overlap.cpp
//
//  Created by Ryan Cormier on 6/16/20.
//
//  This program assembles the phi X174 genome from 1168 simulated
//  error free reads 100 neucleotides long. First, The All-Pairs 
//  Suffix-Prefix problem is solved, and then a Hamiltonian path is
//  constructed on the resulting overlap graph.
//

#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;

const int NUM_READS = 1618;
const int READ_SIZE = 100;
const char TERM_CHAR = '$';
vector<string> G( NUM_READS );

// D[ i ][ j ] is the length of the overlapping substring in reads i and j.
//
vector< vector<int>> D( NUM_READS, vector<int>( NUM_READS ) );

// The adjacencies of the overlap graph  are sorted in decreasing
// order by weight. This makes a greedy construction of a Hamiltonian path
// easier.
//
vector< set< pair<int, int>>> ADJ( NUM_READS );
const int MIN_OVERLAP = 20;

// Each { i, j } in MAX_PAIRS indicates the overlap between reads i and j
// is a maximum and MAX is this maximum.
//
vector< pair<int, int>> MAX_PAIRS;
int MAX = 0;

// forward references
//
void ReadInput();
string SolveGreedy();
void SolveAPSP();
void BuildTree();

int main()
{
    for ( int i = 0 ; i < NUM_READS ; i++ )
    {
        cin >> G[ i ];
        G[ i ].push_back( TERM_CHAR );
    }

    SolveAPSP();
    string genome = SolveGreedy();
    
    cout << genome << endl;
    
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
const int MAX_LENGTH = 161900;
const int HASH_TABLE_SIZE = 500009;

// The suffix tree.
//
vector<Edge> Edges( HASH_TABLE_SIZE );
vector<Node> Nodes( MAX_LENGTH * 2 );
int Node::Count = 1;

// Brute force. A more efficient strategy is used below in SolveAPSP().
//
int overlap( string& a, string& b )
{
    int n = ( int )( a.length() < b.length() ? a.length() : b.length() );
    
    for ( int i = 0 ; i < n ; i++ )
    {
        if ( a.substr( i ) == b.substr( 0, n - i ) )
            return ( n - i );
    }
    
    return 0;
}

// For each pair of reads from MAX_PAIRS, construct a Hamiltonian path by
// select the outging edge to an available node of maximum weight. The label
// of minimum length is returned.
//
string SolveGreedy()
{
    string res;
    
    for ( int i = 0 ; i < MAX_PAIRS.size() ; i++ )
    {
        int u = MAX_PAIRS[ i ].first;
        int v = MAX_PAIRS[ i ].second;
        int d = D[ u ][ v ];
        string s( G[ u ] );
        s.pop_back();       // Don't forget to remove the terminal char
        s += G[ v ].substr( d );
        s.pop_back();
        vector<bool> used( NUM_READS );
        used[ u ] = 1;
        used[ v ] = 1;
        int count = 0;
        bool ok = 1;
        while ( count < NUM_READS && ok )
        {
            u = v;
            set< pair<int, int>>::iterator it = ADJ[ u ].begin();
            while ( it != ADJ[ u ].end() && used[ it->second ]  )
                it++;
            
            // Since we have a minimum on overlap, we might reach a dead end.
            if ( it == ADJ[ u ].end() )
            {
                ok = 0;
                break;
            }
            // ADJ[ u ] is already sorted so the next unused node is a maximum.
            v = it->second;
            d = D[ u ][ v ];
            s += G[ v ].substr( d );
            s.pop_back();
            count++;
        }
        
        if ( ok )
        {
            // Remove any overlap from the end of the string and the start.
            size_t l = res.length() / 2;
            string a( s.substr( s.length() - l ) );
            string b( s.substr( 0, l ) );
            int d = overlap( a, b );
            if ( d > 0 )
                s.erase( 0, d );

            if ( res.empty() || s.length() < res.length() )
                res = s;
        }
    }
    
    return res;
}



// A recursive depth first search is used for SolveAPSP().
//
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
                int j = Nodes[ node ].Indices[ k ].string_index;
                
                for ( int i = 0 ; i < S.size() ; i++ )
                {
                    if ( ! S[ i ].empty() )
                    {
                        // the top of the stack is the largest j -> i overlap
                        int v = S[ i ].top();
                        
                        // the magnitude of the overlap is the length of the
                        // path label.
                        int d = Nodes[ v ].path_length;
                        
                        D[ i ][ j ] = d;
                        
                        // Keep track of the largest overlaps to use
                        // as a starting point for reassembly.
                        if ( d >= MIN_OVERLAP )
                            ADJ[ i ].insert( { READ_SIZE - d, j } );
                        
                        if ( d == MAX )
                        {
                            MAX_PAIRS.push_back( { i, j } );
                        }
                        else if ( d > MAX )
                        {
                            MAX_PAIRS.clear();
                            MAX_PAIRS.push_back( { i, j } );
                            MAX = d;
                        }
                    }
                }
            }
        }
        
    }
}


// First, create a generalized suffix tree from the set of reads and a set of
// stacks, one for each read. Using a depth first search, any node v incedent 
// to a terminal edge has the indicess of the corresponding reads in a 
// container L[ v ]. Push this node onto the i-th stack for all i  in L[ v ]. 
// When a leaf node is reached and the path label is an entire read, then the
// path length of the node on the top of a stack is a maximum suffix prefix
// match. A stack is empty when there is no overlap. When the search moves 
// back above a node v, pop the top of the i-th stack for each i in L[ v ].
// See Dan Gusfield's "Algorithms on Strings, Trees, and Sequences."
//
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
