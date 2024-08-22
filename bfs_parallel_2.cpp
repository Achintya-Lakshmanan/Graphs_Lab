#include <Accelerate/Accelerate.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <chrono>

class Graph {
private:
    int vertices;
    std::vector<std::vector<float>> adjacencyMatrix;

public:
    Graph(const std::string& filename) {
        readGraphFromFile(filename);
    }

    void readGraphFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + filename);
        }

        std::string line;
        getline(file, line);
        std::istringstream iss(line);
        iss >> vertices;

        adjacencyMatrix.resize(vertices, std::vector<float>(vertices, 0));

        for (int i = 0; i < vertices; ++i) {
            getline(file, line);
            std::istringstream row(line);
            for (int j = 0; j < vertices; ++j) {
                row >> adjacencyMatrix[i][j];
            }
        }

        file.close();
    }

    void printGraph() {
        std::cout << "Adjacency Matrix:" << std::endl;
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                // std::cout << adjacencyMatrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    std::vector<int> bfsMatrixMultiplication(int startVertex) {
        std::vector<float> unit(vertices, 0);
        unit[startVertex] = 1;

        std::vector<float> visited(vertices, 0);
        visited[startVertex] = 1;

        std::vector<float> result(vertices, 0);
        std::vector<int> bfsOrder;
        bfsOrder.push_back(startVertex);

        std::vector<std::vector<float>> transposedMatrix(vertices, std::vector<float>(vertices));
        // #pragma omp parallel for )
        for (int i=0; i<vertices; i++)
        {
            for (int j=0; j<vertices; j++)
            {
                transposedMatrix[i][j] = adjacencyMatrix[j][i];
            }
        }

        while (true)
        {
            #pragma omp parallel for
            for (int i=0; i<vertices; i++)
            {
                for (int j=0; j<vertices; j++)
                {
                    result[i] += adjacencyMatrix[i][j] * unit[j];
                }
            }

            bool newVertexFound = false;
            #pragma omp parallel for
            for (int i=0; i<vertices; i++)
            {
                if (result[i] > 0 && visited[i] == 0)
                {
                    visited[i] = 1;
                    bfsOrder.push_back(i);
                    newVertexFound = true;
                }
            }

            if (!newVertexFound) break;

            #pragma omp parallel for
            for (int i=0; i<vertices; i++)
            {
                unit[i] = (result[i] > 0 && visited[i] == 1) ? 1 : 0;
            }

            #pragma omp parallel for
            for (int i=0; i<vertices; i++)
            {
                result[i] = 0;
            }
        }

        return bfsOrder;
    }

    //     std::vector<std::vector<float>> transposedMatrix(vertices, std::vector<float>(vertices));
    //     vDSP_mtrans(adjacencyMatrix[0].data(), 1, 
    //     transposedMatrix[0].data(), 1, 
    //     vertices, vertices);
        
    //     while (true) {
    //         vDSP_mmul(adjacencyMatrix[0].data(), 1,
    //         unit.data(), 1,
    //         result.data(), 1,
    //         vertices,
    //         1,
    //         vertices);

    //         bool newVertexFound = false;
    //         for (int i = 0; i < vertices; ++i) {
    //             if (result[i] > 0 && visited[i] == 0) {
    //                 visited[i] = 1;
    //                 bfsOrder.push_back(i);
    //                 newVertexFound = true;
    //             }
    //         }

    //         if (!newVertexFound) break;

    //         for (int i = 0; i < vertices; ++i) {
    //             unit[i] = (result[i] > 0 && visited[i] == 1) ? 1 : 0;
    //         }
    //     }

    //     return bfsOrder;
    // }

    int getVertices() const {
        return vertices;
    }
};

int main() {
    try {
        auto start = std::chrono::high_resolution_clock::now();

        Graph g("random_graph.txt");

        std::cout << "Graph read from file:" << std::endl;
        // g.printGraph();

        std::cout << "\nBFS traversal order for each starting vertex:" << std::endl;
        for (int i = 0; i < g.getVertices(); ++i) {
            std::vector<int> bfsOrder = g.bfsMatrixMultiplication(i);
            // std::cout << "Starting from vertex " << i << ": ";
            for (int v : bfsOrder) {
                // std::cout << v << " ";
            }
            // std::cout << std::endl;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;

        std::cout << "\nTotal execution time: " << duration.count() << " milliseconds" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}