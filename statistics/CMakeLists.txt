cmake_minimum_required(VERSION 3.0)
project(cspr_statistics)

set(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")
set(CMAKE_CXX_STANDARD 11)
set(EXEC_NAME cspr_benchmark)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

add_executable(cspr_statistics
        ../statistics/statistics.cpp
        ../src/phylotree/tree.h
        ../src/phylotree/tree.cpp
        ../src/phylotree/euler_tree.cpp
        ../src/phylotree/euler_tree.h
        ../src/newick/lexer.cpp
        ../src/newick/lexer.h
        ../src/newick/token.cpp
        ../src/newick/token.h
        ../src/newick/parser.h
        ../src/newick/parser.cpp
        ../src/phylotree/sparse_table.h
        ../src/glpk/param.cpp
        ../src/glpk/param.h
        ../src/glpk/orig.cpp
        ../src/glpk/orig.h
        ../src/glpk/param.cpp)

target_link_libraries(cspr_statistics glpk)