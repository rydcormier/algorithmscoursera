//
//  puzzle.cpp
//
//  Created by Ryan Cormier on 6/19/20.
//
//  Given 25 "puzzle pieces", this program returns a 5 x 5 grid of the
//  pieces such that any two neighboring pieces share the same color
//  on the common side.
//
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const int NUM_PIECES = 25;

enum Sides { Top, Left, Bottom, Right };

//  The puzzle piece.
struct Piece
{
    string top;
    string left;
    string bottom;
    string right;
    
    void intit( string& s );
    string str() const { return string( "(" + top + "," + left + "," + bottom +
                                        "," + right + ")" ); }
};

// the puzzle
vector<Piece> Pieces( NUM_PIECES );

//
//  We have an adjaceny list for each side of a piece. For example
//  if j is an element of ADJ[ i ][ Top ], then piece j's bottom color
//  is the same as i's top.
//
vector< vector< vector<int>>> ADJ( NUM_PIECES, vector< vector<int>>( 4 ));

//
//  We use a DFS to find a Hamiltonian path. The piece we start from is placed
//  in the top left corner and we keep adding unused adjacent pieces from left
//  to right and top to bottom. When there are no more valid pieces to add,
//  we either have used all the pieces and we're done, or we reached a dead end
//  and must backtrack. If no solution exists starting from the original piece,
//  the path will be empty.
//
void DFS( int u, vector<int>& path, vector<bool>& used )
{
    // We know the current position by the size of the path.
    int pos = ( int ) path.size();
    
    // Verify that this is a valid position for the current piece.
    if ( pos > 4 )
        if ( Pieces[ u ].top != Pieces[ path[ pos - 5 ] ].bottom )
            return;
    
    path.push_back( u );
    used[ u ] = 1;
    
    if ( path.size() == NUM_PIECES )    // we're done
        return;
    
    // If we are on the right edge, we move to the next row on the far left.
    // The adjacencies we need are from the bottom of the piece at the
    // far left of this row.  Otherwise we just move right.
    int p, s;
    if ( ( ( pos + 1 ) % 5 ) == 0 )
    {
        p = path[ pos - 4 ];
        s = Bottom;
    }
    else
    {
        p = u;
        s = Right;
    }
    vector<int> adj( ADJ[ p ][ s ] );
    
    // Try all possible pieces until we have a hamiltonian path
    for ( int i = 0 ; i < adj.size() ; i++ )
    {
        if ( ! used[ adj[ i ] ] )
            DFS( adj[ i ], path, used );
        
        if ( path.size() == NUM_PIECES )
            return;
    }
    
    // Dead end => move back
    path.pop_back();
    used[ u ] = 0;
}

// Format the solution.
string AssemblePath( vector<int>& path )
{
    string res;
    int i = 0;
    int c = 0;
    
    while ( i < path.size() )
    {
        if ( c == 5 )
        {
            res.push_back( '\n' );
            c = 0;
        }
        else if ( c!= 0 )
            res.push_back( ';' );
        
        res += Pieces[ path[ i ] ].str();
        i++;
        c++;
    }
    
    return res;
}

int main()
{
    for ( int i = 0 ; i < NUM_PIECES ; i++ )
    {
        string line;
        cin >> line;
        Pieces[ i ].intit( line );
    }
    
    // First we add edges to the graphs representing matching colors on
    // opposite sides of the pieces.
    for ( int i = 0 ; i < NUM_PIECES ; i++ )
    {
        for ( int j = 0 ; j < NUM_PIECES ; j++ )
        {
            if ( i == j )
                continue;
            if ( Pieces[ i ].bottom == Pieces[ j ].top )
                ADJ[ i ][ Bottom ].push_back( j );
            if ( Pieces[ i ].right == Pieces[ j ].left )
                ADJ[ i ][ Right ].push_back( j );
        }
    }
    
    vector<int> path;
        
    // Use a depth first search to find a hamiltonian path.
    for ( int i = 0 ; i < NUM_PIECES ; i++ )
    {
        path.clear();
        vector<bool> used( NUM_PIECES );
        DFS( i, path, used );
        
        if ( path.size() == NUM_PIECES )
            break;
    }
    
    cout << AssemblePath( path ) << endl;
    
    return 0;
}

void Piece::intit( string& s )
{
    s.erase( s.begin() );
    s.erase( s.end() - 1 );
    stringstream ss( s );
    int side = 0;
    string color;
    
    while ( getline( ss, color, ',' ) )
    {
        switch ( side )
        {
            case Top:
                top = color;
                break;
            case Left:
                left = color;
                break;
            case Bottom:
                bottom = color;
                break;
            case Right:
                right = color;
            
            default:
                break;
        }
        side++;
    }
}

string Piece::str() const
{
    return string( "(" + top + "," + left + "," + bottom + "," + right + ")" );
}
