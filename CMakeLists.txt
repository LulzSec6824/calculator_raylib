cmake_minimum_required(VERSION 3.10)

project(ray)

find_package(raylib CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE
       src/main.cpp
       src/calculator.cpp
       src/button.cpp
       includes/button.h
       includes/calculator.h
       
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        raylib
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resource)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resource/Ubuntu-Regular.ttf ${CMAKE_CURRENT_BINARY_DIR}/resource/Ubuntu-Regular.ttf COPYONLY)
