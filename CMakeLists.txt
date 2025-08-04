cmake_minimum_required(VERSION 3.10)
project(ray)

# Platform detection
set(IS_WINDOWS FALSE)
set(IS_MACOS FALSE)
set(IS_LINUX FALSE)

if(WIN32)
    set(IS_WINDOWS TRUE)
elseif(APPLE)
    set(IS_MACOS TRUE)
elseif(UNIX AND NOT APPLE)
    set(IS_LINUX TRUE)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler detection
set(IS_MSVC FALSE)
set(IS_CLANG FALSE)
set(IS_GCC FALSE)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(IS_MSVC TRUE)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(IS_CLANG TRUE)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(IS_GCC TRUE)
endif()

# Runtime library configuration
# For MSVC, set static runtime before adding subdirectories
if(IS_MSVC)
    # Use static runtime libraries (/MT)
    foreach(flag_var
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif()
    endforeach(flag_var)
endif()

add_subdirectory(raylib_v5)

# Define RELEASE_BUILD in Release mode to use embedded resources
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-DRELEASE_BUILD)
endif()

# Add resource exporter executable
add_executable(resource_exporter src/resource_exporter.cpp)
target_link_libraries(resource_exporter raylib)

# Custom command to generate embedded resources
set(EMBEDDED_RESOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/font_ubuntu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/icon_calc.h
)

# Make sure the includes directory exists
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/includes)

add_custom_command(
    OUTPUT ${EMBEDDED_RESOURCE_FILES}
    COMMAND resource_exporter
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS resource_exporter ${CMAKE_CURRENT_SOURCE_DIR}/resource/Ubuntu-Regular.ttf ${CMAKE_CURRENT_SOURCE_DIR}/resource/calc.png
    COMMENT "Generating embedded resource files"
    VERBATIM
)

# Add custom target for resource generation
add_custom_target(generate_resources ALL DEPENDS ${EMBEDDED_RESOURCE_FILES})



# Platform-specific optimizations
if(IS_WINDOWS)
    if(IS_MSVC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
        option(USE_AVX2 "Use AVX2 instruction set if available" OFF)
        if(USE_AVX2)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:AVX2")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
        else()
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:SSE2")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
        endif()
        set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /GL /Oi")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL /Oi")
    elseif(IS_CLANG)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
        option(USE_NATIVE_ARCH "Use -march=native for Clang (may not be portable)" OFF)
        if(USE_NATIVE_ARCH)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -mtune=native")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mtune=native")
        else()
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse2")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
        endif()
        option(USE_FAST_MATH "Use -ffast-math for Clang (may affect precision)" OFF)
        if(USE_FAST_MATH)
            set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffast-math")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math")
        endif()
        option(USE_LTO "Use Link Time Optimization for Clang" ON)
        if(USE_LTO)
            set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -flto=thin")
            set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto=thin")
            set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto=thin")
        endif()
    endif()
elseif(IS_MACOS)
    if(IS_CLANG)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mmacosx-version-min=10.13")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=10.13")
    endif()
elseif(IS_LINUX)
    if(IS_GCC OR IS_CLANG)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    endif()
endif()

# Debug build profile settings
if(IS_CLANG)
    set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
elseif(IS_GCC)
    set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
elseif(IS_MSVC)
    set(CMAKE_C_FLAGS_DEBUG "/Od /Zi /RTC1 /MTd /D_DEBUG /W4")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Zi /RTC1 /MTd /D_DEBUG /W4")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG:FULL")
endif()

