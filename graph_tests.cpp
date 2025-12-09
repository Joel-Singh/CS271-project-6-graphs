// *******************************************
//  test_graph_example.cpp
//  CS 271 Graph Project
//  Joel Singh, Colin Nguyen
// *******************************************

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>
#include <climits>
#include <limits>
#include "graph.h"
#include <tuple>
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
        // string data;
        double x;
        double y;
        tuple<double, double> data;
        infile >> key >> x >> y;
        get<0>(data) = x;
        get<1>(data) = y; 
        g.insertVertex(key, data);
    }
    for (int i = 0; i < e; ++i) {
        int from, to;
        float weight;
        string label;
        infile >> from >> to >> weight;
        std::getline(infile, label);
        cout << from << " " << to << " " << weight << " " << label << endl;
        g.insertEdge(from, to, weight, label);
    }
    return g;
}

void test_BFS_lengthTwo()
{
   try{
       Graph<int, string> g = createGraphFromFile("lengthTwo.txt");
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

void test_BFS_lengthFive()
{
   try{
       Graph<int, string> g = createGraphFromFile("lengthFive.txt");
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

void test_BFS_lengthOne()
{
   try{
       Graph<int, string> g = createGraphFromFile("lengthOne.txt");
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


void test_shortestPath_lengthTwo()
{
    try{
        Graph<int, string> g = createGraphFromFile("lengthTwo.txt");
        string path = g.shortestPath(35429, 35429);
        cout << "Shortest path from 0 to 1: " << path << endl;
        if (path != "0->1") {
            cout << "Shortest path result is incorrect. Expected: 0->1 but got: " << path << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "Error testing shortest path: " << e.what() << endl;
    }
}




void test_shortestPath_lengthFive()
{
    try{
        Graph<int, string> g = createGraphFromFile("denison.txt");
        string path = g.shortestPath(73712, 635949);
        // cout << "Shortest path from 1 to 3: " << path << endl;
        if (path != "1->3") {
            cout << "Shortest path result is incorrect. Expected: 1->3 but got: " << path << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "Error testing shortest path: " << e.what() << endl;
    }
}

void test_shortestPath_nonexistantVertex()
{
    try{
        Graph<int, string> g = createGraphFromFile("lengthTwo.txt");
        string path = g.shortestPath(0, 12);
        cout << "Shortest path from 0 to 112: " << path << endl;
        if (path != "0->1") {
            cout << "Shortest path result is incorrect. Expected: 0->1 but got: " << path << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "Correctly caucht error testing shortest path: " << e.what() << endl;
    }
}


void test_asAdjMatrix_empty()
{
    try{
        Graph<int, string> g = createGraphFromFile("empty.txt");
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

        int expectedMatrix[g.size()][g.size()] = {};
        
        string expectedString ="";

        if (actualString != expectedString) {
            cout << "Adjacency matrix is incorrect. Expected:\n" << expectedString << "but got:\n" << actualString << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "Error testing adjacency matrix: " << e.what() << endl;
    }    
}


void test_asAdjMatrix_lengthTwo()
{
    try{
        Graph<int, string> g = createGraphFromFile("lengthTwo.txt");
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
            {INT_MAX, 1},
            {1, INT_MAX}
        };
        
        string expectedString = "inf 1\n1 inf\n";

        if (actualString != expectedString) {
            cout << "Adjacency matrix is incorrect. Expected:\n" << expectedString << "but got:\n" << actualString << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "Error testing adjacency matrix: " << e.what() << endl;
    }    
}

void test_asAdjMatrix_lengthOne()
{
    try{
        Graph<int, string> g = createGraphFromFile("lengthOne.txt");
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
        
        string expectedString = "inf\n";

        if (actualString != expectedString) {
            cout << "Adjacency matrix is incorrect. Expected:\n" << expectedString << "but got:\n" << actualString << endl;
        }
    }
    catch (std::exception& e) {
        cerr << "Error testing adjacency matrix: " << e.what() << endl;
    }    
}

void test_asAdjMatrix_lengthFive()
{
    try{
        Graph<int, string> g = createGraphFromFile("lengthFive.txt");
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
            {INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX},
            {INT_MAX, 1, 1, 1, INT_MAX},
            {INT_MAX, 1, 1, 1, INT_MAX},
            {INT_MAX, 1, 1, 1, INT_MAX},
            {INT_MAX, INT_MAX, INT_MAX, INT_MAX, INT_MAX},
        };
        
        string expectedString = "inf inf inf inf inf\ninf 1 1 1 inf\ninf 1 1 1 inf\ninf 1 1 1 inf\ninf inf inf inf inf\n";

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
    // test_asAdjMatrix_empty();
    // test_BFS_lengthTwo();
    // test_shortestPath_lengthTwo();
    // test_asAdjMatrix_lengthTwo();
    // test_BFS_lengthFive();
    test_shortestPath_lengthFive();
    // test_asAdjMatrix_lengthFive();
    // test_asAdjMatrix_lengthOne();
    // test_shortestPath_nonexistantVertex();

    // cout << "Testing completed" << endl;
    // ifstream infile("example.txt");
    // if (!infile) {
    //     cerr << "Error opening file: " << endl;
    //     return 0;
    // }

    // string line;
    // getline(infile, line);

    // int next_space = line.find(' ');
    // string from = line.substr(0, next_space);
    // cout << "The from is " << from << endl;

    // line = line.substr(next_space+1);
    // next_space = line.find(' ');
    // string to = line.substr(0, next_space);
    // cout << "The to is " << to << endl;

    // line = line.substr(next_space+1);
    // next_space = line.find(' ');
    // string weight = line.substr(0, next_space);
    // cout << "The weight is " << weight << endl;

    // string label = line.substr(next_space+1);
    // cout << "The label is" << label << endl;

    
    
    // string substr3 = line.substr(next_space+3);
    // cout << line << endl;
    // cout << substr3 << endl;
    return 0;
}
