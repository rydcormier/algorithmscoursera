//
//  stock_charts.cpp
//  Advanced Algorithms and Complexity - Programming Assignment 1
//
//  Created by Ryan Cormier on 6/9/19.
//

#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>
#include <queue>

using std::vector;
using std::cin;
using std::cout;
using std::queue;

 // A class to store the edges of a graph and thier reverse for a corresponding
 // residual graph.
class FlowGraph
{
public:
    
    struct Edge
    {
        int from, to, capacity, flow;
        
        Edge(int from_, int to_, int capacity_, int flow_) :
        from(from_), to(to_), capacity(capacity_), flow(flow_) {}
        
        Edge(const Edge& e) :
        from(e.from), to(e.to), capacity(e.capacity), flow(e.flow) {}
    };
    
private:
    
    // a list of edges and their reverse.
    vector<Edge> edges;
    
    // An adjacency list storing the outging edge indices for a node.
    vector<vector<size_t> > graph;
    
public:
    
    explicit FlowGraph(size_t n): graph(n) {}
    
    void add_edge(int from, int to, int capacity)
    {
        // Append an edge so that it is stored at an even index and then
        // append the corresponding reverse edge which sill then have an
        // odd index.
        Edge forward_edge = {from, to, capacity, 0};
        Edge backward_edge = {to, from, 0, 0};
        
        graph[from].push_back(edges.size());
        edges.push_back(forward_edge);
        
        graph[to].push_back(edges.size());
        edges.push_back(backward_edge);
    }
    
    size_t size() const
    {
        return graph.size();
    }
    
    const vector<size_t>& get_ids(int from) const
    {
        return graph[from];
    }
    
    const Edge& get_edge(size_t id) const
    {
        return edges[id];
    }
    
    void add_flow(size_t id, int flow)
    {
        // To get a backward edge for a true forward edge (i.e id is even), we
        // should get id + 1 due to the described above scheme. On the other
        // hand, when we have to get a "backward" edge for a backward edge
        // (i.e. get a forward edge for backward - id is odd), id - 1 should
        // be taken.
        //
        // It turns out that id ^ 1 works for both cases. Think this through!
         
        edges[id].flow += flow;
        edges[id ^ 1].flow -= flow;
    }
};

vector<size_t> find_shortest_path(const FlowGraph& graph, int from, int to)
{
    // Return a list of vertex ids for the shortest path of from to to.
    vector<size_t> path;
    vector<bool> found(graph.size(), false);
    vector<size_t> prev(graph.size());
    queue<int> q;
    
    // start at source and use BFS
    int current_node = from;
    found[current_node] = true;
    q.push(current_node);
    
    while (!q.empty() && !found[to])
    {
        current_node = q.front(); q.pop();
        
        vector<size_t> edge_ids = graph.get_ids(current_node);
        
        // check each edge from current node;
        for (vector<size_t>::iterator id = edge_ids.begin();
             id != edge_ids.end(); ++id)
        {
            
            FlowGraph::Edge e = graph.get_edge(*id);
            
            if (!found[e.to] && e.flow < e.capacity)
            {
                // add end node
                q.push(e.to);
                found[e.to] = true;
                prev[e.to] = *id;
                
                if (e.to == to)
                    break;
            }
        }
    }
    
    if (found[to])  // a path exists
    {
        size_t node = to;
        
        while (node != from)
        {
            path.push_back(prev[node]);
            node = graph.get_edge(prev[node]).from;
        }
        
        std::reverse(path.begin(), path.end());
    }
    
    return path;
}

int maxFlow(FlowGraph& graph, int from, int to)
{
    int flow = 0;
    
    vector<size_t> path = find_shortest_path(graph, from, to);
    
    while (!path.empty())
    {
        int min_capacity = 10000; // the maximum allowable capacity
        
        for (size_t i = 0; i < path.size(); ++i)
        {
            FlowGraph::Edge e = graph.get_edge(path[i]);
            min_capacity = std::min(min_capacity, e.capacity - e.flow);
        }
        
        for (vector<size_t>::iterator it = path.begin(); it != path.end(); ++it)
        {
            graph.add_flow(*it, min_capacity);
        }
        
        flow += min_capacity;
        
        path = find_shortest_path(graph, from, to);
    }
    
    
    return flow;
}

