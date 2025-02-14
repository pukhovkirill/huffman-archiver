cmake_minimum_required(VERSION 3.28)

project(huffman-archiver)

# set(CMAKE_BUILD_TYPE Debug)

IF(NOT DEFINED CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF(NOT DEFINED CMAKE_BUILD_TYPE)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/includes")

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(tests)
ENDIF()

add_subdirectory(src)
