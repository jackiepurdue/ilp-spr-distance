cmake_minimum_required(VERSION 3.0)
project(cspr_test)

set(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_C_FLAGS_DEBUG "-D_DEBUG")
set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})

#add_subdirectory(src/benchmark)

#source: https://github.com/gonium/sudoku
#if(GECODE_LOCAL)
#	add_subdirectory (${GECODE_LOCAL})
#	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/${GECODE_LOCAL})
#	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/${GECODE_LOCAL})
#	set(Gecode_LIBRARIES gecodekernel gecodeint gecodesearch gecodeminimodel)
#	set(Gecode_LIBRARIES_D gecodekernel gecodeint gecodesearch gecodeminimodel)
#else(GECODE_LOCAL)
#	FIND_PACKAGE(Gecode)
#	include_directories(${Gecode_INCLUDE_DIRS})
#endif(GECODE_LOCAL)

add_executable(cspr_test
        ../tests/glpk.cpp
		../tests/experimental.cpp
        ../tests/other.cpp
        ../tests/lexical.cpp
        ../tests/tree.cpp
        ../src/phylotree/tree.h
        ../src/phylotree/tree.cpp
        ../src/phylotree/euler_tree.cpp
        ../src/phylotree/euler_tree.h
        ../src/newick/lexer.cpp
        ../src/newick/lexer.h
        ../src/newick/token.cpp
        ../src/newick/token.h
		../tests/catch.h
        ../src/newick/parser.h
        ../src/newick/parser.cpp
        ../src/phylotree/sparse_table.h
        ../src/glpk/param.cpp
        ../src/glpk/param.h
        ../src/glpk/orig.cpp
        ../src/glpk/orig.h
		../src/glpk/param.cpp)

#target_link_libraries(cspr_test glpk ${Gecode_LIBRARIES} ${Gecode_LIBRARIES_D})
target_link_libraries(cspr_test glpk)