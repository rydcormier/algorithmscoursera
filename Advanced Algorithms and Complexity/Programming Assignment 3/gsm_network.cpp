//
//  gsm_network.cpp
//
//  Advanced Algorithms and Complexity: NP-Complete Problems
//  Programming Assignment 3
//
//  Created by Ryan Cormier on 4/8/2020
//

#include <ios>
#include <iostream>
#include <vector>

using namespace std;

struct Edge {
    int from;
    int to;
};

/*
 *
 */
struct ConvertGSMNetworkProblemToSat {
    // 3-COLOR -> SAT reduction
    int numVertices;
    vector<Edge> edges;

    ConvertGSMNetworkProblemToSat(int n, int m) :
        numVertices(n),
        edges(m)
    {  }
    
    // Convert the tower number i and frequency j to a unique index.
    int get_index(int v, int k) {
        return (k + (3 * v) - 3);
    }


    // Convert towers and frequencies into literals and create clauses. Print
    // results to STDOUT.
    void printEquisatisfiableSatFormula() {
        vector< vector<int>> cnf; // Formula in CNF form
        
        //  Literals:
        //      for each tower i and each frequency j create variable x_i_j.
        //      Then 1 <= i <= n and 1 <= j <= 3. Use a flat container and
        //      convert (i, j) to the integer i + 3j - 4.
        //
        //  Clauses:
        //  1) (x_i_1 V x_i_2 V x_i_3) for 1 <= 1 <= n
        //      Every tower is assigned a frequency
        for (int i = 1; i <= numVertices; i++) {
            vector<int> clause;
            for (int j = 1; j <= 3; j++) {
                int k = get_index(i, j);  // flat index
                clause.push_back(k);
            }
            cnf.push_back(clause);
        }
        
        //  2) (-x_u_i V -x_u_j) for 1 <= u <= n and 1 <= i < j <= 3
        //      Every tower is assigned exactly one frequency
        for (int u = 1; u <= numVertices; u++) {
            
            for (int i = 1; i < 3; i++) {
                
                for (int j = 2; j <= 3; j++) {
                                        
                    if (i < j) {
                        int i_idx = get_index(u, i);
                        int j_idx = get_index(u, j);
                        cnf.push_back({-i_idx, -j_idx});
                    }
                }
            }
        }
        
        //  3) (-x_u_i V -x_w_i) for each edge (u, w) and 1 <= i <= 3
        //      No neighboring towers share a frequencies
        for (int k = 0; k < edges.size(); k++) {
            int u = edges[k].from;
            int w = edges[k].to;

            for (int i = 1; i <= 3; i++) {
                int u_idx = get_index(u, i);
                int w_idx = get_index(w, i);
                cnf.push_back({-u_idx, -w_idx});
            }
        }
        
        int numClauses = (4 * numVertices) + (3 * (int) edges.size());
        int numVars = 3 * numVertices;
        
        cout << numClauses << " " << numVars << endl;
        
        for (int i = 0; i < numClauses; i++) {
            vector<int> clause = cnf[i];
            
            for (int j = 0; j < clause.size(); j++) {
                cout << ((j > 0) ? " " : "");
                cout << clause[j];
            }
            
            cout << " 0" << endl;
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    int n, m;
    cin >> n >> m;
    ConvertGSMNetworkProblemToSat converter(n, m);
    for (int i = 0; i < m; ++i) {
        cin >> converter.edges[i].from >> converter.edges[i].to;
    }

    converter.printEquisatisfiableSatFormula();

    return 0;
}
