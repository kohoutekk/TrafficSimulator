find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(traffic_simulator
    main.cpp
)

target_link_libraries(traffic_simulator
    PRIVATE core
    PRIVATE Qt6::Widgets
)
