add_library(pathfinding INTERFACE

    include/pathfinding/pathfinding.hpp
)

target_include_directories(pathfinding
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
