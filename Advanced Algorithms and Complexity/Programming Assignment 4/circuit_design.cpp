//  circuit_design.cpp
//
//  Advanced Algorithms and Complexity
//  Programming Assignment 4: Problem 1
//
//  Created by Ryan Cormier on 4/28/20.
//

#include <algorithm>
#include <iostream>
#include <stack>
#include <vector>

using namespace std;

typedef vector< vector<int>> List;
typedef pair<int, int> Clause;

// Directed Graph
struct Graph {
    int V;
    List adj;
    List adjInv;
    
    Graph(int n) : V(n), adj(n + 1), adjInv(n + 1) {}
    void add_edge(int, int);
    vector<int> topologicalSort();
};

// Strongly connected components
struct SCC {
    vector<int> vertexMap;
    List components;
    
    SCC(int n) : vertexMap(n + 1) {}
    void add_vertex(int, int);
    void generate(Graph &g);
};

// Depth first search
struct DFS {
    vector<int> post(Graph&);
    vector<int> findSCC(Graph&);
    
    void postVisit(Graph&, int, vector<bool>&, vector<int>&);
    void sccVisit(Graph&, int, vector<bool>&, vector<int>&, int);
};

struct TwoCNF {
    int numVars;
    vector<Clause> clauses;
    
    TwoCNF(int n, vector<Clause> c) : numVars(n), clauses(c) {}
};

struct TwoSAT {
    int N;
    TwoCNF formula;
    Graph g;
    
    int negate(int);
    TwoSAT(int n, vector<Clause> clauses) : N(n),
                                            formula(n, clauses),
                                            g(2 * n)
    {}
    bool isSatisfiable(vector<int> &result);
};

int main() {
    int n, m;

    cin >> n >> m;

    vector<Clause> clauses(m);
    for (int i = 0; i < m; i++) {
        cin >> clauses[i].first >> clauses[i].second;
    }
    
    TwoSAT twoSAT(n, clauses);
    vector<int> result(n + 1);
    
    if (twoSAT.isSatisfiable(result)) {
        cout << "SATISFIABLE" << endl;
        
        for (int i = 1; i <= n; i++)
            cout << (i > 1 ? " " : "") << (result[i] ? i : -i);
        
        cout << endl;
    } else {
        cout << "UNSATISFIABLE" << endl;
    }
    
    return 0;
}

bool TwoSAT::isSatisfiable(vector<int> &result) {
    
    // initialize implication graph
    for (int i = 0; i < formula.clauses.size(); i++) {
        Clause c = formula.clauses[i];
        
        // map [-n, -1] -> [n + 1, 2n];
        int x = (c.first > 0 ? c.first : abs(c.first) + formula.numVars);
        int y = (c.second > 0 ? c.second : abs(c.second) + formula.numVars);
        
        // add edges -x -> y and -y -> x
        g.add_edge(negate(x), y);
        g.add_edge(negate(y), x);
    }
    
    // find the strongly connected components
    SCC scc(g.V);
    scc.generate(g);
    
    // make sure x and -x aren't in the same component
    for (int x = 1; x <= N; x++) {
        if (scc.vertexMap[x] == scc.vertexMap[negate(x)])
            return false;
    }
    
    // generate a meta graph
    Graph mg((int) scc.components.size() - 1);
    for (int i = 1; i <= g.V; i++) {
        for (int j = 0; j < g.adj[i].size(); j++) {
            int u = scc.vertexMap[i];
            int v = scc.vertexMap[g.adj[i][j]];
            
            if (u != v)
                mg.add_edge(u, v);
        }
    }
    
    // find reverse topological order of mg
    vector<int> order = mg.topologicalSort();
    reverse(order.begin(), order.end());
    vector<bool> isSet(g.V + 1, false);
    result.resize(g.V + 1);
    
    for (int i = 0; i < order.size(); i++) {
        vector<int> component = scc.components[order[i]];
        
        for (int j = 0; j < component.size(); j++) {
            int u = component[j];
            int not_u = negate(u);
            
            if (! isSet[u]) {
                result[u] = 1;
                result[not_u] = 0;
                isSet[u] = 1;
                isSet[not_u] = 1;
                
            }
        }
    }
    
    return true;
}

int TwoSAT::negate(int n) {
    return (n <= formula.numVars ? n + formula.numVars : n - formula.numVars);
}

void Graph::add_edge(int u, int v) {
    // make sure the edge does not allready exist;
    for (int i = 0; i < adj[u].size(); ++i) {
        if (adj[u][i] == v)
            return;
    }
    
    adj[u].push_back(v);
    adjInv[v].push_back(u);
}

vector<int> Graph::topologicalSort() {
    // find reverse post order
    DFS dfs;
    
    vector<int> res = dfs.post(*this);
    
    reverse(res.begin(), res.end());
    
    return res;
}

void SCC::add_vertex(int v, int cc) {
    vertexMap[v] = cc;
    
    while (components.size() <= cc) {
        components.emplace_back();
    }
    
    components[cc].push_back(v);
}

void SCC::generate(Graph &g) {
    // first find topological order for g
    vector<int> order = g.topologicalSort();
    
    // run dfs on reverse graph -> dfs automatically uses the reverse
    DFS dfs;
    vector<int> comps = dfs.findSCC(g);
    
    for (int v = 1; v < comps.size(); v++) {
        int cc = comps[v];
        
        add_vertex(v, cc);
    }
}

// Post order of g
vector<int> DFS::post(Graph &g) {
    vector<int> res;
    vector<bool> visited(g.V + 1, false);
    
    for (int u = 1; u <= g.V; u++) {
        if (! visited[u])
            postVisit(g, u, visited, res);
    }
    
    return res;
}

void DFS::postVisit(Graph &g, int u, vector<bool> &visited, vector<int> &order) {
    visited[u] = 1;
    
    for (int i = 0; i < g.adj[u].size(); i++) {
        int v = g.adj[u][i];
        
        if (! visited[v])
            postVisit(g, v, visited, order);
    }
    order.push_back(u);
}

// maps the vertices of g to its SCC
vector<int> DFS::findSCC(Graph &g) {
    vector<int> res(g.V + 1);
    
    vector<int> order = post(g);
    reverse(order.begin(), order.end());
    
    vector<bool> visited(g.V + 1, false);
    int cc = 0;
    
    for (int i = 0; i < order.size(); i++) {
        int u = order[i];
        
        if (! visited[u]) {
            cc++;
            sccVisit(g, u, visited, res, cc);
        }
    }
    
    return res;
}

void DFS::sccVisit(Graph &g, int u, vector<bool> &visited,
                   vector<int> &scc, int cc) {
    visited[u] = 1;
    scc[u] = cc;
    
    for (int i = 0; i < g.adjInv[u].size(); i++) {
        int v = g.adjInv[u][i];
        
        if (! visited[v])
            sccVisit(g, v, visited, scc, cc);
    }
}



