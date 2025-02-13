add_subdirectory(archiver)
add_subdirectory(huffman)
add_subdirectory(utils)

include_directories( "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../../includes")

set(CMAKE_C_FLAGS " -std=c99")
add_executable(huffer main.c)
target_link_libraries(huffer huff arc util)
