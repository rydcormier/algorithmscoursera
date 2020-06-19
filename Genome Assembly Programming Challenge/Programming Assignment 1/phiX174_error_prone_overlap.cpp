//
//  phiX174_error_prone_overlap.cpp
//
//  Created by Ryan Cormier on 06/18/20
//
//  This programs reads 1618 reads of length 100 from standard input and
//  then assembles and returns the circular genome from which the reads
//  came. This program uses an overlap graph that accounts for possible
//  errors in the reads when calculating edge weights. A Hamiltonian
//  path is constructed maximizing overlap.
//
//  Still a work in progress. A naive algorithm is used to find all suffix
//  prefix matches with up to 2 mismatches. A Hamiltonian path is then
//  constructed greedily. A function to construct the genome from the path
//  while fixing any errors still needs to be done.  A more efficient
//  algorithm is also needed for the APSP problem accounting for errors.
//
#include <ctime>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <utility>

#include "helpers.h"

using namespace std;

const int READ_SIZE = 100;
const int NUM_READS = 1618;

/*
//  The set of reads.
*/
vector<string> G( NUM_READS );

/*
//  D[ i ][ j ] is the maximum length of a suffix-prefix match of reads
//  i and j.
*/
vector< vector<int>> D( NUM_READS, vector<int>( NUM_READS ) );

/*
//  ADJ[ i ] stores all adjacencies to the node i in an overlap
//  graph. The adjacenies are represented by the pair { a, b } with 'a'
//  being the overlap subtracted from the read length and b the index of the
//  overlapping read.  Using a set, the adjacencies will be kept in
//  order by decreasing edge weight.
*/
vector< set< pair<int, int>>> ADJ( NUM_READS );

/*
//  We keep track of the global maximum overlap and the corresponding
//  pairs of indices.  We also set a minimum threshold for the adjaceny
//  list.
*/
int MIN_OVERLAP = 20;
int MAX_OVERLAP = 0;
vector< pair< int, int>> MAX_PAIRS;

vector<int> NUM_USED( NUM_READS );

/*
//  Brute force search for the longest suffix-prefix match with at most
//  k mismatches.
*/
int OverlapNaive( const string &a, const string &b, int k )
{
    
    for ( int i = 0 ; i < READ_SIZE ; i++ )
    {
        string s( a.substr( i ) );
        string p( b.substr( 0, READ_SIZE - i ) );
        int errs = 0;
        int pos = 0;
        
        while ( errs <= k && pos < ( READ_SIZE - i ) )
        {
            if ( s[ pos ] != p[ pos ] )
                errs++;
            pos++;
            
        }
        
        if ( errs <= k )
            return ( READ_SIZE - i );
    }

    return 0;
}

/*
//  Brute force with no mismatches.
*/
int overlap ( const string &a, const string &b )
{
    int n = ( int ) min( a.length(), b.length() );
    
    for ( int i = 0 ; i < n ; i++ )
    {
        size_t pos = ( a.length() == n ? i : i + ( a.length() - n ) );
        if ( a.substr( pos ) == b.substr( 0, n - i ) )
            return ( n - i );
    }
    return 0;
}

/*
//  Solves the All-Pairs Suffix-Prefix problem in a naive and inefficient
//  manner.
*/
void SolveAPSPNaive()
{
    for ( int i = 0 ; i < NUM_READS ; i++ )
    {
        for ( int j = 0 ; j < NUM_READS ; j++ )
        {
            if ( i != j )
            {
                int d = OverlapNaive( G[ i ], G[ j ], 2);
                
                D[ i ][ j ] = d;
                
                if ( d == MAX_OVERLAP )
                    MAX_PAIRS.push_back( { i , j } );
                else if ( d > MAX_OVERLAP )
                {
                    MAX_PAIRS = { { i, j } };
                    MAX_OVERLAP = d;
                }
                
                if ( d >= MIN_OVERLAP )
                    ADJ[ i ].insert( { READ_SIZE - d, j } );
            }
        }
    }
}

/*
//  Constructs a Hamiltonian path on the overlap graph in a gready fashion:
//  starting with an edge from MAX_PAIRS, select the next edge with maximum
//  weight.
*/
vector<int> HamPath()
{
    vector<int> res;
    int len = 0;
    for ( auto p = MAX_PAIRS.begin() ; p != MAX_PAIRS.end() ; p++ )
    {
        vector<int> path;
        int u = p->first;
        int v = p->second;
        res.push_back( u );
        res.push_back( v );
        int l = ( 2 * READ_SIZE ) - D[ u ][ v ];
        vector<bool> used( NUM_READS );
        used[ u ] = 1;
        used[ v ] = 1;
        int count = 2;
        bool ok = 1;
        while ( count < NUM_READS && ok )
        {
            u = v;
            auto it = ADJ[ u ].begin();
            while ( it != ADJ[ u ].end() && used[ it->second ] )
                it++;
            
            if ( it == ADJ[ u ].end() )
            {
                ok = 0;
                break;
            }
            
            v = it->second;
            res.push_back( v );
            used[ v ] = 1;
            l += it->first;
            count++;
            
        }
        
        if ( ok )
        {
            l -= D[ v ][ p->first ];
            
            
            if ( res.empty() || l < len )
            {
                res = path;
                len = l;
            }
        }
    }
    
    return res;
}

string AssembleFromPath( vector<int>& path )
{
    string s;
    
    // TODO : Look ahead in the path up to any overlapping reads and
    //  correct any errors;
    
    return s;
}

void load_vals();

int main()
{
    ReadLines( "/Users/ryan/Desktop/reads.txt", G );
    ReadTable( "/Users/ryan/Desktop/apsp_err_prone.txt", D );
    load_vals();
    
    
    vector<string> s( G );
    vector< vector<int>> d( D );
    
    
    vector<int> path = HamPath();
    string genome = AssembleFromPath( path );
    
    cout << genome << endl;;

    
    return 0;
}

void load_vals()
{
    for ( int i = 0 ; i < NUM_READS ; i++ )
    {
        for ( int j = 0 ; j < NUM_READS ; j++ )
        {
            if ( D[ i ][ j ] == MAX_OVERLAP )
                MAX_PAIRS.push_back( { i, j } );
            else if ( D[ i ][ j ] > MAX_OVERLAP )
                MAX_PAIRS = { { i, j } };
            if ( D[ i ][ j ] >= MIN_OVERLAP )
                ADJ[ i ].insert( { READ_SIZE - D[ i ][ j ], j } );
        }
    }
}
