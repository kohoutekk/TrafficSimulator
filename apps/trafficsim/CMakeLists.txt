find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(trafficsim
    main.cpp
    src/MainWindow.hpp
    src/SimulationView.hpp
    src/CarItem.hpp
)

target_link_libraries(trafficsim
    PRIVATE core
    PRIVATE Qt6::Widgets
)
