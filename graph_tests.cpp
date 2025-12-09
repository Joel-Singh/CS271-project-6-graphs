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

        // Get the rest of the line
        std::getline(infile, label);

        int first_letter = label.find_first_not_of(' ');
        // If there isn't a letter in the rest of the line, there must not be a label
        bool no_label = first_letter == string::npos;
        if (no_label) {
            label = "";
        } else {
            // Remove forward spaces
            label = label.substr(first_letter);

            int last_letter = label.find_last_not_of(' ');

            if (last_letter == string::npos) {
                last_letter = label.size() - 1;
            }

            // Remove trailing spaces
            label = label.substr(0, last_letter + 1);
        }

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




void test_shortestPath_lengthTen()
{
    Graph<int, string> g = createGraphFromFile("denison.txt");
    string path = g.shortestPath(73712, 635949);
    string correct_path = "Total distance: 10\n(-82.518332, 40.069045)\n(-82.518329, 40.069083)\n(-82.520055, 40.069182)\n(-82.522614, 40.070549)\n(-82.522673, 40.070789)\n(-82.522984, 40.071528)\n(-82.523694, 40.071846)\n(-82.525078, 40.072356)\n(-82.525146, 40.072547)\n(-82.525236, 40.072556)\n(-82.525307, 40.072550)\n";

    if (path != correct_path) {
        cout << "Shortest path result is incorrect. got: `" << path << "`" << endl;
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
    test_shortestPath_lengthTen();
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
