cmake_minimum_required(VERSION 3.10)

project(ray)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE
        src/main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        raylib
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resource)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resource/CascadiaCode-Regular.ttf ${CMAKE_CURRENT_BINARY_DIR}/resource/CascadiaCode-Regular.ttf COPYONLY)
