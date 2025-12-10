//=================================================================
// CS 271 - Project 6
// Graph.cpp
// Fall 2025
// This is the implementation file for the Graph class
// Joel Singh, Colin Nguyen
//=================================================================

#include <queue>
#include <climits>
#include <stack>
#include <vector>
#include <set>


//=================================================================
// Default constructor
// Creates an empty graph
// Parameters:  none
// Returns:     none
//=================================================================
template <class K, class D>
Graph<K,D>::Graph ( )
{
    numV = 0;
    numE = 0;
}

//=================================================================
// Constructor
// Creates a graph with given vertices and edges
// Parameters:  keys - vector of vertex keys
//              data - vector of vertex data
//              edges - vector of edges as tuples(vertex1, vertex2, weight)
// Returns:     none
//=================================================================
template <class K, class D>
Graph<K,D>::Graph ( vector<K> keys, vector<D> data, vector<tuple<K,K,int>> edges )
{
    numV = 0;
    numE = 0;
    for (int i = 0; i < keys.size(); i++)
        insertVertex(keys[i], data[i]);
    for (int j = 0; j < edges.size(); j++)
    {
        K v1 = get<0>(edges[j]);
        K v2 = get<1>(edges[j]);
        int w = get<2>(edges[j]);
        string label = get<3>(edges[j]);
        insertEdge(v1, v2, w, label);
    }
}

//=================================================================
// Destructor
// Destroys the graph
// Parameters:  none
// Returns:     none
//=================================================================
template <class K, class D>
Graph<K,D>::~Graph ( ) {}

//=================================================================
// isEdge
// Checks if there is an edge between two vertices
// Parameters:  v1 - key of the first vertex
//              v2 - key of the second vertex
// Returns:     true if edge exists in graph, false otherwise
//=================================================================
template <class K, class D>
bool Graph<K,D>::isEdge ( K v1, K v2 ) const{
    if (vertices.find(v1) == vertices.end() || vertices.find(v2) == vertices.end())
        throw invalid_argument("Error in isEdge: One or both vertices not found.");
    // get the adjacency list of v1
    const list<tuple<K, int>>& adj = vertices.at(v1).adj;
    // check if v2 is in the adjacency list
    for (const auto& edge : adj) {
        if (get<0>(edge) == v2)
            return true;
    }
    return false;
 }

//=================================================================
// getWeight
// Returns the weight of the edge between two vertices
// Parameters:  v1 - key of the first vertex
//              v2 - key of the second vertex
// Returns:     weight of the edge if it exists, INT_MAX otherwise
//=================================================================
template <class K, class D>
int Graph<K,D>::getWeight ( K v1, K v2 ) const {
    if (!isEdge(v1, v2))
        return INT_MAX;
    // get the adjacency list of v1
    const list<tuple<K, int>>& adj = vertices[v1].adj;
    for (const auto& edge : adj) {
        if (get<0>(edge) == v2) // first element in tuple is the adjacent vertex key
            return get<1>(edge); // second element is the weight
    }
    return INT_MAX;
}

//=================================================================
// insertEdge
// Inserts an edge between two vertices with a given weight
// Parameters:  v1 - key of the first vertex
//              v2 - key of the second vertex
//              w  - weight of the edge
// Returns:     none
//=================================================================
template <class K, class D>
void Graph<K,D>::insertEdge ( K v1, K v2, int w, string label)
{
    if (vertices.find(v1) == vertices.end() || vertices.find(v2) == vertices.end())
        throw invalid_argument("Error in insertEdge: One or both vertices not found.");
        
    // if edge already exists, update weight
    for (auto& edge : vertices[v1].adj) {
        if (get<0>(edge) == v2) {
            edge = make_tuple(v2, w, label);
            return;
        }
    }
    // otherwise, add new edge to adjacency list
    vertices[v1].adj.push_back(make_tuple(v2, w, label));
    numE++;
}

//=================================================================
// insertVertex
// Inserts a vertex with a given key and data
// Parameters:  key  - key of the vertex
//              data - data associated with the vertex
// Returns:     none
//=================================================================
template <class K, class D>
void Graph<K,D>::insertVertex ( K key, tuple<double, double> data )
{
    if (vertices.find(key) != vertices.end()){
        // vertex already exists, so just update data
        vertices[key].data = data;
    }
    else {
        VertexInfo<K,D> newVertex;
        newVertex.data = data;
        newVertex.key = key;
        vertices[key] = newVertex;
        numV++;
    }
}

