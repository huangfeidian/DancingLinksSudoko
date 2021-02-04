cmake_minimum_required(VERSION 3.2)
PROJECT(dancing_links)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest")
endif(MSVC)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

FILE(GLOB_RECURSE DLX_SRCS ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_library(dancing_links ${DLX_SRCS})

ADD_EXECUTABLE(matrix_solver ${CMAKE_SOURCE_DIR}/test/basic_matrix_solver.cpp)
target_link_libraries(matrix_solver dancing_links)

ADD_EXECUTABLE(sudoku_dlx_heap_test ${CMAKE_SOURCE_DIR}/test/sudoku_dlx_heap_test.cpp)
target_link_libraries(sudoku_dlx_heap_test dancing_links)

ADD_EXECUTABLE(sudoku_dlx_solve_one ${CMAKE_SOURCE_DIR}/test/sudoku_dlx_solve_one.cpp)
target_link_libraries(sudoku_dlx_solve_one dancing_links)

ADD_EXECUTABLE(sudoku_dlx_solve_all ${CMAKE_SOURCE_DIR}/test/sudoku_dlx_solve_all.cpp)
target_link_libraries(sudoku_dlx_solve_all dancing_links)

ADD_EXECUTABLE(array_heap_test ${CMAKE_SOURCE_DIR}/test/array_heap_test.cpp)
target_link_libraries(array_heap_test dancing_links)
