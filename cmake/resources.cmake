if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DRELEASE_BUILD)
endif()

add_executable(resource_exporter src/resource_exporter.cpp)
target_link_libraries(resource_exporter raylib)

set(EMBEDDED_RESOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/font_ubuntu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/icon_calc.h
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/includes)

add_custom_command(
    OUTPUT ${EMBEDDED_RESOURCE_FILES}
    COMMAND resource_exporter
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS resource_exporter ${CMAKE_CURRENT_SOURCE_DIR}/resource/Ubuntu-Regular.ttf ${CMAKE_CURRENT_SOURCE_DIR}/resource/calc.png
    COMMENT "Generating embedded resource files"
    VERBATIM
)

add_custom_target(generate_resources ALL DEPENDS ${EMBEDDED_RESOURCE_FILES})

add_executable(fix_headers src/fix_generated_headers.cpp)
target_link_libraries(fix_headers PRIVATE raylib)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/headers_fixed.stamp
    COMMAND fix_headers
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS generate_resources fix_headers
    COMMENT "Fixing generated headers"
)

add_custom_target(fix_headers_target ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/headers_fixed.stamp)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resource)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/resource/Ubuntu-Regular.ttf 
    ${CMAKE_CURRENT_BINARY_DIR}/resource/Ubuntu-Regular.ttf 
    COPYONLY
)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/resource/calc.png
    ${CMAKE_CURRENT_BINARY_DIR}/resource/calc.png
    COPYONLY
)