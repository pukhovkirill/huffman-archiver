cmake_minimum_required(VERSION 3.28)
set(CMAKE_VERBOSE_MAKEFILE ON)

project("runtests")

add_subdirectory(suites)

set(CMAKE_C_FLAGS " -std=c99 -O3 -Wall -Wextra -Wimplicit")

include_directories("/usr/include")
add_executable(runtests main.c utils.c)
target_link_libraries(runtests cunit dl)
add_custom_target(test COMMAND ${PROJECT_NAME})
