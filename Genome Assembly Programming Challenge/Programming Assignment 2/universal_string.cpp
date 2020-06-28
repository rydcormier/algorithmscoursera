//
//  universal_string.cpp
//
//  Created by Ryan Cormier on 6/19/20.
//
//  Given a an integer k, his program finds and returns a
//  k-universal circular binary string.
//
#include <bitset>
#include <cmath>
#include <iostream>
#include <list>
#include <string>
#include <vector>

using namespace std;

size_t to_ulong( const string& s );
string to_bin_string( size_t v, size_t len );

list<size_t> EulerianPath( vector< list<size_t>>& adj );

int E = 0;

int main()
{
    size_t k;
    cin >> k;
    
    vector< list<size_t>> adj( pow( 2, k - 1 ) );
    
    for ( size_t i = 0 ; i < pow( 2, k ) ; i++ )
    {
        string s = to_bin_string( i, k );
        size_t u = to_ulong( s.substr( 0, k - 1 ) );
        size_t v = to_ulong( s.substr( 1 ) );
        adj[ u ].push_back( v );
        E++;
    }
    
    list<size_t> path = EulerianPath( adj );
    
    auto it = path.begin();
    cout << to_bin_string( *it, k - 1 );
    it++;
    while ( it != path.end() )
    {
        cout << to_bin_string( *it, k - 1 ).back();
        it++;
    }
    cout << endl;
    return 0;
}

list<size_t> EulerianPath( vector< list<size_t>>& adj )
{
    list<size_t> path;
    size_t u = 0;
    
    path.push_back( u );
    
    while ( true )
    {
        while ( !adj[ u ].empty() )
        {
            path.push_back( adj[ u ].back() );
            adj[ u ].pop_back();
            u = path.back();
        }
        
        if ( path.size() >=  E )
            break;
        
        do
        {
            path.pop_back();
            u = path.back();
            path.push_front( u );
        }
        while ( adj[ u ].empty() );
    }
    while ( path.front() == path.back() )
        path.pop_back();
    
    return path;
}

size_t to_ulong( const string& s )
{
    bitset<16> b( s );
    
    return b.to_ulong();
}

string to_bin_string( size_t v, size_t len )
{
    bitset<16> b( v );
    return b.to_string<char, string::traits_type,
                       string::allocator_type>().substr( 16 - len );
}