# Release build profile settings with LTO
if(IS_CLANG)
    if(IS_MACOS)
        # macOS Clang settings
        set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -ffast-math -fomit-frame-pointer")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -ffast-math -fomit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
    elseif(IS_WINDOWS)
        # Windows Clang settings with enhanced optimizations
        set(CLANG_RELEASE_FLAGS "-O3 -DNDEBUG -fomit-frame-pointer -fvectorize -funroll-loops")
        # Add architecture flags if enabled
        if(USE_NATIVE_ARCH)
            set(CLANG_RELEASE_FLAGS "${CLANG_RELEASE_FLAGS} -march=native -mtune=native")
        endif()
        # Add fast math if enabled
        if(USE_FAST_MATH)
            set(CLANG_RELEASE_FLAGS "${CLANG_RELEASE_FLAGS} -ffast-math")
        endif()
        # Add LTO if enabled
        if(USE_LTO)
            set(CLANG_RELEASE_FLAGS "${CLANG_RELEASE_FLAGS} -flto=thin")
            set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto=thin")
        endif()
        
        set(CMAKE_C_FLAGS_RELEASE "${CLANG_RELEASE_FLAGS}")
        set(CMAKE_CXX_FLAGS_RELEASE "${CLANG_RELEASE_FLAGS}")
    else()
        # Linux Clang settings with thin LTO
        set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -flto=thin -march=native -ffast-math -fomit-frame-pointer")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto=thin -march=native -ffast-math -fomit-frame-pointer")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto=thin")
    endif()
elseif(IS_GCC)
    set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -ffast-math -fomit-frame-pointer")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -flto -march=native -ffast-math -fomit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto")
elseif(IS_MSVC)
    set(CMAKE_C_FLAGS_RELEASE "/O2 /Ob3 /GL /Gy /GF /Ot /Oi /GT /Qpar /fp:fast ${ARCH_FLAG} /D NDEBUG /W4")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob3 /GL /Gy /GF /Ot /Oi /GT /Qpar /fp:fast ${ARCH_FLAG} /D NDEBUG /W4")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG /OPT:REF /OPT:ICF /CGTHREADS:8")
endif()

# RelWithDebInfo profile settings
if(IS_CLANG)
    if(IS_WINDOWS)
        # Windows Clang settings with debug info
        set(CLANG_RELWITHDEBINFO_FLAGS "-O2 -g -DNDEBUG")
        
        # Add architecture flags if enabled
        if(USE_NATIVE_ARCH)
            set(CLANG_RELWITHDEBINFO_FLAGS "${CLANG_RELWITHDEBINFO_FLAGS} -march=native -mtune=native")
        endif()
        
        # Add fast math if enabled
        if(USE_FAST_MATH)
            set(CLANG_RELWITHDEBINFO_FLAGS "${CLANG_RELWITHDEBINFO_FLAGS} -ffast-math")
        endif()
        
        set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CLANG_RELWITHDEBINFO_FLAGS}")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CLANG_RELWITHDEBINFO_FLAGS}")
    else()
        # macOS/Linux Clang settings
        set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG -march=native")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG -march=native")
    endif()
elseif(IS_GCC)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG -march=native")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g -DNDEBUG -march=native")
elseif(IS_MSVC)
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "/O2 /Zi /Gy /GF /Oi ${ARCH_FLAG} /D NDEBUG /W4 /MT")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/O2 /Zi /Gy /GF /Oi ${ARCH_FLAG} /D NDEBUG /W4 /MT")
    set(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO "${CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO} /DEBUG:FULL /OPT:REF /OPT:ICF")
endif()

# MinSizeRel profile settings (optimized for size)
if(IS_CLANG)
    if(IS_MACOS)
        set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto")
        set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -flto")
    elseif(IS_WINDOWS)
        # Windows Clang settings optimized for size
        set(CLANG_MINSIZEREL_FLAGS "-Os -DNDEBUG -fomit-frame-pointer -fvectorize")
        
        # Add LTO if enabled
        if(USE_LTO)
            set(CLANG_MINSIZEREL_FLAGS "${CLANG_MINSIZEREL_FLAGS} -flto=thin")
            set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -flto=thin")
        endif()
        
        set(CMAKE_C_FLAGS_MINSIZEREL "${CLANG_MINSIZEREL_FLAGS}")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "${CLANG_MINSIZEREL_FLAGS}")
    else()
        # Linux Clang settings with thin LTO
        set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto=thin")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto=thin")
        set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -flto=thin")
    endif()
