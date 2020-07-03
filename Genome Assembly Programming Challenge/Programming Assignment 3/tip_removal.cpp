//
//  tip_removal.cpp
//
//  Created by Ryan Cormier on 6/3/20
//
#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <stack>
#include <string>
#include <vector>

using namespace std;

typedef vector< vector<int>> Table;

int NUM_READS;
const int READ_SIZE = 100;
const int HASH_TABLE_SIZE = 35023;
const int K = 15;
hash<string> hash_string;

vector<string> Reads;

namespace Graph {
void Build();
Table FindSCCs();
void print_debugging();
}

int main()
{
    for ( string s ; cin >> s ; )
        Reads.push_back( s );
    NUM_READS = static_cast<int>( Reads.size() );
    
    Graph::Build();
    
    Table scc = Graph::FindSCCs();
    
    int count = 0;
    for ( auto cc : scc )
        if ( cc.size() == 1 )
            count++;
    
    cout << count << endl;
    
    return 0;
}

namespace Graph
{
struct Node
{
    int key, id, pos;
    Node( int i, int j ) : id( i ), pos( j ) {}
    Node() : Node( -1, 0 ) {}
    int Insert();
    string Label() const;
};
int V;
vector<Node> Nodes;
Table Adj, RAdj;
pair<string, int>* HashTable;
int Hash( const string& );
void PostVisit( int, vector<int>&, vector<bool>& );
vector<int> PostOrder( );
vector<int> TopologicalSort( );
void SCCVisit( int, int, Table&, vector<bool>& );
}

Table Graph::FindSCCs()
{
    Table scc;
    int cc = 0;
    vector<bool> visited( V, 0 );
    
    vector<int> order = TopologicalSort();
    
    for ( auto u : order )
        if ( ! visited[ u ] )
            SCCVisit( u, cc++, scc, visited );
    
    return scc;
}

void Graph::Build()
{
    HashTable = new pair<string, int>[ HASH_TABLE_SIZE ];
    vector< set<int>> unique_adj;
    
    for ( int i = 0 ; i < NUM_READS ; i++ )
    {
        for ( int j = 0 ; j <= READ_SIZE - K ; j++ )
        {
            int u = Node( i, j ).Insert();
            int v = Node( i, j + 1 ).Insert();
            string ul { Nodes[ u ].Label() };
            string vl { Nodes[ v ].Label() };
            
            
            while ( unique_adj.size() <= max( u, v ) )
                unique_adj.push_back( set<int>() );
            
            unique_adj[ u ].insert( v );
        }
    }
    delete [] HashTable;
    V = static_cast<int>( unique_adj.size() );
    Adj.resize( V );
    RAdj.resize( V );
    
    for ( int u = 0 ; u < V ; u++ )
    {
        for ( auto v : unique_adj[ u ] )
        {
            Adj[ u ].push_back( v );
            RAdj[ v ].push_back( u );
        }
    }
}

void Graph::SCCVisit( int u, int cc, Table& scc, vector<bool>& visited )
{
    visited[ u ] = 1;
    
    while ( scc.size() <= cc )
        scc.push_back( vector<int>() );
    scc[ cc ].push_back( u );
    
    for ( auto v : RAdj[ u ] )
        if ( ! visited[ v ] )
            SCCVisit( v, cc, scc, visited );
}

vector<int> Graph::PostOrder( )
{
    vector<int> order;
    vector<bool> visited( V, 0 );
    
    for ( int u = 0 ; u != V ; u++ )
        if ( ! visited[ u ] )
            PostVisit( u, order, visited );
    
    return order;
}

vector<int> Graph::TopologicalSort( )
{
    
    vector<int> order = PostOrder( );
    
    reverse( order.begin(), order.end() );
    
    return order;
}

void Graph::PostVisit( int u, vector<int>& order, vector<bool>& visited )
{
    visited[ u ] = 1;
    for ( auto v : Adj[ u ] )
        if ( ! visited[ v ] )
            PostVisit( v, order, visited );
    order.push_back( u );
}

int Graph::Hash( const string & s ) {
    return hash_string( s ) % HASH_TABLE_SIZE; }

string Graph::Node::Label() const
{
    string res;
    if ( 0 <= id && id < NUM_READS )
        res = Reads[ id ].substr( pos, K - 1 );
    return res;
}

int Graph::Node::Insert()
{
    int i = Hash( Label() );
    
    for ( ; ; )
    {
        if ( HashTable[ i ].first.empty() )
        {
            key = static_cast<int>( Nodes.size() );
            HashTable[ i ] = { Label(), key };
            Nodes.push_back( *this );
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
