# clang++ -std=c++11 bfs_parallel.cpp -o bfs_parallel -framework Accelerate -stdlib=libc++ -lc++abi
# ./bfs_parallel

clang++ -std=c++11 bfs_parallel.cpp -o bfs_parallel
./bfs_parallel