elseif(IS_GCC)
    set(CMAKE_C_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG -flto")
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} -flto")
elseif(IS_MSVC)
    set(CMAKE_C_FLAGS_MINSIZEREL "/O1 /Os /GL /Gy /GF /Oi /Gw ${ARCH_FLAG} /D NDEBUG /W4 /MT")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "/O1 /Os /GL /Gy /GF /Oi /Gw ${ARCH_FLAG} /D NDEBUG /W4 /MT")
    set(CMAKE_EXE_LINKER_FLAGS_MINSIZEREL "${CMAKE_EXE_LINKER_FLAGS_MINSIZEREL} /LTCG /OPT:REF /OPT:ICF /CGTHREADS:8")
endif()

# Target configuration
# In Release mode on Windows, create a GUI application (no console window)
if(IS_WINDOWS)
    # Handle both single-configuration and multi-configuration generators
    if(CMAKE_CONFIGURATION_TYPES)
        # Multi-configuration generator (Visual Studio, Xcode)
        add_executable(${PROJECT_NAME} WIN32
            src/main.cpp
            src/calculator.cpp
            src/button.cpp
            src/parser.cpp
            src/winmain.cpp
            ${EMBEDDED_RESOURCE_FILES}
        )
        # Set the SUBSYSTEM:CONSOLE property for Debug and RelWithDebInfo configurations
        set_target_properties(${PROJECT_NAME} PROPERTIES
            LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE"
            LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:CONSOLE"
        )
    else()
        # Single-configuration generator
        if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
            add_executable(${PROJECT_NAME} WIN32
                src/main.cpp
                src/calculator.cpp
                src/button.cpp
                src/parser.cpp
                src/winmain.cpp
                ${EMBEDDED_RESOURCE_FILES}
            )
        else()
            add_executable(${PROJECT_NAME}
                src/main.cpp
                src/calculator.cpp
                src/button.cpp
                src/parser.cpp
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
        ${EMBEDDED_RESOURCE_FILES}
    )
endif()

# Add fix_headers executable
add_executable(fix_headers src/fix_generated_headers.cpp)
target_link_libraries(fix_headers PRIVATE raylib)

# Add a custom command to run fix_headers after resource generation
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/headers_fixed.stamp
    COMMAND fix_headers
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS generate_resources fix_headers
    COMMENT "Fixing generated headers"
)

# Add a custom target for the header fixing step
add_custom_target(fix_headers_target ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/headers_fixed.stamp)

# Add dependency to ensure resources are generated and headers are fixed before building the main executable
add_dependencies(${PROJECT_NAME} generate_resources fix_headers_target)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE includes)

# Link libraries
# Main dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Add math library for non-Windows platforms
if(NOT IS_WINDOWS)
    target_link_libraries(${PROJECT_NAME} PRIVATE m)
endif()

# Platform-specific settings
# MSVC-specific settings
if(IS_MSVC)
    # Add preprocessor definitions for MSVC compatibility
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
        _CRT_SECURE_NO_WARNINGS
        _USE_MATH_DEFINES
    )
endif()

# macOS-specific settings
if(IS_MACOS)
    # Enable ARC (Automatic Reference Counting) for Objective-C/C++ code
    if(IS_CLANG)
        set_target_properties(${PROJECT_NAME} PROPERTIES
            XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
        )
    endif()
endif()

# Resource handling
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/resource)
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/resource/Ubuntu-Regular.ttf 
    ${CMAKE_CURRENT_BINARY_DIR}/resource/Ubuntu-Regular.ttf 
    COPYONLY
)

# Copy PNG icon file for all platforms and build types
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/resource/calc.png
    ${CMAKE_CURRENT_BINARY_DIR}/resource/calc.png
    COPYONLY
)
