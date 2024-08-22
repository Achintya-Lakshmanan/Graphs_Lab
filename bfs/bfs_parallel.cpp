#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <omp.h>

using namespace std;

class Graph {
private:
    int vertices;
    vector<vector<int>> adjMatrix;

public:
    Graph(const string& filename) {
        readGraphFromFile(filename);
    }

    void readGraphFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            throw runtime_error("File not found");
        }

        file >> vertices;
        adjMatrix.resize(vertices, vector<int>(vertices, 0));

        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                file >> adjMatrix[i][j];
            }
        }

        file.close();
    }

    void printGraph() {
        cout << "Adjacency Matrix:" << endl;
        for (int i = 0; i < vertices; ++i) {
            cout << i << ": ";
            for (int j = 0; j < vertices; ++j) {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    vector<int> bfs(int startVertex) {
        vector<bool> visited(vertices, false);
        vector<int> bfsOrder = {startVertex};
        vector<int> unit_vector(vertices, 0);
        unit_vector[startVertex] = 1;
        visited[startVertex] = true;

        while (true) {
            vector<int> new_unit_vector(vertices, 0);

            #pragma omp parallel for
            for (int i = 0; i < vertices; i++) {
                int sum = 0;
                for (int j = 0; j < vertices; j++) {
                    sum += adjMatrix[i][j] * unit_vector[j];
                }
                new_unit_vector[i] = sum;
            }

            bool foundNew = false;
            #pragma omp parallel for
            for (int i = 0; i < vertices; i++) {
                if (new_unit_vector[i] != 0 && !visited[i]) {
                    #pragma omp critical
                    {
                        visited[i] = true;
                        bfsOrder.push_back(i);
                        foundNew = true;
                    }
                }
            }

            if (!foundNew) {
                break;
            }

            unit_vector = new_unit_vector;
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
        //     cout << "BFS starting from vertex " << i << ": ";
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