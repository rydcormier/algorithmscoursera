//
//  phiX174_error_free_overlap.cpp
//
//  Created by Ryan Cormier on 6/16/20.
//
#include <ctime>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;

typedef vector< vector<int>> Matrix;

const int TESTING = 0;
char* READS_FILE;

const int NUM_READS = 1168;
const int READ_SIZE = 100;
const char TERM_CHAR = '$';
vector<string> G( NUM_READS );

// The classes that follow are used create the generalized suffix tree. The
// implementation is a modified version of the code by Mark Nelson. See
// stree2006.cpp at marknelson.us.
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

// The length of all the reads combined ( including a terminating character )
// and a very large prime number;
const int MAX_LENGTH = 116900;
const int HASH_TABLE_SIZE = 500009;

// The suffix tree.
vector<Edge> Edges( HASH_TABLE_SIZE );
vector<Node> Nodes( MAX_LENGTH * 2 );
int Node::Count = 1;

// D[ i ][ j ] is the length of the overlapping substring in reads i and j.
Matrix D( NUM_READS, vector<int>( NUM_READS ) );

// By storing adjacencies separately, we can exclude any overlaps below a
// certain threshold, and by using sets, we can easily keep the adjacencis
// ordered by decreasing overlap.  This allows an easy greedy approach
// to finding a hamiltonion path.
vector< set< pair<int, int>>> ADJ( NUM_READS );
const int MIN_OVERLAP = 20;

// Each { i, j } in MAX_PAIRS indicates the overlap between reads i and j
// are a maximum.
vector< pair<int, int>> MAX_PAIRS;

// The maximum from MAX_PAIRS.
int MAX = 0;

// forward references
void ReadInput();
string SolveGreedy();
string SolveRecursive();
void SolveAPSP();
void BuildTree();

int main()
{
    time_t start, finish;
    
    if ( TESTING )
        start = time( NULL );
    
    ReadInput();
    
    SolveAPSP();
    string genome = SolveGreedy();
    
    if ( TESTING )
    {
        finish = time( NULL );
        printf( "Genome length: %lu\nProcess time: %.2f\n",
               genome.length(),
               difftime( finish, start ) );
        
    }
    else
        printf( "%s\n", genome.c_str() );
    
    return 0;
}

void ReadInput()
{
    if ( TESTING )
    {
        ifstream ifs;
        ifs.open( READS_FILE );
        if ( ifs.is_open() )
        {
            string line;
            int i;
            
            while ( getline( ifs, line ) )
            {
                line.push_back( TERM_CHAR );
                G[ i ] = line;
                i++;
            }
        }
        ifs.close();
    }
    else
    {
        for ( int i = 0 ; i < NUM_READS ; i++ )
        {
            cin >> G[ i ];
            G[ i ].push_back( TERM_CHAR );
        }
    }
}

// Brute force. A more efficient strategy is used below in SolveAPSP().
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

// With the first read from order, succesively append the substrings of the
// next read from order excluding any overlap. When done, just remove
// any overlap between the end of the string and the beginning.
string AssembleReads( vector<int>& order )
{
    string res( G[ order[ 0 ] ] );
    res.pop_back();
    int u, v, d;
    
    for ( int i = 0 ; i < NUM_READS - 1 ; i++ )
    {
        u = order[ i ];
        v = order[ i + 1 ];
        d = D[ u ][ v ];
        string a( G[ v ] );
        a.pop_back();
        res += a.substr( d );
    }
    
    size_t l = ( res.length() / 2 );
    string a( res.substr( res.length() - l ) );
    string b( res.substr( 0, l ) );
    d = overlap( a, b );
    
    res.erase( 0, d );
    
    return res;
}

// Recursive call to find a hamiltonian path.
void SolveVisit( int u, vector<int>& path, vector<bool>& visited )
{
    visited[ u ] = 1;
    path.push_back( u );
    if ( path.size() == NUM_READS ) // We have a Hamiltonian path.
        return;
    
    // Iterate through adjacencies which are already sorted in decreasing
    // order by overlap.
    set< pair<int, int>>::iterator it;
    for ( it = ADJ[ u ].begin() ; it != ADJ[ u ].end() ; it++ )
    {
        int v = it->second;
        if ( !visited[ v ] )
            SolveVisit( v, path, visited );
        
        if ( path.size() == NUM_READS ) // we're done
            return;
    }
    // Dead end => backtrack
    path.pop_back();
}

// Starting from the end node for each maximum edge in MAX_PAIRS, Use a
// "greedy" dfs to find a hamilton path, if it exists. Use the path to
// assemble the reads and return the string with minimum length.
string SolveRecursive()
{
    vector<int> path;
    string res;
    
    for ( int i = 0 ; i < MAX_PAIRS.size() ; i++ )
    {
        path.clear();
        vector<bool> visited( NUM_READS );
        int u = MAX_PAIRS[ i ].first;
        int v = MAX_PAIRS[ i ].second;
        
        // u -> v has maximum weight.
        path.push_back( u );
        visited[ u ] = 1;
        SolveVisit( v, path, visited ); // greedy start
        
        if ( path.size() == NUM_READS ) // hamiltonian path
        {
            string s = AssembleReads( path );
            
            if ( res.empty() || s.length() < res.length() )
                res = s;
        }
    }
    return res;
}

// Like above but just straigt greedy.
string SolveGreedy()
{
    string res;
    
    for ( int i = 0 ; i < MAX_PAIRS.size() ; i++ )
    {
        int u = MAX_PAIRS[ i ].first;
        int v = MAX_PAIRS[ i ].second;
        int d = D[ u ][ v ];
        string s( G[ u ] );
        s.pop_back();
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
            
            if ( it == ADJ[ u ].end() )
            {
                ok = 0;
                break;
            }
            
            v = it->second;
            d = D[ u ][ v ];
            s += G[ v ].substr( d );
            s.pop_back();
            count++;
        }
        
        if ( ok )
        {
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



// A recursive function for the DFS in SolveAPSP().
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
                        
                        int d = Nodes[ v ].path_length;
                        
                        D[ i ][ j ] = d;
                        
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


// Using the strategy outlined by Dan Gusfield, First, create a general suffix
// tree from the set of reads and a stack for each read. While performing a
// depth first search, any node v incedent to a terminal edge has the indeces of
// the reads whose suffixes terminate at that node in a container L[ v ]. Push
// this node onto of the stack i for each i in L[ v ]. When a leaf node is
// reached representing a complete read, then the top of the i-th stack is
// the node whose path length is the maximum suffix prefix match between the
// corresponding reads. . No overlap occurs when the stack is empty. When
// the search moves back above an internal node v, pop the top the stack for
// each index i such that i is in L[ v ]. See Gusfield's "Algorithms on
// Strings, Trees, and Sequences."
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
