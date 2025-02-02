add_subdirectory(huffman)

set(CMAKE_C_FLAGS " -std=c99")
add_executable(huffman-archiver main.c)
