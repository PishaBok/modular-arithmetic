cmake_minimum_required(VERSION 3.5)

project(Modular_Arithmetic)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH "/Users/pavel/Qt/6.6.1/macos")

find_package(Boost REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Charts Widgets)

set(PROJECT_SOURCES
        main.cpp
        rns.cpp rns.hpp    
        my_task.hpp my_task.cpp    
)

add_executable(Modular_Arithmetic ${PROJECT_SOURCES})

target_link_libraries(Modular_Arithmetic PRIVATE Boost::boost)
target_link_libraries(Modular_Arithmetic PRIVATE Qt6::Core Qt6::Charts Qt6::Widgets)
