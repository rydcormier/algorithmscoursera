//
//  eulerian_cycle.cpp
//
//  Created by Ryan Cormier on 6/19/20.
//
//  Given a strongly connected directed graph, this program determines
//  if an Eulerian path exists. If so, such a path is printed to
//  standard output.
//

#include <iostream>
#include <list>
#include <vector>

using namespace std;

// |V|, |E|, and the adjacency list for G< V, E >
int V, E;
vector< vector<int>> ADJ;

int ReadInput();

/*
 *  Starting from the given vertex, walk the graph until there are no more
 *  unused edges from the current vertex. If all the edges have been used, the
 *  path is returned. If not, we backtrack the path until there is a vertex
 *  with an unused edge.  We retrace the whole path from the new vertex and
 *  then continue on an unused edge.
 */
list<int> EulerianPath( int u )
{
    list<int> path;
    path.push_back( u );
    
    while ( true )
    {
        // Keep on moving while we can
        while ( !ADJ[ u ].empty() )
        {
            path.push_back( ADJ[ u ].back() );
            ADJ[ u ].pop_back();
            u = path.back();
        }
        
        // we're done
        if ( path.size() >= E )
            break;
        
        // backtrack and retrace
        do
        {
            path.pop_back();
            u = path.back();
            path.push_front( u );
        }
        while ( ADJ[ u ].empty() );
    }
    
    // we want a path, not a cycle
    if ( path.front() == path.back() )
        path.pop_back();
    
    return path;
}

int main()
{
    int u = ReadInput();
    
    if ( u < 0 )
    {
        cout << 0 << endl;
        return 0;
    }
    else
        cout << 1 << endl;
    
    list<int> path = EulerianPath( u );
    
    auto v = path.begin();
    
    cout << *v;
    v++;
    
    while ( v != path.end() )
        cout << " " << *v++;
    
    cout << endl;
    
    return 0;
}

/*
*  The number of vertices and number of edges are read from standard input.
*  The edges are then entered into the adjacency list and the number of
*  edges per vertex is tracked to check if the graph is balanced.  If the
*  graph is balanced, a vertex with maximum outgoing edges is returned.
*  Otherwise -1 is returned.
*/
int ReadInput()
{
    cin >> V >> E;
    ADJ.resize( V + 1 );
    vector<pair<int, int>> deg( V + 1, { 0, 0 } );
    
    for ( int i = 0 ; i < E ; i++ )
    {
        int v, u;
        cin >> u >> v;
        
        ADJ[ u ].push_back( v );
        deg[ u ].first++;
        deg[ v ].second++;
    }
 
    int max_v = 0;
    int max_deg = 0;
    
    for ( int i = 1 ; i <= V ; i++ )
    {
        if ( deg[ i ].first != deg[ i ].second )
            return -1;
        
        if ( deg[ i ].first > max_deg )
        {
            max_v = i;
            max_deg = deg[ i ].first;
        }
    }
    return max_v;
}
