#include <iostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <ctime>

using namespace std;

// Function to perform DFS on the graph
void dfs(vector<vector<int>>& graph, int startNode) {
    int numNodes = graph.size();
    vector<bool> visited(numNodes, false);
    stack<int> nodeStack;

    nodeStack.push(startNode);
    visited[startNode] = true;

    while (!nodeStack.empty()) {
        int currentNode = nodeStack.top();
        nodeStack.pop();
        cout << currentNode << " ";

        for (int neighbor : graph[currentNode]) {
            if (!visited[neighbor]) {
                nodeStack.push(neighbor);
                visited[neighbor] = true;
            }
        }
    }
}

int main() {
    vector<vector<int>> graph = {
        {1, 2},     // Node 0 is connected to nodes 1 and 2
        {0, 3, 4},  // Node 1 is connected to nodes 0, 3, and 4
        {0, 5},     // Node 2 is connected to nodes 0 and 5
        {1},        // Node 3 is connected to node 1
        {1},        // Node 4 is connected to node 1
        {2}         // Node 5 is connected to node 2
    };


    cout << "DFS traversal: ";
    dfs(graph, 0); // Perform DFS starting from node 0

    return 0;
}