add_library(core STATIC
    include/core/TrafficLight.hpp
    include/core/Vehicle.hpp
    include/core/Simulation.hpp
    include/core/Road.hpp
    include/core/Lane.hpp
    include/core/Intersection.hpp

    src/Vehicle.cpp
    src/Simulation.cpp
)

target_include_directories(core
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(core
    PRIVATE pathfinding
)
