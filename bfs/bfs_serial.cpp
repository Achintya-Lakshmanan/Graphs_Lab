#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <chrono>

using namespace std;

class Graph {
private:
    int vertices;
    vector<vector<int>> adjacencyList;

public:
    Graph(const string& filename) {
        readGraphFromFile(filename);
    }

    void readGraphFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            throw runtime_error("Unable to open file: " + filename);
        }

        string line;
        getline(file, line);
        istringstream iss(line);
        iss >> vertices;

        adjacencyList.resize(vertices);

        for (int i = 0; i < vertices; ++i) {
            getline(file, line);
            istringstream row(line);
            for (int j = 0; j < vertices; ++j) {
                int edge;
                row >> edge;
                if (edge == 1) {
                    adjacencyList[i].push_back(j);
                }
            }
        }

        file.close();
    }

    void printGraph() {
        cout << "Adjacency List:" << endl;
        for (int i = 0; i < vertices; ++i) {
            cout << i << ": ";
            for (int neighbor : adjacencyList[i]) {
                cout << neighbor << " ";
            }
            cout << endl;
        }
    }

    vector<int> bfs(int startVertex) {
        vector<bool> visited(vertices, false);
        vector<int> bfsOrder;
        queue<int> queue;

        visited[startVertex] = true;
        queue.push(startVertex);

        while (!queue.empty()) {
            int currentVertex = queue.front();
            queue.pop();
            bfsOrder.push_back(currentVertex);

            for (int neighbor : adjacencyList[currentVertex]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    queue.push(neighbor);
                }
            }
        }

        return bfsOrder;
    }

    int getVertices() const {
        return vertices;
    }
};

int main() {
    try {
        auto start = chrono::high_resolution_clock::now();

        Graph g("random_graph.txt");

        cout << "Graph read from file:" << endl;
        // g.printGraph();

        // for (int i = 0; i < g.getVertices(); ++i) {
        //     vector<int> bfsOrder = g.bfs(i);
        //     cout << "vertex " << i << ": ";
        //     for (int v : bfsOrder) {
        //         cout << v << " ";
        //     }
        //     cout << endl;
        // }

        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duration = end - start;

        cout << "\nTotal execution time: " << duration.count() << " milliseconds" << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}