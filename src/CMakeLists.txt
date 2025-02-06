add_subdirectory(huffman)
add_subdirectory(archiver)

set(CMAKE_C_FLAGS " -std=c99")
add_executable(huffman-archiver main.c
        archiver/huffman_archiver.c
        ../includes/archiver.h)
