#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <limits>
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
        adjMatrix.resize(vertices, vector<int>(vertices, INT_MAX));

        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                int weight;
                file >> weight;
                if (i == j) {
                    adjMatrix[i][j] = 0; 
                } else if (weight != 0) {
                    adjMatrix[i][j] = weight;
                }
            }
        }

        file.close();
    }

    void printGraph() {
        cout << "Adjacency Matrix:" << endl;
        for (int i = 0; i < vertices; ++i) {
            cout << i << ": ";
            for (int j = 0; j < vertices; ++j) {
                if (adjMatrix[i][j] == INT_MAX) {
                    cout << "INF ";
                } else {
                    cout << adjMatrix[i][j] << " ";
                }
            }
            cout << endl;
        }
    }
    
    vector<vector<int>> min_plus(const vector<vector<int>>& A, const vector<vector<int>>& B) {
        vector<vector<int>> C(vertices, vector<int>(vertices, INT_MAX));
        #pragma omp parallel for
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                for (int k = 0; k < vertices; k++) {
                    if (A[i][k] != INT_MAX && B[k][j] != INT_MAX) {
                        C[i][j] = min(C[i][j], A[i][k] + B[k][j]);
                    }
                }
            }
        }
        return C;
    }

    vector<vector<int>> bellman_ford_all() {
        vector<vector<int>> dist = adjMatrix;
        for (int k = 0; k < vertices - 1; k++) {
            dist = min_plus(dist, dist);
        }
        return dist;
    }

    int getVertices() const {
        return vertices;
    }
        
};

int main() {
    try {
        auto start = chrono::high_resolution_clock::now();

        Graph g("random_graph.txt");

        // cout << "Graph read from file:" << endl;
        // g.printGraph();

        vector<vector<int>> shortest = g.bellman_ford_all();

        // for (int i = 0; i < g.getVertices(); i++) {
        //     cout << "Vertex: " << i;
        //     cout << endl;
        //     for (int j = 0; j < g.getVertices(); j++) {
        //         if (shortest[i][j] == INT_MAX) {
        //             cout << "INF ";
        //         } else {
        //             cout << shortest[i][j] << " ";
        //         }
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