//=================================================================
// toString
// Represents the graph as a string, each line includes the 
//      adjacency list for a vertex
// Parameters:  none
// Returns:     string representation of the graph
//=================================================================
template <class K, class D>
string Graph<K,D>::toString ( ) const
{
    stringstream ss;
    for (const auto& pair : vertices) {
        ss << pair.first << ": ";
        for (const auto& edge : pair.second.adj) {
            ss << "(" << get<0>(edge) << ", weight: " << get<1>(edge) << ") ";
        }
        ss << endl;
    }
    return ss.str();
}

//=================================================================
// DFSVisit
// Helper function for Depth-First Search
// Parameters:  u - the current vertex
//              time - the current time counter
// Returns:     none
//=================================================================
template <class K, class D>
void Graph<K,D>::DFSVisit ( K u, int& time )
{
    time++;
    vertices[u].d_time = time;
    vertices[u].color = 'g';
    for (const auto& edge : vertices[u].adj) {
        K v = get<0>(edge);
        if (vertices[v].color == 'w') {
            vertices[v].pre = &u;
            DFSVisit(v, time);
        }
    }
    vertices[u].color = 'b';
    time++;
    vertices[u].f_time = time;
}

//=================================================================
// DFS
// Performs Depth-First Search on the graph
// Parameters:  none
// Returns:     none
//=================================================================
template <class K, class D>
void Graph<K,D>::DFS ( )
{
    // set all vertices to initial state (white, no predecessor)
    // in vertices map, .first is key, .second is VertexInfo
    for (auto& u : vertices) {
        u.second.color = 'w';
        u.second.pre = nullptr;
    }
    int time = 0; // initialize time counter
    // visit each vertex
    for (auto& u : vertices) {
        if (u.second.color == 'w') {
            DFSVisit(u.first, time);
        }
    }
}

//=================================================================
// topologicalSort
// Returns a string representing the topological sort of the graph
//  (keys in decreasing finishing time order)
//   Main idea: visit each vertex only after all dependencies are
//   complete (i.e., all vertices along path to it have been visited)
//   Requires graph to be a DAG (Directed Acyclic Graph)
// Parameters:  none
// Returns:     string representation of the topological sort
//=================================================================
template <class K, class D>
string Graph<K,D>::topologicalSort( )
{
    DFS();

    // get all the finishing times, store in a priority queue with respective keys
    // priority determined by finishing time (max has highest priority)
    priority_queue<pair<int, K>> finishTimes;
    for (const auto& pair : vertices) {
        finishTimes.push({pair.second.f_time, pair.first});
    }
    // build the result string
    // only include "->" between keys, no trailing arrow
    stringstream ss;
    while (!finishTimes.empty()) {
        ss << finishTimes.top().second;
        finishTimes.pop();
        if (!finishTimes.empty()) {
            ss << "->";
        }
    }
    return ss.str();
}
//=================================================================
// BFS
// Performs Breadth-First Search starting from the given source vertex
// Parameters:  source - the starting vertex for BFS
// Returns:     none
//=================================================================
template <class K, class D>
void Graph<K,D>::BFS ( K source )
{
    for (auto& [_, vrt] : vertices) {
        vrt.color = 'w';
        vrt.d = INT_MAX;
        vrt.pre = nullptr;
    }
    VertexInfo<K, D>& src = vertices[source];
    src.color = 'g';
    src.d = 0;
    src.pre = nullptr;

    queue<K> q;
    q.push(source);
    while(!q.empty()) {
        K predecessor = q.front();
        q.pop();

        const auto& adj = vertices[predecessor].adj;

        for (const tuple<K, int, string>& edge: adj) {
            K v_key = get<0>(edge);
            VertexInfo<K, D>& v = vertices[v_key];
            if (v.color == 'w') {
                v.color = 'g';
                v.d = vertices[predecessor].d + 1;
                v.pre = &vertices[predecessor].key;
                q.push(v_key);
            }
        }
        vertices[predecessor].color = 'b';
    }
}

