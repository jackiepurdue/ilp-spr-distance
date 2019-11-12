cmake_minimum_required(VERSION 3.0)
project(cspr_main)

set(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")
set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

add_executable(cspr_main
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
        glpk/orig.cpp
        glpk/orig.h
		../src/glpk/param.cpp main.cpp main.h)

target_link_libraries(cspr_main glpk)
