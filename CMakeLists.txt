cmake_minimum_required(VERSION 3.10)
project(ray)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(raylib)

include(cmake/platforms.cmake)
include(cmake/resources.cmake)

if(IS_WINDOWS)
    if(CMAKE_CONFIGURATION_TYPES)
        add_executable(${PROJECT_NAME} WIN32
            src/main.cpp
            src/calculator.cpp
            src/button.cpp
            src/parser.cpp
            src/metrics.cpp
            src/theme.cpp
            src/display.cpp
            src/winmain.cpp
            ${EMBEDDED_RESOURCE_FILES}
        )
        set_target_properties(${PROJECT_NAME} PROPERTIES
            LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
        )
    else()
        if(CMAKE_BUILD_TYPE STREQUAL "Release")
            add_executable(${PROJECT_NAME} WIN32
                src/main.cpp
                src/calculator.cpp
                src/button.cpp
                src/parser.cpp
                src/metrics.cpp
                src/theme.cpp
                src/display.cpp
                src/winmain.cpp
                ${EMBEDDED_RESOURCE_FILES}
            )
        else()
            add_executable(${PROJECT_NAME}
                src/main.cpp
                src/calculator.cpp
                src/button.cpp
                src/parser.cpp
                src/metrics.cpp
                src/theme.cpp
                src/display.cpp
                ${EMBEDDED_RESOURCE_FILES}
            )
        endif()
    endif()
else()
    add_executable(${PROJECT_NAME}
        src/main.cpp
        src/calculator.cpp
        src/button.cpp
        src/parser.cpp
        src/metrics.cpp
        src/theme.cpp
        src/display.cpp
        ${EMBEDDED_RESOURCE_FILES}
    )
endif()

add_dependencies(${PROJECT_NAME} generate_resources fix_headers_target)

target_include_directories(${PROJECT_NAME} PRIVATE includes)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

if(NOT IS_WINDOWS)
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()