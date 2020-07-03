//
//  tip_removal.cpp
//
//  Created by Ryan Cormier on 6/3/20
//
//  This program simulates the task of tip removal on a De Bruijn graph.
//  The graph is constructed from the set of 15-mers generated from a list
//  of error-prone reads. When finished, the number of tips found is printed
//  to standard output.
//
#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <string>
#include <vector>

using namespace std;

typedef vector< vector<int>> Table;

//  Values given in the problem. The number of reads is given as 400, but
//  according the some forum rumors, there may be many more. Just keep pushing
//  reads into a vector until they stop coming and then set NUM_READS.
int NUM_READS;
const int READ_SIZE = 100;
const int HASH_TABLE_SIZE = 35023;
const int K = 15;
hash<string> hash_string;

//  Home for the error-prone reads.
vector<string> Reads;

//  Interface for the graph used in the main() function.
namespace Graph
{
    void Build();
    Table FindSCCs();
}

//  First build a De Bruijn graph and then find the strongly connected
//  components. Any component with a single element corresponds to a tip.
//  The number of tips is printed to standard output.
int main()
{
    for ( string s ; cin >> s ; )
        Reads.push_back( s );
    NUM_READS = static_cast<int>( Reads.size() );
    
    // Build the graph
    Graph::Build();
    
    // find the components
    Table scc = Graph::FindSCCs();
    
    // count the tips
    int count = 0;
    for ( auto cc : scc )
        if ( cc.size() == 1 ) count++;
    
    cout << count << endl;
    
    return 0;
}

//
//  The implementer's interface
//
namespace Graph
{
//  The vertex for the De Bruijn graph. The vertex label is a (k - 1)mer from
//  some read[ i ]. the label is stored by the index of the read in
//  READs and by the position of the label's first caracter.
struct Vertex
{
    int key, id, pos;
    Vertex( int i, int j ) : id( i ), pos( j ) {}
    Vertex() : Vertex( -1, 0 ) {}
    int Insert();
    string Label() const;
};
// The number of vertices in the graph.
int V;

//  The vertices and edges of the graph.
vector<Vertex> Vertices;
Table Adj;

//  A hash table and hash function for building the graph.
vector<int>* HashTable;
int Hash( const string& );

// Find the post order of the vertices using a depth-first search.
vector<int> PostOrder( );
void PostVisit( int, vector<int>&, vector<bool>& );

// recursive call for FindSCCs.
void SCCVisit( int, int, Table&, vector<bool>& );
}

// ------------------------------------------------------------------------- //
//                          The Implementation                               //
//-------------------------------------------------------------------------- //

//  Use a DFS on the inverse graph ordering the vertices by reverse
//  post order on the original graph.
Table Graph::FindSCCs()
{
    // generate the inverse
    Table inverse_graph( V );
    for ( int u = 0 ; u != V ; u++ )
        for ( auto v : Adj[ u ] )
            inverse_graph[ v ].push_back( u );
    // Set up for the recursive call
    Table scc;
    int cc = 0;
    vector<bool> visited( V, 0 );
    vector<int> order = PostOrder();
    reverse( order.begin(), order.end() );
    for ( auto u : order )
        if ( ! visited[ u ] )
            SCCVisit( u, cc++, scc, visited );
    
    return scc;
}

//  Connect the (K - 1)-mers in each K-mer. Use a hash table to quickly
//  process a new node and avoid duplicate labels. If the edge is unique and
//  not a self-loop, add it to the graph.
void Graph::Build()
{
    // Hash the vertex labels and store the vertex key.
    HashTable = new vector<int>( HASH_TABLE_SIZE, -1 );
    
    // use set to avoid parallell edges
    vector< set<int>> unique_adj;
    
    //  For each read
    for ( int i = 0 ; i < NUM_READS ; i++ )
    {
        // For each K-mer
        for ( int j = 0 ; j <= READ_SIZE - K ; j++ )
        {
            //  Create an edge from the prefix (K - 1)-mer to the
            //  suffix (K - 1)-mer.
            int u = Vertex( i, j ).Insert();
            int v = Vertex( i, j + 1 ).Insert();
            
            while ( unique_adj.size() <= max( u, v ) )
                unique_adj.push_back( set<int>() );
            
            unique_adj[ u ].insert( v );
        }
    }
    // clean up and fill in the adjacency list.
    delete [] HashTable;
    V = static_cast<int>( unique_adj.size() );
    Adj.resize( V );
    for ( int u = 0 ; u < V ; u++ )
        for ( auto v : unique_adj[ u ] )
            Adj[ u ].push_back( v );
}

// Recursive DFS on the Inverse graph.
void Graph::SCCVisit( int u, int cc, Table& scc, vector<bool>& visited, Table& inverse_adj )
{
    visited[ u ] = 1;
    
    while ( scc.size() <= cc )
        scc.push_back( vector<int>() );
    scc[ cc ].push_back( u );
    
    for ( auto v : InverseAdj[ u ] )
        if ( ! visited[ v ] )
            SCCVisit( v, cc, scc, visited );
}

// The order of the vertices by when they are fully processed during a DFS.
vector<int> Graph::PostOrder( )
{
    vector<int> order;
    vector<bool> visited( V, 0 );
    
    for ( int u = 0 ; u != V ; u++ )
        if ( ! visited[ u ] )
            PostVisit( u, order, visited );
    
    return order;
}

// recursive DFS. The order is tracked buy when the call is finished.
void Graph::PostVisit( int u, vector<int>& order, vector<bool>& visited )
{
    visited[ u ] = 1;
    for ( auto v : Adj[ u ] )
        if ( ! visited[ v ] )
            PostVisit( v, order, visited );
    order.push_back( u );
}

//  Vertices have a unique label, so only the label needs to be hashed.
int Graph::Hash( const string & s ) {
    return hash_string( s ) % HASH_TABLE_SIZE; }

//  The vertex has the string index and starting position of the label
//  and lenght is a global variable.  Just make sure the vertex has been
//  initialized and return the corresponding substring.
string Graph::Vertex::Label() const
{
    string res;
    if ( 0 <= id && id < NUM_READS )
        res = Reads[ id ].substr( pos, K - 1 );
    return res;
}

//  Hash the label and either find an unused slot in the table or the index
//  of an existing vertex. If the slot is unused, set the key to the number
//  of vertices already inserted, append the vertex to the Vertices container,
//  and insert the key into the table. If the slot is used, compare the label
//  of the vertex corresponding to the table value. If equal, set the key to
//  the table value. If not, step forward until one of the first two cases
//  occur. Return the key when done.
int Graph::Vertex::Insert()
{
    int i = Hash( Label() );
    
    for ( ; ; )
    {
        if ( HashTable[ i ].first.empty() )
        {
            key = static_cast<int>( Vertices.size() );
            HashTable[ i ] = { Label(), key };
            Vertices.push_back( *this );
            break;
        }
        if ( HashTable[ i ].first == Label() )
        {
            key = HashTable[ i ].second;
            break;
        }
        i = ++i % HASH_TABLE_SIZE;
    }
    return key;
}
