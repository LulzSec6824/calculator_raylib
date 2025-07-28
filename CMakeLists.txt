cmake_minimum_required(VERSION 3.10)

project(ray)

add_subdirectory(raylib_v5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set Releasebuild profiles

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # Clang specific flags
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # GCC specific flags
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # MSVC specific flags
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2 /D NDEBUG")
endif()

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE
       src/main.cpp
       src/calculator.cpp
       src/button.cpp
       src/parser.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE includes)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        raylib
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resource)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/resource/Ubuntu-Regular.ttf ${CMAKE_CURRENT_BINARY_DIR}/resource/Ubuntu-Regular.ttf COPYONLY)
