// *******************************************
//  test_graph_example.cpp
//  CS 271 Graph Project: Example Test File
//  Expand on these tests!
// *******************************************

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <climits>
#include "graph.h"
using namespace std;

// helper function to create a graph from a file
// first line: number of vertices (v) and number of edges (e)
// next v lines: vertex key (int) and data (string)
// next e lines: edge from vertex1 to vertex2 with weight
Graph<int, string> createGraphFromFile(const string& filename)
{
    Graph<int, string> g;
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error opening file: " << filename << endl;
        return g;
    }
    int v, e;
    infile >> v >> e;
    for (int i = 0; i < v; ++i) {
        int key;
        string data;
        infile >> key >> data;
        g.insertVertex(key, data);
    }
    for (int i = 0; i < e; ++i) {
        int from, to;
        double weight;
        infile >> from >> to >> weight;
        g.insertEdge(from, to, weight);
    }
    return g;
}

void test_DFS()
{
   Graph<int, string> g = createGraphFromFile("example.txt");
   auto start = chrono::high_resolution_clock::now();
   g.DFS();
   auto end = chrono::high_resolution_clock::now();
   chrono::duration<double> elapsed = end - start;
   cout << "DFS took " << elapsed.count() << " seconds." << endl;
}

void test_topologicalSort()
{
   // create graph from file example.txt
   Graph<int, string> g = createGraphFromFile("example.txt");
   string topSortResult = g.topologicalSort();
   cout << "Topological Sort Result: " << topSortResult << endl;
   if (topSortResult != "0->1->2->3") {
       cout << "Topological sort result is incorrect. Expected: 0->1->2->3 but got: " << topSortResult << endl;
   }
}

void test_BFS()
{
   try{
       Graph<int, string> g = createGraphFromFile("example.txt");
       auto start = chrono::high_resolution_clock::now();
       g.BFS(0);
       auto end = chrono::high_resolution_clock::now();
       chrono::duration<double> elapsed = end - start;
       cout << "BFS took " << elapsed.count() << " seconds from source 0." << endl;
   }
   catch (std::exception& e) {
       cerr << "Error testing BFS: " << e.what() << endl;
       return;
   }
}

void test_shortestPath()
{
    try{
        Graph<int, string> g = createGraphFromFile("example.txt");
        string path = g.shortestPath(0, 3);
        cout << "Shortest path from 0 to 3: " << path << endl;
        if (path != "0->2->3") {
            cout << "Shortest path result is incorrect. Expected: 0->2->3 but got: " << path << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "Error testing shortest path: " << e.what() << endl;
    }
}

void test_asAdjMatrix()
{
    try{
        Graph<int, string> g = createGraphFromFile("example.txt");
        int** adjMatrix = g.asAdjMatrix();
        if (adjMatrix == nullptr) {
            cout << "Error: adjacency matrix is null." << endl;
            return;
        }
        // Convert adjMatrix to string for comparison
        string actualString;
        for (int i = 0; i < g.size(); ++i) {
            for (int j = 0; j < g.size(); ++j) {
                if (adjMatrix[i][j] == INT_MAX)
                    actualString += "inf ";
                else
                actualString += to_string(adjMatrix[i][j]) + " ";
            }
            actualString.pop_back(); // remove trailing space
            actualString += "\n";
        }
        cout << "Adjacency Matrix:" << endl;
        cout << actualString << endl;

        int expectedMatrix[g.size()][g.size()] = {
            {INT_MAX, 1, 2, INT_MAX},
            {INT_MAX, INT_MAX, 3, INT_MAX},
            {INT_MAX, INT_MAX, INT_MAX, 4},
            {INT_MAX, INT_MAX, INT_MAX, INT_MAX}
        };
        
        string expectedString ="inf 1 2 inf\ninf inf 3 inf\ninf inf inf 4\ninf inf inf inf\n";

        if (actualString != expectedString) {
            cout << "Adjacency matrix is incorrect. Expected:\n" << expectedString << "but got:\n" << actualString << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "Error testing adjacency matrix: " << e.what() << endl;
    }    
}

int main()
{

    Graph<int, string> g = createGraphFromFile("example.txt");
    cout << "Graph created from file:" << endl;
    cout << g.toString() << endl;

    test_DFS();
    test_topologicalSort();
    test_BFS();
    test_shortestPath();
    test_asAdjMatrix();

    cout << "Testing completed" << endl;

    return 0;
}
