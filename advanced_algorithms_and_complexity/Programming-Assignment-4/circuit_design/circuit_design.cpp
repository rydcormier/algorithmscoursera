//
//  main.cpp
//  Assignment4
//
//  Created by Ryan Cormier on 4/21/20.
//  Copyright © 2020 Ryan Cormier. All rights reserved.
//

#include <iostream>
#include<stack>
#include <vector>

using namespace std;

struct Clause {
    int firstVar;
    int secondVar;
};

struct Graph {
    int numVertices;
    int numEdges;
    vector< vector<int>> adj;
    vector< vector<int>> adjInv;
    
    Graph(int n, int m=0) : numVertices(n),
    numEdges(m),
    adj(vector<vector<int>>(n + 1)),
    adjInv(vector<vector<int>>(n + 1))
    {}
    
    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adjInv[v].push_back(u);
        numEdges++;
    }
};

// recursive call for firstDFS
void firstVisit(Graph &g, int u, stack<int> &s, vector<bool> &visited) {
    visited[u] = true;
    
    for (int i = 0; i < g.adj[u].size(); i++) {
        int v = g.adj[u][i];
        if (!visited[v])
            firstVisit(g, v, s, visited);
    }
    
    s.push(u);
}

// basic DFS and returns a reverse post ordering
stack<int> firstDFS(Graph &g) {
    stack<int> s;
    vector<bool> visited(g.numVertices + 1, false);
    
    for (int u = 1; u <= g.numVertices; u++) {
        if (!visited[u])
            firstVisit(g, u, s, visited);
    }
    
    return s;
}

struct SCC {
    vector<int> compNumber;
    vector< vector<int>> comps;
    
    SCC(int n) : compNumber(n + 1), comps() {}
    
    void addToComponent(int u, int cc) {
        while (cc >= comps.size())
            comps.push_back(vector<int>());
        compNumber[u] = cc;
        comps[cc].push_back(u);
    }
};

void secondVisit(Graph &g, int u, vector<bool> &visited, SCC &scc, int cc) {
    visited[u] = true;
    scc.addToComponent(u, cc);
    
    for (vector<int>::iterator v = g.adjInv[u].begin(); v != g.adjInv[u].end(); v++) {
        if (!visited[*v])
            secondVisit(g, *v, visited, scc, cc);
    }
    
}

SCC secondDFS(Graph &g, stack<int> &s) {
    SCC scc(g.numVertices);
    vector<bool> visited(g.numVertices + 1, false);
    int cc = 0;
    
    while (!s.empty()) {
        
        int u = s.top();
        s.pop();
        
        if (!visited[u]) {
            cc++;
            secondVisit(g, u, visited, scc, cc);
        }
    }
    
    return scc;
}

Graph metaGraph(Graph &g, const SCC &scc) {
    Graph res((int) scc.comps.size() - 1);
    
    // check each edge (u, v) and if they are in different SCC's add the edge.
    for (int u = 1; u <= g.numVertices; u++) {
        for (vector<int>::iterator v = g.adj[u].begin(); v != g.adj[u].end(); v++) {
            int uC = scc.compNumber[u];
            int vC = scc.compNumber[*v];
            
            if (uC != vC)
                res.add_edge(uC, vC);
        }
    }
    
    return res;
}

void thirdVisit(Graph &g, int u, vector<bool> &visited, vector<int> &order) {
    visited[u] = true;
    
    for (vector<int>::iterator v = g.adj[u].begin(); v != g.adj[u].end(); v++) {
        if (!visited[*v])
            thirdVisit(g, *v, visited, order);
    }
    
    order.push_back(u);
    
}

vector<int> thirdDFS(Graph &g) {
    
    vector<int> order;
    vector<bool> visited(g.numVertices + 1, false);
    for (int u = 1; u <= g.numVertices; u++) {
        
        if (!visited[u])
            thirdVisit(g, u, visited, order);
    }
    
    return order;
}

