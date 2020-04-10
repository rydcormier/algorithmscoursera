//
//  cleaning_apartment.cpp
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

static bool TESTING = true;

struct Edge {
    int from;
    int to;
};

typedef vector< vector<int>> NonAdjacencyList;

struct ConvertHampathToSat {
    int numVertices;
    //vector<Edge> edges;
    NonAdjacencyList naList;

    ConvertHampathToSat(int n, const vector<Edge>& edges) :
        numVertices(n),
        naList(n + 1)
    {
        vector<vector<bool>> nonadjacent(n + 1, vector<bool>(n + 1, true));
        
        for (int i = 0; i < edges.size(); i++) {
            int u = edges[i].from;
            int v = edges[i].to;
            
            nonadjacent[u][v] = false;
            nonadjacent[v][u] = false;
        }
        
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (i != j && nonadjacent[i][j]) {
                    naList[i].push_back(j);
                }
            }
        }
    }
    
    int convertIndex(int x, int y) {
        // convert a vertex number and position to an index.
        return (((x - 1) * numVertices) + y);
    }

    void printEquisatisfiableSatFormula() {
        // For the SAT formula, create a variable for each vertex for each
        // position in the path: x_i_j | 1 <= i, j <= n.  These variables are
        // subject to the following constraints:
        //      1) Each vertex belongs to a path.
        //      2) Each vertex appears just once in a path.
        //      3) Each position in a path is occupied by some vertex.
        //      4) No two vertices occupy the same position of a path.
        //      5) Two successive vertices on a path must be connected by an
        //          edge.
        
        vector< vector<int>> formula;   // formula in cnf form.
        
        // Add Clauses
        // Constraint 1: (x_v_1 V x_v_2 V ... V x_v_n) | 1 <= v <= n
        for (int v = 1; v <= numVertices; v++) {
            vector<int> clause;
            
            for (int i = 1; i <= numVertices; i++) {
                int idx = convertIndex(v, i);
                clause.push_back(idx);
            }
            formula.push_back(clause);
        }
        
        // Constraint 2: (-x_v_i V -x_v_j) | 1 <= v <= n, 1 <= i < j <= n
        for (int v = 1; v <= numVertices; v++) {
            for (int i = 1; i < numVertices; i++) {
                for (int j = i + 1; j <= numVertices; j++) {
                    int vi_idx = convertIndex(v, i);
                    int vj_idx = convertIndex(v, j);
                    
                    formula.push_back({-vi_idx, -vj_idx});
                }
            }
        }
        
        // Constraint 3: (x_1_j V x_2_j V ... V x_n_j) | 1 <= j <= n
        for (int j = 1; j <= numVertices; j++) {
            vector<int> clause;
            
            for (int i = 1; i <= numVertices; i++) {
                int idx = convertIndex(i, j);
                clause.push_back(idx);
            }
            formula.push_back(clause);
        }
        
        // Constraint 4: (-x_u_i V -x_v_i) | 1 <= i <= n & 1 <= u < v <= n
        for (int i = 0; i <= numVertices; i++) {
            for (int u = 1; u < numVertices; u++) {
                for (int v = u + 1; u <= numVertices; v++) {
                    int iu_idx = convertIndex(i, u);
                    int iv_idx = convertIndex(i, v);
                    formula.push_back({-iu_idx, -iv_idx});
                }
            }
        }
        
        // Constraint 5: (-x_u_k V -x_v_(k+1)) | 1 <= k < n & (u, v) not in E
        for (int k = 1; k < numVertices; k++) {
            for (int u = 1; u <= numVertices; u++) {
                for (int i = 0; i < naList[u].size(); i++) {
                    int v = naList[u][i];
                    
                    int uk_idx = convertIndex(u, k);
                    int vk_1_idx = convertIndex(v, k + 1);
                    
                    formula.push_back({-uk_idx, -vk_1_idx});
                }
            }
        }
        int n = numVertices ^ 2;    // number of variables
        int c = (int) formula.size();     // number of clauses
        
        // print out formula
        if (TESTING) {      // dimacs
            cout << "p " << n << " " << c << endl;
        } else {
            cout << c << " " << n << endl;
        }
        
        for (int i = 0; i < c; i++) {
            vector<int> clause = formula[i];
            cout << (i > 0 ? " " : "");
            
            for (int j = 0; j < clause.size(); j++) {
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
    //ConvertHampathToSat converter(n, m);
    vector<Edge> edges(m);
    for (int i = 0; i < m; ++i) {
        cin >> edges[i].from >> edges[i].to;
    }
    
    ConvertHampathToSat converter(n, edges);

    converter.printEquisatisfiableSatFormula();

    return 0;
}
