//
//  evacuation.cpp
//  Advanced Algorithms and Complexity - Programming Assignment 1
//
//  Created by Ryan Cormier on 6/2/19.
//

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using std::queue;
using std::vector;

/*
 * This class implements a bit unusual scheme for storing edges of the graph,
 * in order to retrieve the backward edge for a given edge quickly.
 */
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
    /* List of all - forward and backward - edges */
    vector<Edge> edges;
    
    /* These adjacency lists store only indices of edges in the edges list */
    vector<vector<size_t> > graph;
    
public:
    explicit FlowGraph(size_t n): graph(n) {}
    
    void add_edge(int from, int to, int capacity)
    {
        /*
         * Note that we first append a forward edge and then a backward edge,
         * so all forward edges are stored at even indices (starting from 0),
         * whereas backward edges are stored at odd indices in the list edges
         */
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
        /*
         * To get a backward edge for a true forward edge (i.e id is even), we 
         * should get id + 1 due to the described above scheme. On the other 
         * hand, when we have to get a "backward" edge for a backward edge 
         * (i.e. get a forward edge for backward - id is odd), id - 1 should
         * be taken.
         *
         * It turns out that id ^ 1 works for both cases. Think this through!
         */
        
        int b_id = ( ( id % 2 ) == 0  ? id + 1 : id - 1 );
        edges[id].flow += flow;
        edges[b_id].flow -= flow;
    }
};

FlowGraph read_data()
{
    int vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    
    FlowGraph graph(vertex_count);
    
    for (int i = 0; i < edge_count; ++i)
    {
        int u, v, capacity;
        std::cin >> u >> v >> capacity;
        graph.add_edge(u - 1, v - 1, capacity);
    }
    
    return graph;
}

vector<size_t> find_shortest_path(const FlowGraph& graph, int from, int to)
{
    /* Return a list of vertex ids for the shortest path of from to to. */
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


int max_flow(FlowGraph& graph, int from, int to)
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

int main() {
    std::ios_base::sync_with_stdio(false);
    FlowGraph graph = read_data();
    
    std::cout << max_flow(graph, 0, (int) graph.size() - 1) << "\n";
    return 0;
}
