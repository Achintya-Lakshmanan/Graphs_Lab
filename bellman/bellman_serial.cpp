#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <chrono>
#include <limits>

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

    vector<int> bellman_ford(int startVertex) {
        vector<int> distance(vertices, INT_MAX);
        vector<int> prev_distance(vertices, INT_MAX);
        distance[startVertex] = 0;

        for (int i = 0; i < vertices - 1; ++i) {
            prev_distance = distance;
            bool changed = false;
            for (int u = 0; u < vertices; ++u) {
                for (int v = 0; v < vertices; ++v) {
                    if (adjMatrix[u][v] != INT_MAX && distance[u] != INT_MAX &&
                        distance[u] + adjMatrix[u][v] < distance[v]) {
                        distance[v] = distance[u] + adjMatrix[u][v];
                        changed = true;
                    }
                }
            }
            if (!changed) {
                // If no changes were made in this iteration, check if it's the same as the previous iteration
                if (distance == prev_distance) {
                    break;  // Stop if distance remains the same for 2 iterations
                }
            }
        }

        // for (int u = 0; u < vertices; ++u) {
        //     for (int v = 0; v < vertices; ++v) {
        //         if (adjMatrix[u][v] != INT_MAX && distance[u] != INT_MAX &&
        //             distance[u] + adjMatrix[u][v] < distance[v]) {
        //             cerr << "Graph contains a negative-weight cycle" << endl;
        //             return vector<int>(vertices, INT_MIN);
        //         }
        //     }
        // }
        return distance;
    }

    vector<vector<int>> shortestpaths_all() {
        vector<vector<int>> all_distances(vertices);

        for (int i = 0; i < vertices; ++i) {
            all_distances[i] = bellman_ford(i);
        }
        return all_distances;
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

        vector<vector<int>> shortest = g.shortestpaths_all();

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