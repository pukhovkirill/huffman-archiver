cmake_minimum_required(VERSION 3.28)
project(huffman_archiver C)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_C_STANDARD 99)

add_executable(huffman_archiver main.c
        huffman/huffman_algorithm.h
        huffman/huffman_algorithm.c
        huffman/h_priority_queue.h
        huffman/h_priority_queue.c
        huffman/h_tree.h
        huffman/h_tree.c
)
