cmake_minimum_required(VERSION 3.25.1)
project(06_JanKwiatkowski)

set(CMAKE_CXX_STANDARD 23)

# compiler warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

add_executable(06_JanKwiatkowski main.cpp
        mgmres.cpp
        mgmres.hpp
        constants.h
        types.h
        algebraic_solver.cpp
        algebraic_solver.h)
