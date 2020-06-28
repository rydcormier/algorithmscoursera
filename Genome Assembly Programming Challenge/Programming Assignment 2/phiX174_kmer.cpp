//
//  phiX174_kmer.cpp
//
//  Created by Ryan Cormier on 6/28/20.
//
//  This program assembles the phiX174 phage genome from the given the
//  k-mer composition. The input data consists of 5396 k-mers each
//  10 nucleotides long.
//
#include <functional>
#include <iostream>
#include <list>
#include <string>
#include <vector>

using namespace std;

const int NUM_KMERS = 5396;
const int K = 10;
const int HASH_TABLE_SIZE = 12503;

string KMERS[ NUM_KMERS ];

//
//  The Genome is assembled using an Eulerian Path from the De Bruijn
//  Graph of the given k-mers.
//
struct DeBruignGraph
{
    //
    //  The vertex label is either a prefix or a suffix of a read. All we
    //  need to store is the index of the read and a boolean value for the
    //  position of the first character. Because all the labels are length
    //  K - 1, the start will either be at index 0 or 1.
    //
    struct Vertex
    {
        int string_index;
        bool pos;
        Vertex() { string_index = -1; }
        Vertex( int i, bool j ) : string_index( i ), pos( j ) {}
        static int Hash( const string& label );
        int Insert();
        string Label() const;
        bool operator==( const Vertex& rhs ) const;
    };
    
    //
    //  The Vertices are stored in a hash table and the edges in an
    //  adjacency list.
    static vector<Vertex> Vertices;
    static vector< list<int>> Adj;
};
typedef DeBruignGraph G;
typedef G::Vertex Vertex;

vector<Vertex> G::Vertices( HASH_TABLE_SIZE );
vector< list<int>> G::Adj( HASH_TABLE_SIZE );

// Forward references
list<int> EulerianPath( int u );
hash<string> hash_string;

int main()
{
    // populate KMERS
    for ( int i = 0 ; i < NUM_KMERS ; i++ )
        cin >> KMERS[ i ];
    
    // Construct the graph and store the index of the first vertex used.
    int w = -1;
    for ( int i = 0 ; i < NUM_KMERS ; i++ )
    {
        int u = Vertex( i, 0 ).Insert();   // prefix
        int v = Vertex( i, 1 ).Insert();   // suffix
        
        if ( w < 0 )    // u is the first vertex
            w = u;
        
        // store the edge u -> v;
        G::Adj[ u ].push_back( v );
    }
    
    // Find a Eulerian path
    list<int> path = EulerianPath( w );
    
    //  For each node in path, just add the last letter of the label.
    string genome;
    for ( list<int>::iterator it = path.begin() ; it != path.end() ; it++ )
        genome.push_back( G::Vertices[ *it ].Label().back() );
    
    cout << genome << endl;
}

//
//  Given a starting vertex, return a list of the nodes in an Eulerain
//  path on G.
//
list<int> EulerianPath( int u )
{
    list<int> path;
    path.push_back( u );
    
    while ( true )
    {
        //  Keep moving while edges are available
        while ( ! G::Adj[ u ].empty() )
        {
            path.push_back( G::Adj[ u ].back() );
            G::Adj[ u ].pop_back();
            u = path.back();
        }
        
        // All the edges have been used
        if ( path.size() >= NUM_KMERS - 1 )
            break;
        
        // We're stuck. Backtrack until an edge is available.
        do {
            path.pop_back();
            u = path.back();
            path.push_front( u );
        } while ( G::Adj[ u ].empty() );
    }
    
    // we want a path, not a cycle.
    while ( path.back() == path.front() )
        path.pop_back();
    
    return path;
}

int Vertex::Hash( const string& label ) {
    return hash_string( label ) % HASH_TABLE_SIZE; }

string Vertex::Label() const {
    return string( KMERS[ string_index ].substr( pos, K - 1) ); }

//
//  The vertex inserts a copy of itself in the first unused slot in the
//  table. In the case of a colision, if the vertex has the same label,
//  we are done - labels are unique - else step forward until there is
//  an equivalent label or there is an unused slot.
//
int Vertex::Insert()
{
    int i = Hash( this->Label() );
    
    while ( true )
    {
        if ( G::Vertices[ i ].string_index == -1 )
        {
            G::Vertices[ i ] = *this;
            break;
        }
        if ( G::Vertices[ i ] == *this )
            break;
        
        // step forward
        i = ++i % HASH_TABLE_SIZE;
    }
    
    return i;
}

//
//  Vertices are equivalent if they point to the same string and position
//  or they have equivalent labels.
//
bool Vertex::operator==( const Vertex& rhs ) const
{
    if ( string_index == rhs.string_index )
        return ( pos == rhs.pos );
    if ( string_index >= 0 )
        return ( Label() == rhs.Label() );
    return false;
}
