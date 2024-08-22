import random

def generate_random_graph(num_nodes, edge_probability):
    with open('random_graph.txt', 'w') as f:
        # Write the number of nodes
        f.write(f"{num_nodes}\n")
        
        # Generate the upper triangular part of the adjacency matrix
        matrix = [[0 for _ in range(num_nodes)] for _ in range(num_nodes)]
        for i in range(num_nodes):
            for j in range(i+1, num_nodes):
                if random.random() < edge_probability:
                    matrix[i][j] = matrix[j][i] = 1

        # Write the symmetric adjacency matrix
        for row in matrix:
            f.write(' '.join(map(str, row)) + '\n')

# Generate a 500-node graph with approximately 2% edge density
n, d = 50000, .01
generate_random_graph(n, d)

print(f"Random graph with {n} nodes has been generated and saved to 'random_graph.txt'")