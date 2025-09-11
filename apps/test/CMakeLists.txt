find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(test
    main.cpp
)

target_link_libraries(test
    PRIVATE core
    PRIVATE Qt6::Widgets
)
