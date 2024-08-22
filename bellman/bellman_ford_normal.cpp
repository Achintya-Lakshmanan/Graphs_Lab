#include <iostream>
#include <fstream>
#include <vector>
#include <limits>

using namespace std;

void bellmanFord(const vector<vector<int>>& graph, int source) {
    int V = graph.size();
    vector<int> distance(V, numeric_limits<int>::max());
    distance[source] = 0;

    // Relax edges |V| - 1 times
    for (int i = 1; i < V; i++) {
        for (int u = 0; u < V; u++) {
            for (int v = 0; v < V; v++) {
                if (graph[u][v] != 0 && distance[u] != numeric_limits<int>::max() &&
                    distance[u] + graph[u][v] < distance[v]) {
                    distance[v] = distance[u] + graph[u][v];
                }
            }
        }
    }

    // Check for negative-weight cycles
    for (int u = 0; u < V; u++) {
        for (int v = 0; v < V; v++) {
            if (graph[u][v] != 0 && distance[u] != numeric_limits<int>::max() &&
                distance[u] + graph[u][v] < distance[v]) {
                cout << "Graph contains negative weight cycle" << endl;
                return;
            }
        }
    }

    // Print the distance array
    cout << "Vertex Distance from Source (" << source << "):" << endl;
    for (int i = 0; i < V; i++) {
        cout << "Vertex " << i << ": " << (distance[i] == numeric_limits<int>::max() ? "INF" : to_string(distance[i])) << endl;
    }
}

int main() {
    ifstream inputFile("random_graph.txt");
    if (!inputFile) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    int V;
    inputFile >> V;

    vector<vector<int>> graph(V, vector<int>(V, 0));

    // Read the adjacency matrix
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            inputFile >> graph[i][j];
        }
    }

    inputFile.close();

    int source;
    cout << "Enter the source vertex (0 to " << V - 1 << "): ";
    cin >> source;

    if (source < 0 || source >= V) {
        cerr << "Invalid source vertex." << endl;
        return 1;
    }

    bellmanFord(graph, source);

    return 0;
}