//=================================================================
// shortestPath
// Finds the shortest path between two vertices using BFS results
// Parameters:  s - source vertex key
//              d - destination vertex key
// Returns:     string representation of the shortest path
//=================================================================
template <class K, class D>
string Graph<K,D>::shortestPath ( K s, K d, bool weighted )
{
    if (!weighted) {
        BFS(s);
        
        if (vertices.find(s) == vertices.end() || vertices.find(d) == vertices.end()) {
            return "Either one or both of your input keys don't exist as a vertex.";
        } else {
            return shortestPathRecursive(s, d);
        }
    } else {
        dijkstra(s);
    }
}

//=================================================================
// shortestPathRecursive
//=================================================================
template <class K, class D>
string Graph<K,D>::shortestPathRecursive ( K s, K d, int distance )
{
    if (s == d) {
        VertexInfo<K, D>& s_string = vertices[s];
        tuple<double, double> s_info = s_string.data;
        return string("Total distance: ") + to_string(distance) + "\n(" + (to_string(get<0>(s_info))) + ", " + (to_string(get<1>(s_info))) + ")" + "\n";
    } else if (vertices.at(d).pre == nullptr) {
        return "";
    } else {

        VertexInfo<K, D>& d_string = vertices[d];
        tuple<double, double> d_info = d_string.data;
        string label;
        // assert(vertices.at(d).pre != nullptr);
        for (auto& edge : vertices[*vertices.at(d).pre].adj) {
            if (get<0>(edge) == d) {
                label = get<2>(edge);
            }
        }

        string d_str = label + "(" + (to_string(get<0>(d_info))) + ", " + (to_string(get<1>(d_info))) + ")" + "\n";
        return shortestPathRecursive(s, *vertices.at(d).pre, distance + 1) + d_str;
    }
}

//=================================================================
// asAdjMatrix
// Returns the adjacency matrix representation of the graph
//   smallest key value corresponds to row/column 0, etc.
//   use weight value for edges, INT_MAX for no edge
// Parameters:  none
// Returns:     2D array (matrix) of edge weights
//=================================================================
template <class K, class D>
int** Graph<K,D>::asAdjMatrix ( ) const
{
    int** matrix = new int*[numV];

    for (int i = 0; i < numV; i++) {
        matrix[i] = new int[numV];
    }

    for (int i = 0; i < numV; i++) {
        for (int j = 0; j < numV; j++) {
            matrix[i][j] = INT_MAX;
        }
    }

    for (int k = 0; k < numV; k++) {
        for (auto& edge : vertices.at(k).adj) {
            K key = get<0>(edge);
            int weight = get<1>(edge);
            matrix[k][key] = weight;
        }
    }

    return matrix;
}

//=================================================================
// dijkstra
//=================================================================
template <class K, class D> 
void Graph<K,D>::dijkstra ( K s ) 
{
    // Initialize single source 
    initializeSingleSource(s);
    set<K> keys_processed; 

    auto cmp = [](VectorInfo<K, D> left, VectorInfo<K, D> right) { return left.d < right.d; };
    priority_queue<VectorInfo<K,D>, vector<VectorInfo<K,D>>, decltype(cmp)> q(cmp);

    for (auto& [_, vrt] : vertices) {
        q.push(vrt);
    }

    K u;
    while (!q.empty()) { 
        // these two lines act as EXTRACT-MIN 
        u = q.top();
        q.pop();

        if (keys_processed.contains(u)) {
            continue;
        }

        // add u to the end of s
        s.insert(u);

        for (auto& edge : vertices.at(u).adj) {
            K v_key = get<0>(edge);
            VertexInfo<K,D>& v = vertices.at(v_key);
            relax(u, v);

            if (relax == true) {
                // decrease key
                q.push(v);
            }
        }  
    }
}

//=================================================================
// initializeSingleSource
//=================================================================
template <class K, class D>
void Graph<K,D>::initializeSingleSource ( K s )
{
    for (auto& [_, vrt] : vertices) {
        vrt.d = INT_MAX;
        vrt.pre = nullptr;
    }

    vertices.at(s).d = 0;
}

//=================================================================
// relax
//=================================================================
template <class K, class D>
bool Graph<K,D>::relax( K u, K v )
{
    int weight = 0;
    for (auto& edge : vertices.at(v).adj) {
        if (get<0>(edge) == u) {
            weight = get<0>(edge);
        }
    }

    if (vertices.at(v).d > vertices.at(u).d + weight) {
        vertices.at(v).d = vertices.at(u).d + weight;
        vertices.at(v).pre = &u;
        return true;
    } 

    return false;
}