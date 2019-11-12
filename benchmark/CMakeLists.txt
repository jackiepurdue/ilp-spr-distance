cmake_minimum_required(VERSION 3.0)
project(cspr_benchmark)




set(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")
set(CMAKE_CXX_STANDARD 11)
set(EXEC_NAME cspr_benchmark)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

add_executable(cspr_benchmark
        ../benchmark/benchmark.cpp
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



FIND_PACKAGE(BenchMark)

include_directories(${Benchmark_INCLUDE_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(cspr_benchmark glpk Threads::Threads "${CMAKE_THREAD_LIBS_INIT}" ${Benchmark_LIBRARY} benchmark )