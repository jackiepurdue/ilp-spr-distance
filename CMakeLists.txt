cmake_minimum_required(VERSION 3.0)
project(cspr)

set(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")
set(CMAKE_CXX_STANDARD 11)
set(EXEC_NAME cspr_benchmark)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(statistics)
add_subdirectory(benchmark)

