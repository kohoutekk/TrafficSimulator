cmake_minimum_required(VERSION 3.16)
project(TrafficSimulator)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libs/pathfinding)
add_subdirectory(apps/core)
add_subdirectory(apps/test)
add_subdirectory(apps/trafficsim)
