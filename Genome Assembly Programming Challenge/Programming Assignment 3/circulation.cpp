//
//  circulation.cpp
//
//  Created by Ryan Cormier on 6/25/20
//
//  Given a flow network with lower bounds, this program determines if
//  circulation is feasible. If it is feasible the flow f( u, v ) for each
//  ( u, v ) int the network is printed to standard output.
//
#include <algorithm>
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <vector>

using namespace std;

/*
 *  The flow network is a directed graph with each edge having a flow value
 *  f( u, v ) subject to the condition that l( u, v ) <= f <= c( u, v ), with
 *  l being the given lower bound and c being the given capacity. There is also
 *  the condition that flow is conserved: the amount of flow coming into
 *  a vertex equals the flow leaving that vertex. We add a source and a
 *  sink connected to every vertex and solve the equivalent max flow problem.
 */
struct FlowNetwork
{
    //
    //  A directed edge ( u, v ) with a positive real valued flow ( f ), a
    //  lower bound ( l ), and a capacity ( c ).
    //
    struct Edge
    {
        int u, v, l, f, c;
        Edge( int u_, int v_, int l_, int c_ )
        :   u( u_ ), v( v_ ), l( l_ ), c( c_ ), f( 0 ) {}
    };
    
    //
    //  The graph: a set of E edges and an adjacency list mapping V vertices
    //  to corresponding outgoing edges. We also track the sum of all the
    //  lower bounds going into and out of each vertex and the total in the
    //  variable D. The variable S and T are the source and the sink
    //  respectively.
    //
    vector<Edge> Edges;
    vector< vector<int>> Adj;
    vector<int> total_in;
    vector<int> total_out;
    int E, D, V, S, T;
    
    void AddEdge( int u, int v, int l, int c );
    void AddFlow( int edge_index, int flow );
};
typedef FlowNetwork::Edge Edge;

//  BFS to find augmenting path for MaxFlow
list<int> ShortestPath( FlowNetwork& G, int s, int t );

/*
 *  We use the Edmonds-Karp strategy: as long as a path exists between the
 *  source and the sink in the residual graph, we choose the augmenting path
 *  with minimum unit distance and augment the flow of each edge of the path
 *  by min( c( u, v ) - f( u, v ) ) for all ( u, v ) in path. When a path
 *  no longer exists we have a maximum flow on the network.
 */
int MaxFlow( FlowNetwork& G )
{
    int Flow = 0;

    // Find the augmenting path while one exists;
    list<int> path = ShortestPath( G, G.S, G.T );
    
    while ( ! path.empty() )
    {
        //  The path can only augment flow by the minimum of the difference
        //  between an edge's capacity and flow.
        int f = 50;
        for ( auto id = path.begin() ; id != path.end() ; id++ )
        {
            int delta = G.Edges[ *id ].c - G.Edges[ *id ].f;
            f = ( delta < f ? delta : f );
        }

        //  Augment the path and update the flow
        for ( auto id = path.begin() ; id != path.end() ; id++ )
            G.AddFlow( *id, f );
        
        Flow += f;
        
        // Find the next augmenting path
        path = ShortestPath( G, G.S, G.T );
    }
    
    return Flow;
}

FlowNetwork ReadInput( );

int main()
{
    FlowNetwork G = ReadInput( );
    
    int F = MaxFlow( G );
    
    //  A circulation is feasible iff the maximum flow is equal to the sum
    //  of all the lower bounds.
    if ( F != G.D )
        cout << "NO" << endl;
    else
    {
        cout << "YES" << endl;
        for ( int i = 0 ; i < G.E ; i++ )
        {
            Edge forward = G.Edges[ 2 * i ];
            Edge backward = G.Edges[ ( 2 * i ) + 1 ];
            int flow = abs( backward.f ) + forward.l;
            
            cout <<  flow << endl;
        }
    }
    
    return 0;
}

/*
 *  Returns the shortest path s -> ... -> t unit distance using a
 *  breadth-first search.
 */
list<int> ShortestPath( FlowNetwork& G, int s, int t )
{
    list<int> path;
    vector<bool> discovered( G.V, 0);
    vector<int> prev( G.V, 0 );
    queue<int> Q;
    
    // start the BFS from s
    Q.push( s );
    discovered[ s ] = 1;
    while ( ! Q.empty() )
    {
        int vertex = Q.front();
        Q.pop();

        vector<int> edges = G.Adj[ vertex ];
        //
        //  For each adjacent edge, if the end vertex is undiscovered and
        //  if flow is less than  capacity, add the end node to the queue.
        //
        for ( int i = 0 ; i < edges.size() ; i++ )
        {
            Edge e = G.Edges[ edges[ i ] ];

            if ( ! discovered[ e.v ] && e.f < e.c )
            {
                Q.push( e.v );
                discovered[ e.v ] = 1;
                prev[ e.v ] = edges[ i ];

                if ( e.v == t ) // we're done
                    break;
            }
        }
    }
    
    //  if t was discovered, construct the path following prev[ t ]
    if ( discovered[ t ] )
    {
        int v = t;
        while ( v != s )
        {
            path.push_front( prev[ v ] );
            v = G.Edges[ prev[ v ] ].u;
        }
    }
    return path;
}

void FlowNetwork::AddEdge( int u, int v, int l, int c )
{
    //  By adding a residual edge to the graph after the forward edge,
    //  all forward edges will have even indices, and all backward edges
    //  will have odd indices. Also note we subtract the lower bound from the
    //  capacity of the forward edge.
    Edge forward( u, v, l, c - l );
    Edge backward( v, u, 0, 0 );

    int idx = ( int ) Edges.size();
    Adj[ u ].push_back( idx );
    Edges.push_back( forward );
    
    idx = ( int ) Edges.size();
    Adj[ v ].push_back( idx );
    Edges.push_back( backward );
    
}

void FlowNetwork::AddFlow( int edge_index, int flow )
{
    //  Add the flow to the corresponding edge and subtract it from the
    //  residual if edge_index is even or the opposite if odd. Either way
    //  the code is the same.
    
    Edges[ edge_index ].f += flow;
    Edges[ edge_index ^ 1 ].f -= flow;
    
}

FlowNetwork ReadInput( )
{
    FlowNetwork G;
    cin >> G.V >> G.E;
    
    
    // add a source and a sink
    G.S = G.V++;
    G.T = G.V++;
    G.Adj.resize( G.V );
    G.total_in.resize( G.V );
    G.total_out.resize( G.V );
    for ( int i = 0 ; i < G.E ; i++ )
    {
        int u, v, l, c;
        cin >> u >> v >> l >> c;
        G.AddEdge( u - 1, v - 1, l, c );
        G.D += l;
        G.total_in[ v - 1 ] += l;
        G.total_out[ u - 1 ] += l;
    }
    
    for ( int v = 0 ; v < G.S ; v++ )
    {
        G.AddEdge( G.S, v, 0, G.total_in[ v ] );
        G.AddEdge( v, G.T, 0, G.total_out[ v ] );
    }
    return G;
}