struct TwoSatisfiability {
    int numVars;
    vector<Clause> clauses;
    
    TwoSatisfiability(int n, int m) :   numVars(n),
    clauses(m)
    {}
    
    int negationIndex(int i) {
        return (i <= numVars ? i + numVars : i - numVars);
    }
    
    Graph implicationGraph() {
        Graph ig(numVars * 2);
        
        for (int i = 0; i < clauses.size(); i++) {
            Clause clause = clauses[i];
            
            // [-n, -1] -> [n + 1, 2n]
            int u = (clause.firstVar > 0 ?
                     clause.firstVar : abs(clause.firstVar) + numVars);
            int v = (clause.secondVar > 0 ?
                     clause.secondVar : abs(clause.secondVar) + numVars);
            
            // first edge : -u -> v
            ig.add_edge(negationIndex(u), v);
            
            // second edge : -v -> u
            ig.add_edge(negationIndex(v), u);
        }
        
        return ig;
    }
    
    SCC stronglyConnectedComponents(Graph &g) {
        
        // run firstDFS to get reverse post order
        stack<int> rpo = firstDFS(g);
        
        // run secondDFS to find components
        return secondDFS(g, rpo);
    }
    
    bool isSatisfiableNaive(vector<int>& result) {
        // This solution tries all possible 2^n variable assignments.
        // It is too slow to pass the problem.
        // Implement a more efficient algorithm here.
        for (int mask = 0; mask < (1 << numVars); ++mask) {
            for (int i = 0; i < numVars; ++i) {
                result[i] = (mask >> i) & 1;
            }
            
            bool formulaIsSatisfied = true;
            
            for (const Clause& clause: clauses) {
                bool clauseIsSatisfied = false;
                if (result[abs(clause.firstVar) - 1] == (clause.firstVar < 0)) {
                    clauseIsSatisfied = true;
                }
                if (result[abs(clause.secondVar) - 1] == (clause.secondVar < 0)) {
                    clauseIsSatisfied = true;
                }
                if (!clauseIsSatisfied) {
                    formulaIsSatisfied = false;
                    break;
                }
            }
            
            if (formulaIsSatisfied) {
                return true;
            }
        }
        return false;
    }
    
    bool isSatisfiable(vector<int>& result) {
        Graph g = implicationGraph();
        SCC scc = stronglyConnectedComponents(g);
        
        // make sure x and -x aren't in the same scc
        for (int u = 1; u <= numVars; u++) {
            if (scc.compNumber[u] == scc.compNumber[negationIndex(u)])
                return false;
        }
        
        Graph mg = metaGraph(g, scc);
        
        vector<int> order = thirdDFS(mg);
        
        vector<bool> isSet(g.numVertices + 1, false);
        vector<bool> binVector(g.numVertices + 1);
        for (int i = 0; i < order.size(); i++) {
            vector<int> comp = scc.comps[order[i]];
            
            for (vector<int>::iterator u = comp.begin(); u != comp.end(); u++) {
                
                if (!isSet[*u]) {
                    binVector[*u] = 1;
                    isSet[*u] = true;
                    
                    binVector[negationIndex(*u)] = 0;
                    isSet[negationIndex(*u)] = true;
                }
            }
        }
        
        for (int i = 1; i <= numVars; i++) {
            
            result[i] = (binVector[i] ? i : -i);
        }
        
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    
    int n, m;
    cin >> n >> m;
    TwoSatisfiability twoSat(n, m);
    for (int i = 0; i < m; ++i) {
        cin >> twoSat.clauses[i].firstVar >> twoSat.clauses[i].secondVar;
    }
    
    vector<int> result(n + 1);
    if (twoSat.isSatisfiable(result)) {
        cout << "SATISFIABLE" << endl;
        for (int i = 1; i <= n; ++i) {
            cout << (i > 1 ? " " : "") << result[i];
        }
        cout << endl;
    } else {
        cout << "UNSATISFIABLE" << endl;
    }
    
    return 0;
}


