cmake_minimum_required(VERSION 3.15)
project(algorithms)

set(CMAKE_CXX_STANDARD 14)

add_executable(algebraic ./tests/algebraic.cpp)

target_include_directories(algebraic
        PRIVATE
        ./algorithms/algebraic/
)