class StockCharts
{
public:
    void Solve()
    {
        vector<vector<int>> stock_data = ReadData();
        int result = MinCharts(stock_data);
        WriteResponse(result);
    }
    
private:
    vector<vector<int>> ReadData()
    {
        int num_stocks, num_points;
        cin >> num_stocks >> num_points;
        
        vector<vector<int>> stock_data(num_stocks, vector<int>(num_points));
        
        for (int i = 0; i < num_stocks; ++i)
        {
            for (int j = 0; j < num_points; ++j)
            {
                cin >> stock_data[i][j];
            }
        }
        return stock_data;
    }
    
    void WriteResponse(int result)
    {
        cout << result << "\n";
    }
    
    int MinChartsGreedy(const vector<vector<int>>& stock_data) {
        // Vector of charts; each chart is a vector of indices of individual stocks.
        vector<vector<int>> charts;
        for (int i = 0; i < stock_data.size(); ++i)
        {
            bool added = false;
            for (auto& chart : charts)
            {
                bool can_add = true;
                for (int index : chart)
                {
                    if (!compare(stock_data[i], stock_data[index]) &&
                        !compare(stock_data[index], stock_data[i]))
                    {
                        can_add = false;
                        break;
                    }
                }
                
                if (can_add)
                {
                    chart.push_back(i);
                    added = true;
                    break;
                }
            }
            
            if (!added)
            {
                charts.emplace_back(vector<int>{i});
            }
        }
        
        return (int) charts.size();
    }
    
    int MinCharts(const vector<vector<int>>& stock_data)
    {
        // Find the maximum flow on a bipartite network with a stock
        // having a node on each side. A matching on the graph represents
        // the combining of stocks in a chartand decreasing the number
        // of charts needed by one. By giving each edge a capacity of 1,
        // the minimum number of charts would be the total number of
        // stocks minus the maximum flow on the corresponding network.
        int n = (int) stock_data.size();
        int source = 0;
        int sink = 2 * n + 1;
        
        FlowGraph graph = getNetwork(stock_data);
        return (n - maxFlow(graph, source, sink));
    }
    
    bool compare(const vector<int>& stock1, const vector<int>& stock2)
    {
        // Returns true if stock1 is strictly less than stock2 - meaning each
        // price of stock1 is less than corresponding price of stock2.
        for (int i = 0; i < stock1.size(); ++i)
            if (stock1[i] >= stock2[i])
                return false;
        return true;
    }
    
    vector<vector<bool>> compareAllStocks(const vector<vector<int>>& stock_data)
    {
        // Return an n x n boolean matrix with a true value at (i, j) meaning
        // stock i is less than stock j.
        size_t n = stock_data.size();
        vector<vector<bool>> res(n, vector<bool>(n, false));
        vector<vector<bool>> compared(n, vector<bool>(n, false));
        
        for (size_t i = 0; i < n; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                if (compared[i][j] || i == j)
                    continue;
                
                compared[i][j] = true;
                
                if (compare(stock_data[i], stock_data[j]))
                {
                    res[i][j] = true;
                    res[j][i] = false;
                    compared[j][i] = true;
                }
            }
        }
        
        return res;
    }
    
    FlowGraph getNetwork(const vector<vector<int>>& stock_data)
    {
        // Creates a bipartite graph. Each stock has a node in each partition.
        // If a stock is less than another, there will be an edge between the 2
        // directed from the smaller stock on the left hand side to the larger
        // stock on the right hand side.
        FlowGraph graph(2 * stock_data.size() + 2);
        vector<vector<bool>> comparisons = compareAllStocks(stock_data);
        
        int n = (int) stock_data.size();
        int source = 0;
        int sink = (int) graph.size() - 1;
        
        // connect source to nodes 1 through n - one for each stock
        for (int i = 1; i <= n; ++i)
            graph.add_edge(source, i, 1);
        
        // Use the comparisons matrix to connect the stock nodes.
        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < n; ++j)
            {
                if (comparisons[i][j])
                {
                    graph.add_edge(i + 1, j + 1 + n, 1);
                }
            }
        }
        
        // connect nodes n + 1 to 2 * n to the sink
        for (int i = n; i < 2 * n; ++i)
        {
            graph.add_edge(i + 1, sink, 1);
        }
        
        
        return graph;
    }
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    StockCharts stock_charts;
    stock_charts.Solve();
    return 0;
}

