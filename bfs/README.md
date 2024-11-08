## Overview

random_graph_generate.py -> generate a random graph and stores as random_graph.txt <br/>

run .sh files to compile the code and run the executable

## Approach

BFS serial -> Maintian queue of visited nodes and explore new nodes as per order of queue <br/>

BFS parallel -> Repeatedly perform adjacency_matrix * unit_vector(target_node) until all nodes have been explored