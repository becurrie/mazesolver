cmake_minimum_required(VERSION 3.9)
project(MazeSolver_v2)

set(CMAKE_CXX_STANDARD 11)

add_executable(MazeSolver_v2 main.cpp StackNode.cpp StackNode.h Stack.cpp Stack.h Maze.cpp Maze.h)